/* ============================================================
 * 判题器 judge.js
 * - 用户代码一律在 Web Worker 中执行（Blob URL 内联创建），主线程绝不执行用户代码
 * - 引擎加载方式：主线程 fetch 引擎源码（多镜像回退 + localStorage 缓存），
 *   内联进 Worker 脚本。原因：JSCPP 打包文件在 Worker 环境判断
 *   window.document 失败会进入"自身作为运行器"分支，不暴露全局 JSCPP；
 *   预置 var window = self; window.document = {} 可让它正常导出。
 * - C / C++：JSCPP 解释器；Python：Pyodide（首次使用时下载约 10MB）
 * - 每个用例限时（题目 timeLimit，默认 3 秒）；超时 terminate 并重建 Worker
 * ============================================================ */
(function () {
    'use strict';

    var JSCPP_MIRRORS = [
        { url: 'https://cdn.jsdelivr.net/gh/felixhao28/JSCPP@gh-pages/dist/JSCPP.es5.min.js', indexURL: null },
        { url: 'https://raw.githubusercontent.com/felixhao28/JSCPP/gh-pages/dist/JSCPP.es5.min.js', indexURL: null }
    ];
    var PYODIDE_VERSION = '0.26.4';
    var PYODIDE_MIRRORS = [
        { url: 'https://cdn.jsdelivr.net/pyodide/v' + PYODIDE_VERSION + '/full/pyodide.js', indexURL: 'https://cdn.jsdelivr.net/pyodide/v' + PYODIDE_VERSION + '/full/' },
        { url: 'https://unpkg.com/pyodide@' + PYODIDE_VERSION + '/pyodide.js', indexURL: 'https://unpkg.com/pyodide@' + PYODIDE_VERSION + '/' }
    ];

    var LANG_CFG = {
        c:      { kind: 'jscpp', readyTimeout: 60000 },
        cpp:    { kind: 'jscpp', readyTimeout: 60000 },
        python: { kind: 'pyodide', readyTimeout: 300000 }
    };

    /* ---------- 引擎源码获取：优先用内置引擎（vendor/*_engine.js，离线可用），
       否则回退到 CDN 多镜像 + localStorage 缓存 ---------- */
    function cacheKey(kind) { return 'ctv1:lib:' + kind + ':v1'; }
    function readCache(kind) {
        try {
            var raw = localStorage.getItem(cacheKey(kind));
            if (!raw) return null;
            var obj = JSON.parse(raw);
            if (obj && obj.url && obj.src && obj.src.length > 5000) return obj;
        } catch (e) { /* 忽略 */ }
        return null;
    }
    function writeCache(kind, url, src) {
        try { localStorage.setItem(cacheKey(kind), JSON.stringify({ url: url, src: src })); } catch (e) { /* 超限等，忽略 */ }
    }
    function fetchText(url) {
        return fetch(url, { cache: 'force-cache' }).then(function (r) {
            if (!r.ok) throw new Error('HTTP ' + r.status);
            return r.text();
        });
    }
    var libCache = { jscpp: null, pyodide: null };   /* { url, indexURL, src } 会话内缓存 */

    function injectScript(src) {
        return new Promise(function (resolve, reject) {
            var s = document.createElement('script');
            s.src = src;
            s.onload = function () { resolve(); };
            s.onerror = function () { reject(new Error('无法加载 ' + src)); };
            document.head.appendChild(s);
        });
    }

    function ensureLib(kind) {
        if (libCache[kind]) return Promise.resolve(libCache[kind]);
        /* ⓪ Python 离线引擎约 17MB，首次用到时才动态加载（C/C++ 常驻） */
        if (kind === 'pyodide' && !(window.__ENGINE_SRC_PYODIDE && window.__ENGINE_SRC_PYODIDE.length > 5000)) {
            return injectScript('vendor/pyodide.engine.js').then(function () {
                return ensureLib(kind);
            });
        }
        /* ① 页面已通过 <script> 内置引擎源码（首选，完全离线可用） */
        var embedded = (kind === 'jscpp') ? window.__ENGINE_SRC_JSCPP : window.__ENGINE_SRC_PYODIDE;
        if (embedded && embedded.length > 5000) {
            var embMirrors = (kind === 'jscpp') ? JSCPP_MIRRORS : PYODIDE_MIRRORS;
            libCache[kind] = { url: '', indexURL: embMirrors[0].indexURL, src: embedded };
            return Promise.resolve(libCache[kind]);
        }
        /* ② 回退：CDN 镜像（网页被人手动删掉 vendor 时仍可工作，需要联网） */
        var cached = readCache(kind);
        var mirrors = (kind === 'jscpp') ? JSCPP_MIRRORS : PYODIDE_MIRRORS;
        function validate(src) {
            if (!src) return false;
            if (kind === 'jscpp') return src.length > 100000 && src.indexOf('JSCPP') >= 0;
            return src.length > 5000 && src.indexOf('loadPyodide') >= 0;
        }
        var chain = Promise.reject();
        if (cached && validate(cached.src)) {
            chain = Promise.resolve(cached).then(function (c) {
                var m = mirrors.filter(function (x) { return x.url === c.url; })[0];
                return { url: c.url, indexURL: m ? m.indexURL : null, src: c.src };
            });
        } else {
            mirrors.forEach(function (m) {
                chain = chain.catch(function () {
                    return fetchText(m.url).then(function (src) {
                        if (!validate(src)) throw new Error('内容校验失败');
                        writeCache(kind, m.url, src);
                        libCache[kind] = { url: m.url, indexURL: m.indexURL, src: src };
                        return libCache[kind];
                    });
                });
            });
        }
        return chain.then(function (lib) { libCache[kind] = lib; return lib; });
    }

    /* ---------- Worker 脚本组装 ---------- */
    var RUN_HANDLER = [
        ';self.onmessage = function (ev) {',
        '  var d = ev.data;',
        '  if (d && d.type === "ping") { self.postMessage({ type: "ready", ok: true }); return; }',
        '  if (!d || d.type !== "run") return;',
        '  var out = "";',
        '  try {',
        '    __run(d.code, d.input, function (s) { out += s; }, function () {',
        '      self.postMessage({ type: "caseResult", id: d.id, ok: true, output: out });',
        '    }, function (err) {',
        '      self.postMessage({ type: "caseResult", id: d.id, ok: false, output: out, error: String((err && err.message) || err) });',
        '    });',
        '  } catch (err) {',
        '    self.postMessage({ type: "caseResult", id: d.id, ok: false, output: out, error: String((err && err.message) || err) });',
        '  }',
        '};'
    ].join('\n');

    var JSCPP_RUNNER = [
        'function __run(code, input, write, done, fail) {',
        '  try {',
        '    JSCPP.run(code, input, { stdio: { write: write } });',
        '    done();',
        '  } catch (e) { fail(e); }',
        '}'
    ].join('\n');

    /* Python Worker 专属逻辑：loadPyodide 异步启动，启动完成后才应答 ready；
       启动期间到达的消息进入队列，避免首条判题消息撞上未就绪的运行时 */
    var PY_HANDLER = [
        'var __py = null, __accepting = false, __queue = [], __pyCollect = null;',
        'function __handle(d) {',
        '  var out = "";',
        '  __pyCollect = function (s) { out += s + "\\n"; };',
        '  try {',
        '    var lines = String(d.input).replace(/\\r/g, "").split("\\n");',
        '    __py.globals.set("__LINES", lines);',
        '    __py.runPython([',
        '      "def input(prompt=\'\'):",',
        '      "    if not __LINES:",',
        '      "        raise EOFError(\'输入数据不足\')",',
        '      "    return __LINES.pop(0)"',
        '    ].join("\\n"));',
        '    __py.runPython(d.code);',
        '    __py.runPython("import sys; sys.stdout.flush(); sys.stderr.flush()");',
        '    self.postMessage({ type: "caseResult", id: d.id, ok: true, output: out });',
        '  } catch (e) {',
        '    var msg = String((e && e.message) || e);',
        '    var parts = msg.split("\\n").filter(function (l) { return l.trim(); });',
        '    self.postMessage({ type: "caseResult", id: d.id, ok: false, output: out, error: parts.length ? parts[parts.length - 1] : msg });',
        '  } finally { __pyCollect = null; }',
        '}',
        'self.onmessage = function (ev) {',
        '  if (!__accepting) { __queue.push(ev.data); return; }',
        '  __handle(ev.data);',
        '};',
        '(async function () {',
        '  try {',
        '    __py = await loadPyodide({ indexURL: __INDEX_URL_JSON });',
        '    __py.setStdout({ batched: function (s) { if (__pyCollect) __pyCollect(s); } });',
        '    __py.setStderr({ batched: function (s) { if (__pyCollect) __pyCollect(s); } });',
        '    __accepting = true;',
        '    self.postMessage({ type: "ready", ok: true });',
        '    while (__queue.length) __handle(__queue.shift());',
        '  } catch (e) {',
        '    self.postMessage({ type: "ready", ok: false, error: "PYTHON_BOOT:" + String((e && e.message) || e) });',
        '  }',
        '})();'
    ].join('\n');

    function buildWorkerSrc(kind, lib) {
        if (kind === 'jscpp') {
            return [
                'var window = self; window.document = {};',
                lib.src,
                ';var JSCPP = window.JSCPP;',
                'if (typeof JSCPP === "undefined") { self.postMessage({ type: "bootError", error: "JSCPP_UNDEFINED" }); }',
                JSCPP_RUNNER,
                RUN_HANDLER
            ].join('\n');
        }
        return [
            lib.src,
            ';var __INDEX_URL_JSON = ' + JSON.stringify(lib.indexURL || '') + ';',
            PY_HANDLER
        ].join('\n');
    }

    function makeWorker(src) {
        var blob = new Blob([src], { type: 'application/javascript' });
        return new Worker(URL.createObjectURL(blob));
    }

    /* ---------- 每种语言一个可重建的 Worker 槽 ---------- */
    function WorkerSlot(lang) {
        var cfg = LANG_CFG[lang];
        this.lang = lang;
        this.kind = cfg.kind;
        this.readyTimeout = cfg.readyTimeout;
        this.w = null;
        this.dead = false;
        this.ready = null;
    }
    WorkerSlot.prototype._fail = function (reason) {
        if (this._settled) return;
        this._settled = true;
        clearTimeout(this._timer);
        if (reason === 'LOAD_FAILED') this.dead = true;
        if (this.w) { this.w.terminate(); this.w = null; }
        this.ready = null;
        this._reject(reason);
    };
    WorkerSlot.prototype.ensure = function () {
        var self = this;
        if (this.dead) return Promise.reject('LOAD_FAILED');
        if (this.w) return this.ready;
        this._settled = false;
        this.ready = new Promise(function (resolve, reject) {
            self._resolve = resolve;
            self._reject = reject;
        });
        this._timer = setTimeout(function () { self._fail('LOAD_TIMEOUT'); }, this.readyTimeout);
        ensureLib(this.kind).then(function (lib) {
            if (self._settled) return;
            var w;
            try {
                w = makeWorker(buildWorkerSrc(self.kind, lib));
            } catch (e) {
                self._fail('WORKER_ERROR');
                return;
            }
            self.w = w;
            w._seq = 0;
            w._pending = {};
            w.onmessage = function (ev) {
                var d = ev.data;
                if (d && d.type === 'ready') {
                    self._settled = true;
                    clearTimeout(self._timer);
                    if (d.ok) self._resolve(w);
                    else self._fail(d.error || 'LOAD_FAILED');
                    return;
                }
                if (d && d.type === 'bootError') {
                    self._fail('BOOT_FAILED:' + (d.error || ''));
                    return;
                }
                if (d && d.type === 'caseResult') {
                    var p = w._pending[d.id];
                    if (p) { delete w._pending[d.id]; p(d); }
                }
            };
            w.onerror = function () { self._fail('WORKER_ERROR'); };
            w.postMessage({ type: 'ping' });
        }, function () {
            self._fail('LOAD_FAILED');
        });
        return this.ready;
    };
    WorkerSlot.prototype.isReady = function () {
        return !!(this.w && this._settled);
    };
    WorkerSlot.prototype.kill = function () {
        if (this.w) { this.w.terminate(); this.w = null; this.ready = null; this._settled = false; }
    };

    var slots = {
        c: new WorkerSlot('c'),
        cpp: new WorkerSlot('cpp'),
        python: new WorkerSlot('python')
    };

    /* ---------- 错误信息中文化 ---------- */
    function friendlyError(lang, raw) {
        var s = String(raw || '');
        if (s === 'LOAD_FAILED') {
            return '运行引擎源码下载失败（已尝试多个镜像），请检查网络后重试；若持续失败，可能是浏览器扩展拦截，可换浏览器或暂停扩展试试';
        }
        if (s === 'WORKER_ERROR' || s === 'LOAD_TIMEOUT') return '运行环境异常，请重试；若持续出现请刷新页面';
        if (/^PYTHON_BOOT/.test(s)) return 'Python 运行时（Pyodide）初始化失败：' + s.slice(12) + '——引擎已完整内置，通常是浏览器内存不足，请刷新页面重试';
        if (s.indexOf('BOOT_FAILED') === 0) return 'C/C++ 解释器初始化失败，请刷新页面重试';
        if (s === 'PYTHON_NOT_READY') return 'Python 运行时仍在加载中，请稍候几秒再试';
        if (/输入数据不足/.test(s)) return '输入数据不足：程序读取的内容超出了该用例提供的数据（EOFError）';
        if (s === 'EOF') return '输入数据不足：程序读取的内容超出了该用例提供的数据（请检查读入量与格式）';
        if (/Memory overflow/i.test(s)) return '输入读取异常：数据不足或读入格式与用例不符';
        if (/EOFError/i.test(s)) return '输入数据不足：程序读取的内容超出了该用例提供的数据';
        if (/Preprocessing Failure/i.test(s)) return '编译错误：代码预处理失败——请检查注释与语法是否完整（注释之外需要有实际代码）';
        if (/ModuleNotFoundError/i.test(s)) return '使用了不可用的第三方库：' + s;
        if (lang === 'python') return '运行出错：' + s;
        if (/overflow of/i.test(s)) return '数值溢出：运算结果超出了变量类型的表示范围（' + s + '）';
        if (/does not exist|not defined|no method/i.test(s)) return '程序错误：' + s + '（注意：本站解释器不支持 <string.h> 函数与 struct，请改用手写循环）';
        return s;
    }

    function classify(raw) {
        var s = String(raw || '');
        if (/Parsing Failure|Preprocessing Failure|SyntaxError/i.test(s)) return 'compile-error';
        return 'runtime-error';
    }

    /* 去掉注释后是否还有实际代码（JSCPP 对"纯注释文件"会预处理失败，提前拦截给友好提示） */
    function hasRealCode(code, lang) {
        var s = String(code || '');
        if (lang === 'python') {
            s = s.replace(/#[^\n]*/g, '');
        } else {
            s = s.replace(/\/\*[\s\S]*?\*\//g, '').replace(/\/\/[^\n]*/g, '');
        }
        return s.trim() !== '';
    }

    /* ---------- 输出归一化与比较 ---------- */
    function normalizeOutput(s) {
        return String(s).replace(/\r/g, '')
            .split('\n')
            .map(function (l) { return l.replace(/[ \t]+$/, ''); })
            .join('\n')
            .replace(/\n+$/, '');
    }
    function compareOutput(actual, expected) {
        return normalizeOutput(actual) === normalizeOutput(expected);
    }

    /* 输入规范化：统一 \r\n → \n；结尾补换行。
     * 真实 C 的 scanf/getchar 读到最后一个数据后不会因"没有换行"而报错，
     * 而解释器的行读取依赖行尾换行，这里统一补齐，用户用例少打一个换行也不影响判题 */
    function normalizeInput(s) {
        s = String(s == null ? '' : s).replace(/\r\n?/g, '\n');
        return (s.length && !s.endsWith('\n')) ? s + '\n' : s;
    }

    /* ---------- 执行单个用例 ---------- */
    function run(lang, code, input, timeLimit) {
        var slot = slots[lang] || slots.c;
        if (!hasRealCode(code, lang)) {
            return Promise.resolve({
                status: 'compile-error',
                output: '',
                error: lang === 'python'
                    ? '编辑器里还没有有效代码——请在任务注释之外写出完整的 Python 程序（用 input() 读入、print() 输出）'
                    : '编辑器里还没有有效代码——请在任务注释之外写出完整的程序（包含头文件与 main 函数）'
            });
        }
        return slot.ensure().then(function (w) {
            return new Promise(function (resolve) {
                var id = ++w._seq;
                var done = false;
                var timer = setTimeout(function () {
                    if (done) return;
                    done = true;
                    delete w._pending[id];
                    slot.kill();   /* 超时销毁，下次判题自动重建 */
                    resolve({ status: 'tle', output: '', error: '' });
                }, (timeLimit || 3000) + 500);
                w._pending[id] = function (d) {
                    if (done) return;
                    done = true;
                    clearTimeout(timer);
                    if (d.ok) {
                        resolve({ status: 'ok', output: d.output, error: '' });
                    } else {
                        var raw = d.error || '';
                        if (raw === 'LOAD_FAILED' || raw === 'WORKER_ERROR' || raw === 'LOAD_TIMEOUT' || raw.indexOf('BOOT_FAILED') === 0 || raw === 'PYTHON_NOT_READY') {
                            slot.kill();
                            resolve({ status: 'load-failed', output: d.output || '', error: friendlyError(lang, raw) });
                        } else {
                            resolve({
                                status: classify(raw),
                                output: d.output || '',
                                error: friendlyError(lang, raw),
                                rawError: raw
                            });
                        }
                    }
                };
                w.postMessage({ type: 'run', id: id, code: code, input: normalizeInput(input) });
            });
        }, function (reason) {
            return Promise.resolve({
                status: 'load-failed',
                output: '',
                error: friendlyError(lang, String(reason))
            });
        });
    }

    window.JUDGE = {
        run: run,
        normalizeOutput: normalizeOutput,
        compareOutput: compareOutput,
        isReady: function (lang) { var s = slots[lang]; return !s || s.isReady(); }
    };
})();
