/* ============================================================
 * 前端应用 app.js —— hash 路由单页应用
 * 页面：题库列表 / 题目详情（左题右码）/ 进度统计
 * 视觉：力扣（LeetCode）风格 —— 现代下拉、题库抽屉、
 *       测试用例/测试结果双面板、参考代码高亮
 * ============================================================ */
(function () {
    'use strict';

    var BANK = window.QUESTION_BANK;
    var STORE = window.STORE;
    var JUDGE = window.JUDGE;

    var DIFF_LABEL = { easy: '简单', medium: '中等', hard: '困难' };
    var DIFF_ORDER = ['easy', 'medium', 'hard'];

    var STATUS_TEXT = {
        ok: '通过',
        tle: '超出时间限制',
        'runtime-error': '运行时错误',
        'compile-error': '编译/解析错误',
        'load-failed': '环境不可用'
    };

    function $(sel, root) { return (root || document).querySelector(sel); }
    function byId(id) { return document.getElementById(id); }

    function esc(s) {
        return String(s).replace(/&/g, '&amp;').replace(/</g, '&lt;')
            .replace(/>/g, '&gt;').replace(/"/g, '&quot;');
    }

    function trunc(s, n) {
        s = String(s === null || s === undefined ? '' : s);
        return s.length > n ? s.slice(0, n) + '…' : s;
    }

    function h(tag, cls, html) {
        var e = document.createElement(tag);
        if (cls) e.className = cls;
        if (html !== undefined) e.innerHTML = html;
        return e;
    }

    /* ============================================================
     * SVG 图标库（stroke 风格，随 currentColor 变色）
     * ============================================================ */
    var ICON_PATHS = {
        check: '<path d="M20 6L9 17l-5-5"/>',
        x: '<path d="M18 6L6 18M6 6l12 12"/>',
        plus: '<path d="M12 5v14M5 12h14"/>',
        chevronDown: '<path d="M6 9l6 6 6-6"/>',
        chevronRight: '<path d="M9 18l6-6-6-6"/>',
        copy: '<rect x="9" y="9" width="13" height="13" rx="2"/><path d="M5 15H4a2 2 0 0 1-2-2V4a2 2 0 0 1 2-2h9a2 2 0 0 1 2 2v1"/>',
        heart: '<path d="M20.8 4.6a5.5 5.5 0 0 0-7.8 0L12 5.7l-1-1.1a5.5 5.5 0 0 0-7.8 7.8l1.1 1L12 21l7.7-7.6 1.1-1a5.5 5.5 0 0 0 0-7.8z"/>',
        list: '<path d="M8 6h13M8 12h13M8 18h13M3 6h.01M3 12h.01M3 18h.01"/>',
        search: '<circle cx="11" cy="11" r="7"/><path d="M21 21l-4.3-4.3"/>',
        clock: '<circle cx="12" cy="12" r="10"/><path d="M12 6v6l4 2"/>',
        terminal: '<polyline points="4 17 10 11 4 5"/><line x1="12" y1="19" x2="20" y2="19"/>',
        checkSquare: '<polyline points="9 11 12 14 22 4"/><path d="M21 12v7a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2V5a2 2 0 0 1 2-2h11"/>',
        code: '<polyline points="16 18 22 12 16 6"/><polyline points="8 6 2 12 8 18"/>',
        zap: '<polygon points="13 2 3 14 12 14 11 22 21 10 12 10 13 2"/>',
        cloud: '<path d="M12 13v8M8 17l4 4 4-4"/><path d="M20.88 18.09A5 5 0 0 0 18 9h-1.26A8 8 0 1 0 3 16.29"/>',
        gear: '<circle cx="12" cy="12" r="3"/><path d="M19.4 15a1.65 1.65 0 0 0 .33 1.82l.06.06a2 2 0 1 1-2.83 2.83l-.06-.06a1.65 1.65 0 0 0-1.82-.33 1.65 1.65 0 0 0-1 1.51V21a2 2 0 1 1-4 0v-.09a1.65 1.65 0 0 0-1-1.51 1.65 1.65 0 0 0-1.82.33l-.06.06a2 2 0 1 1-2.83-2.83l.06-.06a1.65 1.65 0 0 0 .33-1.82 1.65 1.65 0 0 0-1.51-1H3a2 2 0 1 1 0-4h.09a1.65 1.65 0 0 0 1.51-1 1.65 1.65 0 0 0-.33-1.82l-.06-.06a2 2 0 1 1 2.83-2.83l.06.06a1.65 1.65 0 0 0 1.82.33h.08a1.65 1.65 0 0 0 1-1.51V3a2 2 0 1 1 4 0v.09a1.65 1.65 0 0 0 1 1.51h.08a1.65 1.65 0 0 0 1.82-.33l.06-.06a2 2 0 1 1 2.83 2.83l-.06.06a1.65 1.65 0 0 0-.33 1.82v.08a1.65 1.65 0 0 0 1.51 1H21a2 2 0 1 1 0 4h-.09a1.65 1.65 0 0 0-1.51 1z"/>',
        star: '<polygon points="12 2 15.09 8.26 22 9.27 17 14.14 18.18 21.02 12 17.77 5.82 21.02 7 14.14 2 9.27 8.91 8.26 12 2"/>',
        maximize: '<path d="M8 3H5a2 2 0 0 0-2 2v3M16 3h3a2 2 0 0 1 2 2v3M8 21H5a2 2 0 0 1-2-2v-3M16 21h3a2 2 0 0 0 2-2v-3"/>',
        chevronUp: '<path d="M18 15l-6-6-6 6"/>',
        calendar: '<rect x="3" y="4" width="18" height="18" rx="2"/><line x1="16" y1="2" x2="16" y2="6"/><line x1="8" y1="2" x2="8" y2="6"/><line x1="3" y1="10" x2="21" y2="10"/>'
    };
    function ic(name, size) {
        size = size || 16;
        return '<svg width="' + size + '" height="' + size + '" viewBox="0 0 24 24" fill="none" ' +
            'stroke="currentColor" stroke-width="2.2" stroke-linecap="round" stroke-linejoin="round">' +
            (ICON_PATHS[name] || '') + '</svg>';
    }

    /* ---------- 复制到剪贴板（file:// 下带降级方案） ---------- */
    function copyText(t) {
        function done() { toast('已复制到剪贴板', 'ok'); }
        function fallback() {
            var ta = document.createElement('textarea');
            ta.value = t;
            ta.style.position = 'fixed';
            ta.style.opacity = '0';
            document.body.appendChild(ta);
            ta.select();
            try { document.execCommand('copy'); done(); }
            catch (e) { toast('复制失败，请手动选择复制', 'warn'); }
            ta.remove();
        }
        try {
            if (navigator.clipboard && navigator.clipboard.writeText) {
                navigator.clipboard.writeText(t).then(done, fallback);
                return;
            }
        } catch (e) { /* 走降级 */ }
        fallback();
    }

    /* ---------- Markdown 渲染（失败降级为纯文本） ---------- */
    function mdNode(text, cls) {
        var box = h('div', 'md ' + (cls || ''));
        try {
            if (window.marked && window.DOMPurify) {
                var html = window.marked.parse ? window.marked.parse(text) : window.marked(text);
                box.innerHTML = window.DOMPurify.sanitize(html);
                return box;
            }
        } catch (e) { /* 降级 */ }
        box.classList.add('md-plain');
        box.textContent = text;
        return box;
    }

    /* ---------- Toast ---------- */
    function toast(msg, type) {
        var box = $('#toast-box');
        if (!box) { box = h('div'); box.id = 'toast-box'; document.body.appendChild(box); }
        var t = h('div', 'toast ' + (type || 'info'), esc(msg));
        box.appendChild(t);
        setTimeout(function () {
            t.classList.add('toast-out');
            setTimeout(function () { t.remove(); }, 300);
        }, 2400);
    }

    /* ---------- 自定义确认弹层（禁用原生 confirm） ---------- */
    function confirmDialog(message) {
        return new Promise(function (resolve) {
            var overlay = h('div', 'overlay');
            var panel = h('div', 'dialog');
            panel.appendChild(h('div', 'dialog-msg', esc(message)));
            var row = h('div', 'dialog-row');
            var ok = h('button', 'btn btn-primary', '确定');
            var cancel = h('button', 'btn', '取消');
            row.appendChild(cancel); row.appendChild(ok);
            panel.appendChild(row);
            overlay.appendChild(panel);
            document.body.appendChild(overlay);
            function close(v) { overlay.remove(); resolve(v); }
            ok.addEventListener('click', function () { close(true); });
            cancel.addEventListener('click', function () { close(false); });
            overlay.addEventListener('click', function (ev) { if (ev.target === overlay) close(false); });
        });
    }

    /* ============================================================
     * 现代化下拉组件（替换原生 <select>）
     * 用法：createDropdown({ value, options:[{value,label,dot,icon}], onChange, className })
     * ============================================================ */
    function closeAllDropdowns() {
        document.querySelectorAll('.dd.open').forEach(function (d) { d.classList.remove('open'); });
    }
    function createDropdown(o) {
        var root = h('div', 'dd' + (o.className ? ' ' + o.className : ''));
        var btn = h('button', 'dd-btn');
        btn.type = 'button';
        var menu = h('div', 'dd-menu');
        root.appendChild(btn);
        root.appendChild(menu);
        root._value = o.value;

        function dotHtml(dot) { return dot ? '<span class="dd-dot dd-dot-' + dot + '"></span>' : ''; }

        function render() {
            var cur = null;
            o.options.forEach(function (op) { if (op.value === root._value) cur = op; });
            btn.innerHTML =
                (cur && cur.icon ? '<span class="dd-btn-ic">' + cur.icon + '</span>' : '') +
                (cur ? dotHtml(cur.dot) : '') +
                '<span class="dd-btn-label">' + esc(cur ? cur.label : (o.placeholder || '请选择')) + '</span>' +
                '<span class="dd-caret">' + ic('chevronDown', 14) + '</span>';
            menu.innerHTML = '';
            o.options.forEach(function (op) {
                var it = h('button', 'dd-item' + (op.value === root._value ? ' selected' : ''));
                it.type = 'button';
                it.innerHTML =
                    (op.icon ? '<span class="dd-btn-ic">' + op.icon + '</span>' : '') +
                    dotHtml(op.dot) +
                    '<span class="dd-label">' + esc(op.label) + '</span>' +
                    '<span class="dd-check">' + (op.value === root._value ? ic('check', 14) : '') + '</span>';
                it.addEventListener('click', function (ev) {
                    ev.stopPropagation();
                    root._value = op.value;
                    root.classList.remove('open');
                    render();
                    if (o.onChange) o.onChange(op.value, op);
                });
                menu.appendChild(it);
            });
        }

        btn.addEventListener('click', function (ev) {
            ev.stopPropagation();
            var wasOpen = root.classList.contains('open');
            closeAllDropdowns();
            if (!wasOpen) root.classList.add('open');
        });
        root._setValue = function (v) { root._value = v; render(); };
        root._getValue = function () { return root._value; };
        render();
        return root;
    }

    /* ============================================================
     * CodeMirror 只读高亮（参考代码 / 历史代码），失败降级 <pre>
     * ============================================================ */
    function cmTheme() { return document.body.dataset.theme === 'dark' ? 'material-darker' : 'default'; }
    function langMode(l) {
        if (l === 'python') return 'text/x-python';
        if (l === 'cpp') return 'text/x-c++src';
        return 'text/x-csrc';
    }
    function makeReadOnly(container, code, mode) {
        if (window.CodeMirror) {
            try {
                var cm = window.CodeMirror(container, {
                    value: code,
                    mode: mode,
                    readOnly: true,
                    lineNumbers: true,
                    theme: cmTheme(),
                    lineWrapping: false,
                    indentUnit: 4
                });
                cm._kind = 'cm';
                applyEditorSettings(cm);
                setTimeout(function () { try { cm.refresh(); } catch (e) { } }, 0);
                return cm;
            } catch (e) { /* 降级 */ }
        }
        var pre = h('pre', 'ref-code');
        pre.textContent = code;
        container.appendChild(pre);
        return null;
    }

    /* ============================================================
     * 输出 diff 高亮（逐字符 LCS：实际多的标红，预期缺的标绿）
     * ============================================================ */
    function diffLcs(actual, expected) {
        var a = String(actual || ''), b = String(expected || '');
        var n = a.length, m = b.length;
        if (n * m > 250000) return null;
        var dp = [];
        for (var i = 0; i <= n; i++) dp.push(new Uint16Array(m + 1));
        for (i = 1; i <= n; i++) {
            for (var j = 1; j <= m; j++) {
                dp[i][j] = a[i - 1] === b[j - 1]
                    ? dp[i - 1][j - 1] + 1
                    : Math.max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
        var pa = [], pb = [];
        i = n; j = m;
        while (i > 0 || j > 0) {
            if (i > 0 && j > 0 && a[i - 1] === b[j - 1]) {
                pa.push({ ch: a[i - 1], bad: false });
                pb.push({ ch: b[j - 1], good: false });
                i--; j--;
            } else if (j > 0 && (i === 0 || dp[i][j - 1] >= dp[i - 1][j])) {
                pb.push({ ch: b[j - 1], good: true });
                j--;
            } else {
                pa.push({ ch: a[i - 1], bad: true });
                i--;
            }
        }
        pa.reverse(); pb.reverse();
        return { a: pa, b: pb };
    }
    function diffActualHtml(actual, expected) {
        var d = diffLcs(actual, expected);
        if (!d) return esc(actual);
        return d.a.map(function (p) {
            return p.bad ? '<span class="diff-bad">' + esc(p.ch) + '</span>' : esc(p.ch);
        }).join('');
    }
    function diffExpectedHtml(actual, expected) {
        var d = diffLcs(actual, expected);
        if (!d) return esc(expected);
        return d.b.map(function (p) {
            return p.good ? '<span class="diff-good">' + esc(p.ch) + '</span>' : esc(p.ch);
        }).join('');
    }

    /* ---------- 难度 / 状态徽标 ---------- */
    function diffBadge(d) {
        var cn = { easy: '简单', medium: '中等', hard: '困难' }[d] || d;
        return '<span class="diff diff-' + d + '"><i class="diff-dot"></i>' + cn + '</span>';
    }
    function statusIcon(p) {
        if (!p || !p.status) return '<span class="st st-none">—</span>';
        if (p.status === 'solved') return '<span class="st st-solved">✓</span>';
        return '<span class="st st-attempted">✗</span>';
    }
    function statusText(p) {
        if (!p || !p.status) return '未开始';
        return p.status === 'solved' ? '已解决' : '尝试过';
    }

    function getQuestion(id) {
        for (var i = 0; i < BANK.length; i++) if (BANK[i].id === id) return BANK[i];
        return null;
    }

    /* ---------- 收藏 / 编辑器设置 / 布局持久化 ---------- */
    function getFavs() {
        var f = STORE.get('favorites', []);
        return Array.isArray(f) ? f : [];
    }
    function isFav(id) { return getFavs().indexOf(id) >= 0; }
    function toggleFav(id) {
        var f = getFavs();
        var i = f.indexOf(id);
        if (i >= 0) f.splice(i, 1); else f.push(id);
        STORE.set('favorites', f);
        return i < 0;
    }

    var SETTINGS = null;
    function getSettings() {
        if (!SETTINGS) {
            SETTINGS = STORE.get('settings', {}) || {};
            if (typeof SETTINGS.fontSize !== 'number') SETTINGS.fontSize = 13.5;
            if (SETTINGS.tab !== 2 && SETTINGS.tab !== 4) SETTINGS.tab = 4;
            SETTINGS.wrap = !!SETTINGS.wrap;
        }
        return SETTINGS;
    }
    function saveSettings() { STORE.set('settings', SETTINGS); }

    function applyEditorSettings(cm) {
        if (!cm || cm._kind !== 'cm') return;
        try {
            cm.getWrapperElement().style.fontSize = getSettings().fontSize + 'px';
            cm.setOption('indentUnit', getSettings().tab);
            cm.setOption('tabSize', getSettings().tab);
            cm.setOption('lineWrapping', getSettings().wrap);
            cm.refresh();
        } catch (e) { /* 忽略 */ }
    }

    function getLayout() {
        return STORE.get('layout', {}) || {};
    }
    function saveLayout(patch) {
        var l = getLayout();
        Object.keys(patch).forEach(function (k) { l[k] = patch[k]; });
        STORE.set('layout', l);
    }

    /* ============================================================
     * 题库抽屉（力扣式左侧滑出题目列表）
     * ============================================================ */
    var drawerState = null;
    function openProblemDrawer() {
        if (drawerState) return;
        var curId = 0;
        if (location.hash.indexOf('#/problem/') === 0) {
            curId = parseInt(location.hash.slice('#/problem/'.length), 10) || 0;
        }
        var overlay = h('div', 'drawer-overlay');
        var panel = h('aside', 'drawer');

        var head = h('div', 'drawer-head');
        head.innerHTML =
            '<div class="drawer-title">' + ic('list', 18) + '<span>题库</span>' +
            '<span class="drawer-count" id="dw-count"></span></div>';
        var closeBtn = h('button', 'icon-btn', ic('x', 18));
        closeBtn.title = '关闭（Esc）';
        head.appendChild(closeBtn);
        panel.appendChild(head);

        var filtersBox = h('div', 'drawer-filters');
        var search = document.createElement('input');
        search.className = 'input search dw-search';
        search.type = 'text';
        search.placeholder = '搜索题号或标题…';
        search.value = filters.search;
        filtersBox.appendChild(search);

        var ddRow = h('div', 'dd-row');
        var ddDiff = createDropdown({
            value: filters.difficulty, className: 'dd-flex',
            options: [
                { value: 'all', label: '全部难度' },
                { value: 'easy', label: '简单', dot: 'easy' },
                { value: 'medium', label: '中等', dot: 'medium' },
                { value: 'hard', label: '困难', dot: 'hard' }
            ],
            onChange: function (v) { filters.difficulty = v; renderRows(); }
        });
        var ddStatus = createDropdown({
            value: filters.status, className: 'dd-flex',
            options: [
                { value: 'all', label: '全部状态' },
                { value: 'none', label: '未开始' },
                { value: 'attempted', label: '尝试过' },
                { value: 'solved', label: '已解决' }
            ],
            onChange: function (v) { filters.status = v; renderRows(); }
        });
        ddRow.appendChild(ddDiff);
        ddRow.appendChild(ddStatus);
        filtersBox.appendChild(ddRow);
        var ddTag = createDropdown({
            value: filters.tag, className: 'dd-block',
            options: [{ value: 'all', label: '全部标签' }].concat(allTags().map(function (t) { return { value: t, label: t }; })),
            onChange: function (v) { filters.tag = v; renderRows(); }
        });
        filtersBox.appendChild(ddTag);
        panel.appendChild(filtersBox);

        var list = h('div', 'drawer-list');
        panel.appendChild(list);
        overlay.appendChild(panel);
        document.body.appendChild(overlay);
        document.body.classList.add('drawer-open');

        function close() {
            overlay.remove();
            document.body.classList.remove('drawer-open');
            document.removeEventListener('keydown', onKey);
            drawerState = null;
        }
        function onKey(ev) {
            if (ev.key === 'Escape') { closeAllDropdowns(); close(); }
        }
        document.addEventListener('keydown', onKey);
        overlay.addEventListener('click', function (ev) { if (ev.target === overlay) close(); });
        closeBtn.addEventListener('click', close);

        var searchTimer = null;
        search.addEventListener('input', function () {
            clearTimeout(searchTimer);
            searchTimer = setTimeout(function () { filters.search = search.value; renderRows(); }, 250);
        });

        function renderRows() {
            var items = filteredList();
            var cnt = $('#dw-count', head);
            if (cnt) cnt.textContent = items.length + ' / ' + BANK.length;
            list.innerHTML = '';
            if (!items.length) {
                list.appendChild(h('div', 'dw-empty', '没有符合条件的题目'));
                return;
            }
            items.forEach(function (q) {
                var p = STORE.getProgressOf(q.id);
                var row = h('button', 'dw-row' + (q.id === curId ? ' current' : ''));
                row.type = 'button';
                row.innerHTML =
                    '<span class="dw-st">' + statusIcon(p) + '</span>' +
                    '<span class="dw-code">' + esc(q.code) + '</span>' +
                    '<span class="dw-title">' + esc(q.title) + '</span>' +
                    diffBadge(q.difficulty);
                row.addEventListener('click', function () {
                    close();
                    if (q.id !== curId) location.hash = '#/problem/' + q.id;
                });
                list.appendChild(row);
            });
        }
        renderRows();
        drawerState = { close: close };
        setTimeout(function () { try { search.focus(); } catch (e) { } }, 120);
    }

    /* ============================================================
     * 列表页（力扣题库式：左侧栏 + 胶囊筛选 + 44px 斑马纹行）
     * ============================================================ */
    var filters = { search: '', difficulty: 'all', status: 'all', tag: 'all', sort: 'default', fav: false };
    var searchTimer = null;

    function allTags() {
        var seen = {};
        var out = [];
        BANK.forEach(function (q) {
            (q.tags || []).forEach(function (t) {
                if (!seen[t]) { seen[t] = 1; out.push(t); }
            });
        });
        return out;
    }

    function progressStats() {
        var stat = { easy: [0, 0], medium: [0, 0], hard: [0, 0] };
        BANK.forEach(function (q) {
            stat[q.difficulty][1]++;
            var p = STORE.getProgressOf(q.id);
            if (p && p.status === 'solved') stat[q.difficulty][0]++;
        });
        return stat;
    }

    function filteredList() {
        var kw = filters.search.trim().toLowerCase();
        var list = BANK.filter(function (q) {
            if (filters.difficulty !== 'all' && q.difficulty !== filters.difficulty) return false;
            if (filters.tag !== 'all' && (q.tags || []).indexOf(filters.tag) < 0) return false;
            if (filters.fav && !isFav(q.id)) return false;
            var p = STORE.getProgressOf(q.id);
            var st = (p && p.status) || 'none';
            if (filters.status !== 'all' && st !== filters.status) return false;
            if (kw && (String(q.code) + ' ' + q.title).toLowerCase().indexOf(kw) < 0) return false;
            return true;
        });
        if (filters.sort === 'diff-asc') {
            list.sort(function (a, b) { return DIFF_ORDER.indexOf(a.difficulty) - DIFF_ORDER.indexOf(b.difficulty) || a.id - b.id; });
        } else if (filters.sort === 'diff-desc') {
            list.sort(function (a, b) { return DIFF_ORDER.indexOf(b.difficulty) - DIFF_ORDER.indexOf(a.difficulty) || a.id - b.id; });
        }
        return list;
    }

    /* 难度条形图：5 根小竖条，按难度点亮 1/3/5 根 */
    function diffBars(d) {
        var lit = { easy: 1, medium: 3, hard: 5 }[d] || 0;
        var s = '<span class="dbars dbars-' + d + '">';
        for (var i = 1; i <= 5; i++) s += '<i' + (i <= lit ? ' class="on"' : '') + '></i>';
        return s + '</span>';
    }

    /* 每日一题：按日期稳定轮换 */
    function dailyPick() {
        var day = Math.floor(Date.now() / 86400000);
        return BANK[day % BANK.length];
    }

    function renderList() {
        var stat = progressStats();
        var solvedAll = stat.easy[0] + stat.medium[0] + stat.hard[0];
        var wrap = h('div', 'page list-page');

        wrap.innerHTML =
            '<aside class="side-col">' +
            '  <nav class="side-card side-menu">' +
            '    <a class="side-item active" href="#/problems">' + ic('code', 17) + '<span>题目</span></a>' +
            '    <a class="side-item" href="#/progress">' + ic('checkSquare', 17) + '<span>进度</span></a>' +
            '  </nav>' +
            '  <div class="side-card" id="side-progress"></div>' +
            '  <div class="side-card side-daily" id="side-daily"></div>' +
            '</aside>' +
            '<div class="main-col">' +
            '  <div class="pill-row" id="pill-row"></div>' +
            '  <div class="toolbar">' +
            '    <input id="f-search" class="input search" type="text" placeholder="搜索题目" />' +
            '    <span id="f-dd-slot" class="dd-slot"></span>' +
            '    <span class="spacer"></span>' +
            '    <span class="stat-line tool-stat">已解决 <b>' + solvedAll + '</b> / ' + BANK.length + ' 题</span>' +
            '  </div>' +
            '  <div class="table-card lc-list"><div id="list-body"></div></div>' +
            '  <p class="src-note">题面为学习改写版，仅用于本地离线练习。</p>' +
            '</div>';

        /* ---- 左侧栏：进度卡 + 每日一题卡 ---- */
        var sideProgress = $('#side-progress', wrap);
        sideProgress.innerHTML =
            '<div class="sp-head">做题进度</div>' +
            '<div class="sp-all"><b>' + solvedAll + '</b> / ' + BANK.length + ' <span>已解决</span></div>';
        DIFF_ORDER.forEach(function (d) {
            var done = stat[d][0], total = stat[d][1];
            var pct = total ? Math.round(done / total * 100) : 0;
            sideProgress.appendChild(h('div', 'sp-row',
                '<span class="sp-name diff-' + d + '">' + DIFF_LABEL[d] + '</span>' +
                '<span class="sp-track"><i class="sp-fill sp-fill-' + d + '" style="width:' + pct + '%"></i></span>' +
                '<span class="sp-num">' + done + '/' + total + '</span>'));
        });

        var dq = dailyPick();
        var dp = STORE.getProgressOf(dq.id);
        var sideDaily = $('#side-daily', wrap);
        sideDaily.innerHTML =
            '<div class="sp-head">' + ic('calendar', 14) + ' 每日一题 <span class="sp-date">' +
            (new Date().getMonth() + 1) + '月' + new Date().getDate() + '日</span></div>';
        var drow = h('button', 'lc-row daily-row');
        drow.type = 'button';
        drow.innerHTML =
            statusIcon(dp) +
            '<span class="lc-no">' + esc(dq.code) + '.</span>' +
            '<span class="lc-title">' + esc(dq.title) + '</span>' +
            '<span class="diff diff-' + dq.difficulty + ' diff-plain">' + DIFF_LABEL[dq.difficulty] + '</span>';
        drow.addEventListener('click', function () { location.hash = '#/problem/' + dq.id; });
        sideDaily.appendChild(drow);

        /* ---- 分类胶囊（难度单选） ---- */
        var pillRow = $('#pill-row', wrap);
        var pillDefs = [
            { d: 'all', label: '全部题目' },
            { d: 'easy', label: '简单' },
            { d: 'medium', label: '中等' },
            { d: 'hard', label: '困难' }
        ];
        function renderPills() {
            pillRow.innerHTML = '';
            pillDefs.forEach(function (pd) {
                var b = h('button', 'pill' + (filters.difficulty === pd.d ? ' pill-dark' : ''), esc(pd.label));
                b.type = 'button';
                b.addEventListener('click', function () {
                    filters.difficulty = pd.d;
                    renderPills();
                    renderRows();
                });
                pillRow.appendChild(b);
            });
        }
        renderPills();

        /* ---- 工具行：搜索 + 状态/标签/排序下拉 + 收藏筛选 ---- */
        var toolbar = $('.toolbar', wrap);
        var ddStatus = createDropdown({
            value: filters.status,
            options: [
                { value: 'all', label: '全部状态' },
                { value: 'none', label: '未开始' },
                { value: 'attempted', label: '尝试过' },
                { value: 'solved', label: '已解决' }
            ],
            onChange: function (v) { filters.status = v; renderRows(); }
        });
        var ddTag = createDropdown({
            value: filters.tag,
            options: [{ value: 'all', label: '全部标签' }].concat(allTags().map(function (t) { return { value: t, label: t }; })),
            onChange: function (v) { filters.tag = v; renderRows(); }
        });
        var ddSort = createDropdown({
            value: filters.sort,
            options: [
                { value: 'default', label: '默认排序' },
                { value: 'diff-asc', label: '难度：简单 → 困难' },
                { value: 'diff-desc', label: '难度：困难 → 简单' }
            ],
            onChange: function (v) { filters.sort = v; renderRows(); }
        });
        var favBtn = h('button', 'icon-btn fav-filter' + (filters.fav ? ' on' : ''), ic('star', 16));
        favBtn.type = 'button';
        favBtn.title = '只看收藏';
        favBtn.addEventListener('click', function () {
            filters.fav = !filters.fav;
            favBtn.classList.toggle('on', filters.fav);
            renderRows();
        });
        var ddSlot = $('#f-dd-slot', wrap);
        ddSlot.appendChild(ddStatus);
        ddSlot.appendChild(ddTag);
        ddSlot.appendChild(ddSort);
        toolbar.insertBefore(favBtn, $('.spacer', toolbar));

        var search = $('#f-search', wrap);
        search.value = filters.search;
        search.addEventListener('input', function () {
            clearTimeout(searchTimer);
            searchTimer = setTimeout(function () {
                filters.search = search.value;
                renderRows();
            }, 200);
        });

        function renderRows() {
            var body = $('#list-body', wrap);
            var list = filteredList();
            body.innerHTML = '';
            if (!list.length) {
                body.appendChild(h('div', 'empty', filters.fav && !getFavs().length
                    ? '还没有收藏的题目：把鼠标移到题目行上，点亮行尾的星标即可收藏'
                    : '没有符合条件的题目'));
                return;
            }
            list.forEach(function (q) {
                var p = STORE.getProgressOf(q.id);
                var fav = isFav(q.id);
                var row = h('button', 'lc-row');
                row.type = 'button';
                row.innerHTML =
                    statusIcon(p) +
                    '<span class="lc-no">' + esc(q.code) + '.</span>' +
                    '<span class="lc-title">' + esc(q.title) + '</span>' +
                    '<span class="lc-score">' + q.score + ' 分</span>' +
                    '<span class="diff diff-' + q.difficulty + ' diff-plain">' + DIFF_LABEL[q.difficulty] + '</span>' +
                    diffBars(q.difficulty) +
                    '<span class="lc-star' + (fav ? ' on' : '') + '" title="' + (fav ? '取消收藏' : '收藏') + '">' +
                    '<svg width="16" height="16" viewBox="0 0 24 24" fill="' + (fav ? 'currentColor' : 'none') + '" stroke="currentColor" stroke-width="1.8" stroke-linejoin="round">' + ICON_PATHS.star + '</svg></span>';
                row.addEventListener('click', function (ev) {
                    if (ev.target.closest('.lc-star')) {
                        var on = toggleFav(q.id);
                        renderRows();
                        toast(on ? '已收藏「' + q.code + ' ' + q.title + '」' : '已取消收藏', 'info');
                        return;
                    }
                    location.hash = '#/problem/' + q.id;
                });
                body.appendChild(row);
            });
        }
        renderRows();
        return wrap;
    }

    /* ============================================================
     * 编辑器（CodeMirror，加载失败降级为 textarea）
     * ============================================================ */
    function makeEditor(container, value, mode, onChange) {
        if (window.CodeMirror) {
            var cm = window.CodeMirror(container, {
                value: value,
                mode: mode,
                lineNumbers: true,
                indentUnit: 4,
                smartIndent: true,
                indentWithTabs: false,
                theme: cmTheme(),
                extraKeys: { 'Ctrl-Enter': function () { /* 由详情页绑定 */ } }
            });
            applyEditorSettings(cm);
            cm.on('change', onChange);
            cm._kind = 'cm';
            return cm;
        }
        var ta = document.createElement('textarea');
        ta.className = 'editor-fallback';
        ta.value = value;
        ta.spellcheck = false;
        var timer = null;
        ta.addEventListener('input', function () {
            clearTimeout(timer);
            timer = setTimeout(onChange, 400);
        });
        container.appendChild(ta);
        ta._kind = 'ta';
        return ta;
    }
    function editorGetValue(ed) { return ed._kind === 'cm' ? ed.getValue() : ed.value; }
    function editorSetValue(ed, v) {
        if (ed._kind === 'cm') { ed.setValue(v); ed.refresh(); } else { ed.value = v; }
    }
    function editorSetMode(ed, mode) {
        if (ed._kind === 'cm' && ed.setOption) ed.setOption('mode', mode);
    }

    /* ============================================================
     * 详情页
     * ============================================================ */
    var runToken = 0;

    function renderProblem(id) {
        var q = getQuestion(id);
        if (!q) { toast('题目不存在', 'warn'); location.hash = '#/problems'; return h('div'); }

        var myToken = ++runToken;
        var lang = STORE.get('lang', 'c');
        if (lang !== 'c' && lang !== 'cpp' && lang !== 'python') lang = 'c';
        /* 草稿自动保存（与力扣一致）：重开题目 / 切换语言都恢复上次写到一半的代码；
         * 没有草稿时显示初始模板，历史版本在「提交记录」面板里，重置按钮可回到模板 */
        function draftOf(l) {
            var d = STORE.loadCode(q.id, l);
            return (d === null || d === undefined) ? (q.langTemplates[l] || '') : d;
        }
        var initialCode = draftOf(lang);

        var wrap = h('div', 'page problem-page');
        wrap.innerHTML =
            '<div class="split">' +
            '  <section class="left-col" id="left-col"></section>' +
            '  <div class="divider" id="divider"></div>' +
            '  <section class="right-col" id="right-col"></section>' +
            '</div>';

        /* ---- 左栏顶部：题库 / 上一题 / 下一题 / 随机一题（力扣式翻题条） ---- */
        var left = $('#left-col', wrap);
        var bar = h('div', 'plc-bar');
        bar.innerHTML =
            '<button id="goto-list" class="plc-btn" title="打开题库列表">' +
            '<svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><path d="M4 6h16M4 12h16M4 18h16"/></svg>' +
            '<span>题库</span></button>' +
            '<span class="toolbar-sep"></span>' +
            '<button id="goto-prev" class="plc-btn" title="上一题">' +
            '<svg width="17" height="17" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M15 18l-6-6 6-6"/></svg></button>' +
            '<button id="goto-next" class="plc-btn" title="下一题">' +
            '<svg width="17" height="17" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M9 18l6-6-6-6"/></svg></button>' +
            '<button id="goto-random" class="plc-btn" title="随机一题">' +
            '<svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M16 3h5v5M4 20L21 3M21 16v5h-5M15 15l6 6M4 4l5 5"/></svg></button>';
        left.appendChild(bar);

        /* ---- 左栏三个标签页：题目描述 / 题解 / 提交记录 ---- */
        var tabRow = h('div', 'ltabs');
        tabRow.innerHTML =
            '<button class="ltab active" data-pane="desc">' +
            '<svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"/><path d="M14 2v6h6M16 13H8M16 17H8M10 9H8"/></svg>' +
            '<span>题目描述</span></button>' +
            '<button class="ltab" data-pane="sol">' +
            '<svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M2 3h6a4 4 0 0 1 4 4v14a3 3 0 0 0-3-3H2z"/><path d="M22 3h-6a4 4 0 0 0-4 4v14a3 3 0 0 1 3-3h7z"/></svg>' +
            '<span>题解</span></button>' +
            '<button class="ltab" data-pane="hist">' +
            '<svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"/><path d="M12 6v6l4 2"/></svg>' +
            '<span id="hist-tab-label">提交记录（0）</span></button>' +
            '<span class="ltabs-end"><button id="left-collapse" class="icon-btn" title="收起题目面板">' +
            '<svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M15 18l-6-6 6-6"/></svg></button></span>';
        left.appendChild(tabRow);

        var descPane = h('div', 'lpane');
        var solPane = h('div', 'lpane hidden');
        var histPane = h('div', 'lpane hidden');
        left.appendChild(descPane);
        left.appendChild(solPane);
        left.appendChild(histPane);

        /* ---- 题目描述标签页 ---- */
        var head = h('div', 'q-head');
        head.innerHTML =
            '<div class="q-title-row"><span class="q-code">' + esc(q.code) + '</span>' +
            '<h2 class="q-title">' + esc(q.title) + '</h2>' + diffBadge(q.difficulty) +
            '<span class="q-meta">' + q.score + ' 分 · 原题作者 ' + esc(q.author || '—') + ' · 单用例限时 ' + Math.round((q.timeLimit || 3000) / 1000) + ' 秒</span></div>';
        var p = STORE.getProgressOf(q.id);
        if (p && p.status) {
            head.appendChild(h('div', 'q-status-line', '当前状态：' + statusIcon(p) + ' ' + statusText(p) + '（已尝试 ' + (p.attempts || 0) + ' 次）'));
        }
        descPane.appendChild(head);
        descPane.appendChild(mdNode(q.description, 'q-desc'));

        var exBox = h('div', 'examples');
        exBox.appendChild(h('h3', 'sec-title', '示例'));
        (q.examples || []).forEach(function (e, i) {
            var card = h('div', 'example-card');
            card.appendChild(h('div', 'example-label', '示例 ' + (i + 1)));
            card.appendChild(h('div', 'io-label', '输入'));
            card.appendChild(h('pre', 'io-pre', esc(e.input)));
            card.appendChild(h('div', 'io-label', '输出'));
            card.appendChild(h('pre', 'io-pre', esc(e.output)));
            if (e.explain) {
                card.appendChild(h('div', 'io-label', '解释'));
                card.appendChild(h('div', 'io-explain', esc(e.explain)));
            }
            exBox.appendChild(card);
        });
        descPane.appendChild(exBox);

        if ((q.constraints || []).length) {
            var cBox = h('div', 'constraints');
            cBox.appendChild(h('h3', 'sec-title', '约束条件'));
            var ul = h('ul', 'constraint-list');
            q.constraints.forEach(function (c) { ul.appendChild(h('li', '', esc(c))); });
            cBox.appendChild(ul);
            descPane.appendChild(cBox);
        }

        if ((q.hints || []).length) {
            var hintBox = h('details', 'fold');
            hintBox.appendChild(h('summary', '', '提示'));
            var hul = h('ul', 'hint-list');
            q.hints.forEach(function (x) { hul.appendChild(h('li', '', esc(x))); });
            hintBox.appendChild(hul);
            descPane.appendChild(hintBox);
        }

        if ((q.tags || []).length) {
            var tagBox = h('div', 'q-tags');
            q.tags.forEach(function (t) { tagBox.appendChild(h('span', 'tag', esc(t))); });
            descPane.appendChild(tagBox);
        }

        /* ---- 题解标签页：参考思路 + 三份参考代码（CodeMirror 只读高亮） ---- */
        if (q.solution) {
            var solBox = h('div', 'fold');
            solBox.appendChild(h('h3', 'sec-title', '参考思路'));
            solBox.appendChild(h('div', 'sol-text', esc(q.solution)));
            solPane.appendChild(solBox);
        }

        var refSources = [
            ['C', 'c', q.referenceSolution],
            ['C++', 'cpp', ((window.REFERENCE_EXTRA || {})[q.code] || {}).cpp],
            ['Python', 'python', ((window.REFERENCE_EXTRA || {})[q.code] || {}).python]
        ];
        var solSec = h('div', 'sol-section');
        solSec.appendChild(h('h3', 'sec-title', '参考代码'));
        var solHint = h('div', 'sol-hint', '建议先自己完成并通过后，再来对照参考代码；载入会覆盖编辑器当前内容。点击标题可展开 / 收起。');
        solSec.appendChild(solHint);
        refSources.forEach(function (item) {
            if (!item[2]) return;
            var label = item[0], roLang = item[1], code = item[2];
            var block = h('div', 'ref-block');
            var headBtn = h('button', 'ref-block-head',
                '<span class="ref-block-title">' +
                '<span class="ref-lang-tag ref-lang-' + roLang + '">' + label + '</span>' +
                '参考代码（' + label + '）</span>' +
                '<svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M18 15l-6-6-6 6"/></svg>');
            var body = h('div', 'ref-block-body hidden');
            var loadBtn = h('button', 'btn btn-mini', '把这份代码载入到编辑器');
            loadBtn.addEventListener('click', function () {
                confirmDialog('确定把参考代码（' + label + '）载入编辑器吗？当前编辑器内容会被覆盖。').then(function (yes) {
                    if (!yes) return;
                    editorSetValue(editor, code);
                    toast('已载入参考代码（' + label + '），可直接运行或提交', 'ok');
                });
            });
            body.appendChild(loadBtn);
            var host = h('div', 'ro-host');
            body.appendChild(host);
            var roCm = null;
            headBtn.addEventListener('click', function () {
                var open = block.classList.toggle('open');
                body.classList.toggle('hidden', !open);
                if (open) {
                    if (!roCm) roCm = makeReadOnly(host, code, langMode(roLang));
                    else setTimeout(function () { try { roCm.refresh(); } catch (e) { } }, 30);
                }
            });
            block.appendChild(headBtn);
            block.appendChild(body);
            solSec.appendChild(block);
        });
        solPane.appendChild(solSec);

        /* ---- 标签页切换 ---- */
        var ltabBtns = tabRow.querySelectorAll('.ltab');
        var panes = { desc: descPane, sol: solPane, hist: histPane };
        ltabBtns.forEach(function (b) {
            b.addEventListener('click', function () {
                ltabBtns.forEach(function (x) { x.classList.remove('active'); });
                b.classList.add('active');
                Object.keys(panes).forEach(function (k) {
                    panes[k].classList.toggle('hidden', b.dataset.pane !== k);
                });
            });
        });

        /* ---- 翻题条行为 ---- */
        $('#goto-list', bar).addEventListener('click', function () { openProblemDrawer(); });
        var btnPrev = $('#goto-prev', bar);
        var btnNext = $('#goto-next', bar);
        if (q.id <= 1) btnPrev.disabled = true;
        if (q.id >= BANK.length) btnNext.disabled = true;
        btnPrev.addEventListener('click', function () { if (q.id > 1) location.hash = '#/problem/' + (q.id - 1); });
        btnNext.addEventListener('click', function () { if (q.id < BANK.length) location.hash = '#/problem/' + (q.id + 1); });
        $('#goto-random', bar).addEventListener('click', function () {
            var r = q.id;
            while (r === q.id) r = BANK[Math.floor(Math.random() * BANK.length)].id;
            location.hash = '#/problem/' + r;
        });

        /* ---- 右栏：本地判题提示条 + 编辑器窗格 + 测试结果窗格 ---- */
        var right = $('#right-col', wrap);
        if (!STORE.get('env-bar-dismissed', false)) {
            var envBar = h('div', 'env-bar');
            envBar.innerHTML =
                '<span class="env-bar-ic">ⓘ</span>' +
                '<span>判题在<b>本地浏览器</b>完成：C / C++ / Python 三种引擎已全部内置，完全离线可用。</span>';
            var envClose = h('button', 'icon-btn env-close', ic('x', 14));
            envClose.title = '不再显示';
            envClose.addEventListener('click', function () {
                STORE.set('env-bar-dismissed', true);
                envBar.remove();
            });
            envBar.appendChild(envClose);
            right.appendChild(envBar);
        }

        var toolbar = h('div', 'editor-toolbar');
        toolbar.innerHTML =
            '<span id="lang-slot"></span>' +
            '<span class="toolbar-sep"></span>' +
            '<button id="btn-reset" class="icon-btn" title="重置为初始模板">' +
            '<svg width="15" height="15" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M3 12a9 9 0 1 0 3-6.7L3 8"/><path d="M3 3v5h5"/></svg>' +
            '</button>' +
            '<span class="spacer"></span>' +
            '<span id="judge-status" class="judge-status"></span>' +
            '<button id="btn-run" class="btn btn-run">' +
            '<svg width="13" height="13" viewBox="0 0 24 24" fill="currentColor"><path d="M6 4l14 8-14 8V4z"/></svg>运行</button>' +
            '<button id="btn-submit" class="btn btn-submit">' +
            '<svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M12 13v8M8 17l4 4 4-4"/><path d="M20.88 18.09A5 5 0 0 0 18 9h-1.26A8 8 0 1 0 3 16.29"/></svg>提交</button>';
        right.appendChild(toolbar);

        var langDD = createDropdown({
            value: lang, className: 'lang-dd',
            options: [
                { value: 'c', label: 'C', dot: 'langc' },
                { value: 'cpp', label: 'C++', dot: 'langcpp' },
                { value: 'python', label: 'Python', dot: 'langpy' }
            ],
            onChange: function (v) { setLang(v); }
        });
        var langSlot = $('#lang-slot', toolbar);
        langSlot.parentNode.replaceChild(langDD, langSlot);

        var editorPane = h('div', 'editor-pane');
        var editorHead = h('div', 'editor-head');
        editorHead.innerHTML =
            '<span class="editor-head-title">' + ic('code', 15) + '代码</span>' +
            '<span class="spacer"></span>' +
            '<button id="ed-fs" class="icon-btn" title="编辑器全屏">' + ic('maximize', 15) + '</button>' +
            '<button id="ed-collapse" class="icon-btn" title="收起编辑器">' + ic('chevronUp', 15) + '</button>';
        var editorHost = h('div', 'editor-host');
        var editorStatus = h('div', 'editor-status');
        editorStatus.innerHTML =
            '<span id="ed-save" class="ed-save">—</span>' +
            '<span class="spacer"></span>' +
            '<span id="ed-cursor">行 1, 列 1</span>';
        editorPane.appendChild(editorHead);
        editorPane.appendChild(editorHost);
        editorPane.appendChild(editorStatus);
        right.appendChild(editorPane);

        var vDivider = h('div', 'v-divider', '');
        right.appendChild(vDivider);

        /* ---- 结果面板：测试用例 / 测试结果 / 控制台 三个 Tab（仿力扣） ---- */
        var resultPanel = h('div', 'result-panel');
        resultPanel.innerHTML =
            '<div class="result-tabs">' +
            '<button class="rtab active" data-tab="cases"><span class="rtab-ic">' + ic('checkSquare', 15) + '</span>测试用例</button>' +
            '<button class="rtab" data-tab="result"><span class="rtab-ic">' + ic('zap', 15) + '</span>测试结果</button>' +
            '<button class="rtab" data-tab="console"><span class="rtab-ic">' + ic('terminal', 14) + '</span>控制台</button>' +
            '<button id="console-toggle" class="icon-btn" title="收起/展开结果面板">' +
            '<svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M18 15l-6-6-6 6"/></svg>' +
            '</button>' +
            '</div>' +
            '<div id="tab-cases" class="tab-body tab-cases"></div>' +
            '<div id="tab-result" class="tab-body tab-result hidden"></div>' +
            '<div id="tab-console" class="tab-body tab-console hidden"><pre id="console-out" class="console-out"></pre></div>';
        right.appendChild(resultPanel);

        var tabCases = $('#tab-cases', resultPanel);
        var tabResult = $('#tab-result', resultPanel);

        /* 编辑器：草稿自动保存（每次运行/提交的代码另存进「提交记录」历史） */
        var editor = makeEditor(editorHost, initialCode,
            lang === 'python' ? 'text/x-python' : 'text/x-csrc',
            function () { scheduleDraftSave(); });
        setTimeout(function () { if (editor._kind === 'cm') editor.refresh(); }, 50);

        var saveTimer = null;
        function nowTime() {
            return new Date().toLocaleTimeString('zh-CN', { hour: '2-digit', minute: '2-digit' });
        }
        function setSaveHint(text) {
            var el = $('#ed-save', editorStatus);
            if (el) el.textContent = text;
        }
        function scheduleDraftSave() {
            clearTimeout(saveTimer);
            saveTimer = setTimeout(function () {
                STORE.saveCode(q.id, lang, editorGetValue(editor));
                setSaveHint('已存储 ' + nowTime());
            }, 500);
        }
        function flushDraftSave() {
            clearTimeout(saveTimer);
            STORE.saveCode(q.id, lang, editorGetValue(editor));
            setSaveHint('已存储 ' + nowTime());
        }
        function updateCursor() {
            var el = $('#ed-cursor', editorStatus);
            if (!el) return;
            if (editor._kind === 'cm') {
                var c = editor.getCursor();
                el.textContent = '行 ' + (c.line + 1) + ', 列 ' + (c.ch + 1);
            } else {
                var pos = editor.selectionStart || 0;
                var before = editor.value.slice(0, pos).split('\n');
                el.textContent = '行 ' + before.length + ', 列 ' + (before[before.length - 1].length + 1);
            }
        }
        if (editor._kind === 'cm') {
            editor.on('cursorActivity', updateCursor);
        } else {
            editor.addEventListener('input', updateCursor);
            editor.addEventListener('click', updateCursor);
        }
        updateCursor();
        /* 首次打开就存一份模板草稿，保证「已存储」状态真实 */
        STORE.saveCode(q.id, lang, initialCode);
        setSaveHint('已存储 ' + nowTime());

        function setLang(l) {
            if (l === lang) return;
            /* 切语言前先把当前内容存回原语言的草稿，再载入目标语言草稿 */
            STORE.saveCode(q.id, lang, editorGetValue(editor));
            lang = l;
            STORE.set('lang', l);
            editorSetMode(editor, langMode(l));
            editorSetValue(editor, draftOf(l));
            flushDraftSave();
            updateCursor();
        }

        $('#btn-reset', toolbar).addEventListener('click', function () {
            confirmDialog('确定要把代码恢复为本题初始模板吗？当前编辑器内容会被覆盖。').then(function (yes) {
                if (!yes) return;
                STORE.resetCode(q.id, lang);
                editorSetValue(editor, q.langTemplates[lang] || '');
                STORE.saveCode(q.id, lang, editorGetValue(editor));
                setSaveHint('已重置为模板');
                toast('代码已重置为模板');
            });
        });

        /* Tab 切换 */
        var tabs = resultPanel.querySelectorAll('.rtab');
        function switchTab(name) {
            tabs.forEach(function (x) { x.classList.toggle('active', x.dataset.tab === name); });
            ['cases', 'result', 'console'].forEach(function (n) {
                var el = $('#tab-' + n, resultPanel);
                if (el) el.classList.toggle('hidden', n !== name);
            });
        }
        tabs.forEach(function (b) {
            b.addEventListener('click', function () { switchTab(b.dataset.tab); });
        });

        var consoleLines = [];
        function logConsole(line) {
            consoleLines.push(line);
            var text = consoleLines.join('\n');
            if (text.length > 2000) text = text.slice(0, 2000) + '\n…（输出过长，已截断）';
            $('#console-out', resultPanel).textContent = text;
        }

        function setStatus(text, spin) {
            var s = $('#judge-status', toolbar);
            s.innerHTML = spin ? '<span class="spinner"></span>' + esc(text) : esc(text);
        }
        function setBusy(busy, which) {
            $('#btn-run', toolbar).disabled = busy;
            $('#btn-submit', toolbar).disabled = busy;
            if (busy && which) setStatus((which === 'run' ? '运行中…' : '判题中…'), true);
            if (!busy) setStatus('', false);
        }

        /* ==========================================================
         * 「测试用例」Tab：Case 1 / Case 2 / + 新增（仿力扣）
         * 每个 Case 一个 stdin 输入块；示例用例可改可删，+ 加自定义
         * ========================================================== */
        var editCases = [];
        var editActive = 0;
        (q.sampleCases || []).forEach(function (c) {
            editCases.push({ input: c.input, expected: c.expected, custom: false });
        });
        if (!editCases.length) editCases.push({ input: '', expected: '', custom: true });

        function flushActiveEdit() {
            var ta = $('.ce-input', tabCases);
            if (ta && editCases[editActive]) editCases[editActive].input = ta.value;
        }
        function renderEditCases() {
            tabCases.innerHTML = '';
            var bar = h('div', 'case-tabs');
            editCases.forEach(function (c, i) {
                var t = h('button', 'case-tab' + (i === editActive ? ' active' : '') + (c.custom ? ' custom' : ''));
                t.type = 'button';
                t.innerHTML = '<span class="case-tab-name">Case ' + (i + 1) + '</span>' +
                    '<span class="case-tab-x" title="删除该用例">' + ic('x', 12) + '</span>';
                t.addEventListener('click', function (ev) {
                    if (ev.target.closest('.case-tab-x')) {
                        flushActiveEdit();
                        editCases.splice(i, 1);
                        if (!editCases.length) editCases.push({ input: '', expected: '', custom: true });
                        if (editActive >= editCases.length) editActive = editCases.length - 1;
                        renderEditCases();
                        return;
                    }
                    flushActiveEdit();
                    editActive = i;
                    renderEditCases();
                });
                bar.appendChild(t);
            });
            var add = h('button', 'case-add');
            add.type = 'button';
            add.innerHTML = ic('plus', 15) + '<span>添加用例</span>';
            add.addEventListener('click', function () {
                flushActiveEdit();
                editCases.push({ input: '', expected: '', custom: true });
                editActive = editCases.length - 1;
                renderEditCases();
            });
            bar.appendChild(add);
            tabCases.appendChild(bar);

            var body = h('div', 'case-edit');
            var c = editCases[editActive] || { input: '', custom: true };
            body.appendChild(h('div', 'ce-label',
                (c.custom ? '自定义用例 · ' : '示例用例 · ') + '标准输入（stdin）'));
            var block = h('div', 'ce-block');
            var ta = document.createElement('textarea');
            ta.className = 'ce-input';
            ta.value = c.input || '';
            ta.placeholder = '按题目输入格式填写，可多行…';
            ta.spellcheck = false;
            block.appendChild(ta);
            body.appendChild(block);
            /* 输入块随内容自动增高（仿力扣），最高 320px 后内部滚动 */
            var autoGrow = function () {
                ta.style.height = 'auto';
                ta.style.height = Math.min(ta.scrollHeight, 320) + 'px';
            };
            ta.addEventListener('input', autoGrow);
            setTimeout(autoGrow, 0);
            var cp = h('button', 'rc-copy ce-copy', ic('copy', 15));
            cp.title = '复制输入';
            cp.addEventListener('click', function () { copyText(ta.value); });
            block.appendChild(cp);
            body.appendChild(h('div', 'ce-hint',
                c.custom
                    ? '自定义用例在「运行」时附加执行，只显示实际输出、不判对错；留空则忽略。'
                    : '可直接修改示例输入来试验；标签右上角 × 删除，+ 添加自定义用例。'));
            tabCases.appendChild(body);
        }
        renderEditCases();

        /* ==========================================================
         * 「测试结果」Tab：通过/解答错误总览 + Case 胶囊 + 输入/输出/预期
         * ========================================================== */
        var resCases = [];
        var resActive = 0;
        var diffOn = false;
        var resTotalMs = 0;

        function casePass(rc) {
            if (rc.status !== 'ok') return false;
            if (rc.custom) return true;
            return JUDGE.compareOutput(rc.output, rc.expected);
        }
        function caseIcon(rc) {
            if (rc.status === 'running') return '<span class="spinner spinner-sm"></span>';
            if (rc.status === 'pending') return '<span class="rc-dot"></span>';
            if (rc.status === 'ok') {
                if (rc.custom || JUDGE.compareOutput(rc.output, rc.expected))
                    return '<span class="rc-ic pass">' + ic('check', 12) + '</span>';
                return '<span class="rc-ic fail">' + ic('x', 12) + '</span>';
            }
            if (rc.status === 'tle') return '<span class="rc-ic tle">' + ic('clock', 12) + '</span>';
            return '<span class="rc-ic fail">' + ic('x', 12) + '</span>';
        }

        function addResultSec(container, label, textOrHtml, opts) {
            opts = opts || {};
            container.appendChild(h('div', 'rc-label', label));
            var block = h('div', 'rc-block' + (opts.empty ? ' is-empty' : ''));
            if (opts.copy) {
                var cp = h('button', 'rc-copy', ic('copy', 15));
                cp.title = '复制';
                cp.addEventListener('click', function () { copyText(opts.copyText || ''); });
                block.appendChild(cp);
            }
            var pre = h('pre', 'rc-pre');
            if (opts.html) pre.innerHTML = textOrHtml;
            else pre.textContent = textOrHtml;
            block.appendChild(pre);
            container.appendChild(block);
        }

        function renderResultBody(container) {
            var c = resCases[resActive];
            if (!c) return;
            container.innerHTML = '';

            /* 输入（带复制按钮） */
            addResultSec(container, '输入', c.input || '', { copy: true, copyText: c.input || '', empty: !c.input });

            /* 输出 */
            if (c.status === 'pending') {
                addResultSec(container, '输出', '排队中…', { empty: true });
            } else if (c.status === 'running') {
                addResultSec(container, '输出', '运行中…', { empty: true });
            } else if (c.status === 'ok' || c.status === 'tle' || c.status === 'runtime-error') {
                var outHtml;
                if (diffOn && !c.custom && c.status === 'ok' && c.output !== c.expected) {
                    outHtml = diffActualHtml(c.output, c.expected);
                } else {
                    outHtml = esc(c.output === '' ? '（空）' : c.output);
                }
                addResultSec(container, '输出', outHtml, { html: true, empty: c.output === '' });
            } else {
                addResultSec(container, '输出', '（无输出）', { empty: true });
            }

            /* 错误信息 */
            if (c.error) {
                container.appendChild(h('div', 'rc-error', esc(c.error)));
            }

            /* 预期结果（自定义用例不判对错，不显示） */
            if (!c.custom && c.expected) {
                var expHtml;
                if (diffOn && c.status === 'ok' && c.output !== c.expected) {
                    expHtml = diffExpectedHtml(c.output, c.expected);
                } else {
                    expHtml = esc(c.expected);
                }
                addResultSec(container, '预期结果', expHtml, { html: true });
            }

            if (c.ms) container.appendChild(h('div', 'rc-ms', '本用例用时 ' + c.ms + ' ms'));
        }

        function renderResult() {
            tabResult.innerHTML = '';
            if (!resCases.length) {
                tabResult.appendChild(h('div', 'result-empty',
                    '还没有运行结果。点击「运行」或「提交」后，结果会显示在这里。'));
                return;
            }
            var done = resCases.every(function (c) { return c.status !== 'pending' && c.status !== 'running'; });
            var running = !done;
            var passCount = 0;
            resCases.forEach(function (c) { if (casePass(c)) passCount++; });

            /* 顶部总览：通过（绿）/ 解答错误（红）+ 执行用时 + Diff */
            var bar = h('div', 'rs-bar');
            var titleCls = 'fail', titleText = '解答错误';
            if (running) { titleCls = 'run'; titleText = '运行中'; }
            else if (passCount === resCases.length) { titleCls = 'pass'; titleText = '通过'; }
            else {
                var worst = 'wrong';
                resCases.forEach(function (c) {
                    if (c.custom || casePass(c)) return;
                    if (c.status === 'compile-error') worst = 'compile';
                    else if (c.status === 'load-failed' && worst !== 'compile') worst = 'env';
                    else if (c.status === 'runtime-error' && worst !== 'compile' && worst !== 'env') worst = 'runtime';
                    else if (c.status === 'tle' && worst === 'wrong') worst = 'tle';
                });
                titleText = { compile: '编译错误', env: '环境不可用', runtime: '运行错误', tle: '超出时间限制', wrong: '解答错误' }[worst];
                titleCls = worst === 'tle' ? 'warn' : 'fail';
            }
            bar.innerHTML =
                '<span class="rs-title ' + titleCls + '">' +
                (running ? '<span class="spinner"></span>' : '') + esc(titleText) + '</span>' +
                '<span class="rs-ms">执行用时: ' + (done ? resTotalMs : '—') + ' ms</span>';
            var diffBtn = h('button', 'rs-diff' + (diffOn ? ' on' : ''), 'Diff');
            diffBtn.type = 'button';
            diffBtn.title = '高亮预期与实际输出的差异';
            diffBtn.addEventListener('click', function () {
                diffOn = !diffOn;
                diffBtn.classList.toggle('on', diffOn);
                renderResultBody($('.rc-body', tabResult) || rcBody);
            });
            bar.appendChild(diffBtn);
            tabResult.appendChild(bar);

            /* Case 胶囊标签 */
            var pills = h('div', 'rc-tabs');
            resCases.forEach(function (c, i) {
                var t = h('button', 'rc-tab' + (i === resActive ? ' active' : ''));
                t.type = 'button';
                t.innerHTML = caseIcon(c) +
                    '<span>Case ' + (i + 1) + '</span>' +
                    (c.custom ? '<span class="rc-tag">自定义</span>' : '');
                t.addEventListener('click', function () { resActive = i; renderResult(); });
                pills.appendChild(t);
            });
            tabResult.appendChild(pills);

            var rcBody = h('div', 'rc-body');
            tabResult.appendChild(rcBody);
            renderResultBody(rcBody);
        }

        function runCases(cases, mode) {
            flushDraftSave();
            consoleLines = [];
            $('#console-out', resultPanel).textContent = '';
            var codeAtStart = editorGetValue(editor);
            var startedAt = Date.now();

            resCases = cases.map(function (c) {
                return {
                    input: c.input, expected: c.expected || '', custom: !!c.custom,
                    status: 'pending', output: '', error: '', ms: 0
                };
            });
            resActive = 0;
            diffOn = false;
            resTotalMs = 0;

            /* 展开结果面板并切到「测试结果」Tab（仿力扣） */
            resultPanel.classList.remove('collapsed');
            applyVertical();
            if (editor._kind === 'cm') editor.refresh();
            switchTab('result');
            renderResult();
            setBusy(true, mode === 'submit' ? 'submit' : 'run');

            var passCount = 0;
            var runResults = [];
            var chain = Promise.resolve();
            cases.forEach(function (c, i) {
                chain = chain.then(function () {
                    if (myToken !== runToken) return null;   // 页面已切换，放弃
                    resCases[i].status = 'running';
                    resActive = i;
                    renderResult();
                    if (lang === 'python' && !JUDGE.isReady('python')) {
                        setStatus('首次使用 Python：正在加载运行时（约 10MB），请稍候…', true);
                    }
                    var t0 = Date.now();
                    return JUDGE.run(lang, codeAtStart, c.input, q.timeLimit).then(function (r) {
                        if (myToken !== runToken) return null;
                        r.ms = r.ms || (Date.now() - t0);
                        r.input = c.input;
                        logConsole('—— ' + q.code + ' 用例输出 ——\n' + (r.output || '') + (r.error ? '\n[错误] ' + r.error : ''));
                        var rc = resCases[i];
                        rc.status = r.status;
                        rc.output = r.output || '';
                        rc.error = r.error || '';
                        rc.ms = r.ms;
                        if (casePass(rc)) passCount++;
                        runResults.push({
                            status: r.status,
                            error: r.error || '',
                            ms: r.ms || 0,
                            input: c.input,
                            expected: c.expected || '',
                            actual: r.output || '',
                            custom: !!c.custom
                        });
                        if (mode === 'submit') {
                            setStatus('通过 ' + passCount + ' / ' + cases.length + ' 个用例', true);
                        }
                        renderResult();
                        return null;
                    });
                });
            });

            return chain.then(function () {
                if (myToken !== runToken) return null;
                setBusy(false);
                resTotalMs = Date.now() - startedAt;
                /* 自动定位到第一个未通过的用例（力扣行为） */
                var firstBad = -1;
                for (var k = 0; k < resCases.length; k++) {
                    if (!casePass(resCases[k])) { firstBad = k; break; }
                }
                resActive = firstBad >= 0 ? firstBad : 0;
                renderResult();

                if (mode === 'submit') {
                    var all = passCount === cases.length;
                    STORE.markProgress(q.id, all ? 'solved' : 'attempted');
                    if (all) toast('恭喜，本题通过！', 'ok');
                } else {
                    toast('运行完成', 'info');
                }

                /* 写入历史：跳过"编辑器没有代码"的空跑 */
                var noCode = runResults.length > 0 && runResults.every(function (r) {
                    return r.status === 'compile-error' && /还没有有效代码/.test(r.error);
                });
                if (!noCode) {
                    var worst = 'ok';
                    var rank = { ok: 0, 'runtime-error': 2, 'compile-error': 3, tle: 4, 'load-failed': 5 };
                    runResults.forEach(function (r) {
                        if (r.custom) return;
                        if ((rank[r.status] || 0) > (rank[worst] || 0)) worst = r.status;
                    });
                    var allPassed = runResults.filter(function (r) { return !r.custom; })
                        .every(function (r) { return r.status === 'ok'; }) && runResults.some(function (r) { return !r.custom; });
                    var verdict, vclass;
                    if (worst === 'ok') { verdict = '通过'; vclass = 'v-pass'; }
                    else if (worst === 'tle') { verdict = '超出时间限制'; vclass = 'v-tle'; }
                    else if (worst === 'compile-error') { verdict = '编译/解析错误'; vclass = 'v-fail'; }
                    else if (worst === 'load-failed') { verdict = '环境不可用'; vclass = 'v-fail'; }
                    else { verdict = '运行时错误'; vclass = 'v-fail'; }
                    if (mode === 'submit') verdict += ' ' + passCount + '/' + cases.length;
                    else if (!allPassed) verdict = verdict === '通过' ? '通过（示例）' : verdict + '（示例）';
                    STORE.addHistory(q.id, {
                        ts: Date.now(),
                        lang: lang,
                        mode: mode,
                        verdict: verdict,
                        vclass: vclass,
                        pass: passCount,
                        total: cases.length,
                        ms: resTotalMs,
                        code: codeAtStart,
                        cases: runResults.map(function (r) {
                            return {
                                s: r.custom ? '自定义' : (STATUS_TEXT[r.status] || r.status),
                                m: r.ms,
                                i: trunc(r.input, 200),
                                e: trunc(r.expected, 200),
                                a: trunc(r.actual, 300)
                            };
                        })
                    });
                    renderHistoryTab();
                }
                return null;
            });
        }

        $('#btn-run', toolbar).addEventListener('click', function () {
            flushActiveEdit();
            var cases = [];
            editCases.forEach(function (c) {
                if (!c.input || !c.input.trim()) return;
                cases.push({ input: c.input, expected: c.expected, custom: c.custom });
            });
            if (!cases.length) { toast('请至少填写一个测试用例的输入', 'warn'); return; }
            runCases(cases, 'run');
        });

        $('#btn-submit', toolbar).addEventListener('click', function () {
            var cases = [];
            (q.sampleCases || []).forEach(function (c) { cases.push({ input: c.input, expected: c.expected }); });
            (q.hiddenCases || []).forEach(function (c) { cases.push({ input: c.input, expected: c.expected }); });
            runCases(cases, 'submit');
        });

        /* Ctrl+Enter 运行 */
        if (editor._kind === 'cm') {
            editor.setOption('extraKeys', {
                'Ctrl-Enter': function () { $('#btn-run', toolbar).click(); },
                'Cmd-Enter': function () { $('#btn-run', toolbar).click(); }
            });
        } else {
            editor.addEventListener('keydown', function (ev) {
                if ((ev.ctrlKey || ev.metaKey) && ev.key === 'Enter') { ev.preventDefault(); $('#btn-run', toolbar).click(); }
            });
        }

        /* ---------- 提交记录（历史运行/提交，代码展开时高亮） ---------- */
        function renderHistoryTab() {
            var box = histPane;
            if (!box) return;
            var list = STORE.getHistory(q.id);
            var tabBtn = $('#hist-tab-label', tabRow);
            if (tabBtn) tabBtn.textContent = '提交记录（' + list.length + '）';
            box.innerHTML = '';
            if (!list.length) {
                box.appendChild(h('div', 'empty', '还没有运行或提交记录；每次「运行 / 提交」都会自动把当时的代码和结果存入这里'));
                return;
            }
            var top = h('div', 'hist-top');
            var clearBtn = h('button', 'btn btn-mini', '清空本题记录');
            clearBtn.addEventListener('click', function () {
                confirmDialog('确定清空本题的全部历史记录吗？').then(function (yes) {
                    if (!yes) return;
                    STORE.clearHistory(q.id);
                    renderHistoryTab();
                });
            });
            top.appendChild(clearBtn);
            box.appendChild(top);
            list.forEach(function (rec) {
                var item = h('div', 'hist-item');
                var head = h('div', 'hist-head');
                head.innerHTML =
                    '<span class="hist-mode">' + (rec.mode === 'submit' ? '提交' : '运行') + '</span>' +
                    '<span class="case-verdict ' + (rec.vclass || 'v-fail') + '">' + esc(rec.verdict || '') + '</span>' +
                    '<span class="hist-meta">' + esc(rec.lang === 'cpp' ? 'C++' : rec.lang) +
                    ' · ' + (rec.pass !== undefined ? rec.pass + '/' + rec.total + ' · ' : '') +
                    (rec.ms || 0) + 'ms · ' + new Date(rec.ts).toLocaleString('zh-CN') + '</span>';
                item.appendChild(head);
                var body = h('div', 'hist-body hidden');
                body.appendChild(h('div', 'io-label', '当时的代码'));
                var pre = h('pre', 'io-pre hist-code');
                pre.textContent = rec.code || '';
                body.appendChild(pre);
                if (rec.cases && rec.cases.length) {
                    body.appendChild(h('div', 'io-label', '用例结果'));
                    var cl = h('div', 'hist-cases');
                    rec.cases.forEach(function (c) {
                        cl.appendChild(h('div', 'hist-case',
                            '<span class="' + (c.s === '通过' || c.s === '完成' ? 'v-pass' : (c.s === '超出时间限制' ? 'v-tle' : 'v-fail')) + '">' +
                            esc(c.s) + '</span> <span class="hist-meta">' + c.m + 'ms</span>'));
                    });
                    body.appendChild(cl);
                }
                var loadBtn = h('button', 'btn btn-mini', '把这份代码载入到编辑器');
                loadBtn.addEventListener('click', function () {
                    confirmDialog('确定把这条历史代码载入编辑器吗？当前编辑器内容会被覆盖。').then(function (yes) {
                        if (!yes) return;
                        editorSetValue(editor, rec.code || '');
                        toast('已载入历史代码', 'ok');
                    });
                });
                body.appendChild(loadBtn);
                item.appendChild(body);
                head.addEventListener('click', function () {
                    var nowHidden = body.classList.toggle('hidden');
                    /* 首次展开时把纯文本代码替换为 CodeMirror 高亮 */
                    if (!nowHidden && !body._cmDone) {
                        body._cmDone = true;
                        if (window.CodeMirror && pre.parentNode) {
                            var code = pre.textContent;
                            var host = h('div', 'ro-host hist-ro');
                            body.insertBefore(host, pre);
                            pre.remove();
                            makeReadOnly(host, code, langMode(rec.lang));
                        }
                    }
                });
                box.appendChild(item);
            });
        }
        renderHistoryTab();

        /* ==========================================================
         * 布局：拖拽调宽调高 + 收起/全屏 + 状态持久化（刷新后保持）
         * ========================================================== */
        var splitEl = $('.split', wrap);
        var layout = getLayout();
        var lastVPct = (typeof layout.vpct === 'number' ? Math.min(80, Math.max(20, layout.vpct)) : 60);
        var lastLPct = (typeof layout.lpct === 'number' ? Math.min(75, Math.max(20, layout.lpct)) : 45);
        var edCollapsed = !!layout.edCollapsed;
        var leftCollapsed = !!layout.leftCollapsed;
        var edFs = false;

        /* 左栏收起后显示的竖条恢复按钮 */
        var leftRestore = h('button', 'left-restore', '›');
        leftRestore.type = 'button';
        leftRestore.title = '展开题目面板';
        wrap.appendChild(leftRestore);

        function applyHLayout() {
            $('#left-col', wrap).style.width = lastLPct + '%';
            $('#right-col', wrap).style.width = (100 - lastLPct) + '%';
        }
        function applyVertical() {
            vDivider.style.display = edCollapsed ? 'none' : '';
            if (edCollapsed) {
                editorPane.style.flex = '0 0 auto';
            } else if (resultPanel.classList.contains('collapsed')) {
                editorPane.style.flex = '1 1 auto';
            } else {
                editorPane.style.flex = '0 0 ' + lastVPct + '%';
            }
        }
        function applyEdCollapse() {
            editorPane.classList.toggle('collapsed', edCollapsed);
            right.classList.toggle('ed-collapsed', edCollapsed);
            $('#ed-collapse', editorHead).title = edCollapsed ? '展开编辑器' : '收起编辑器';
            applyVertical();
            if (editor._kind === 'cm') editor.refresh();
        }
        function applyLeftCollapse() {
            splitEl.classList.toggle('left-collapsed', leftCollapsed);
            if (!leftCollapsed) applyHLayout();
            if (editor._kind === 'cm') editor.refresh();
        }
        function applyEdFs() {
            wrap.classList.toggle('ed-fs', edFs);
            $('#ed-fs', editorHead).title = edFs ? '退出全屏（Esc）' : '编辑器全屏';
            if (editor._kind === 'cm') editor.refresh();
        }

        applyHLayout();
        applyVertical();
        applyEdCollapse();
        applyLeftCollapse();

        /* 左右栏拖拽调宽 */
        var divider = $('#divider', wrap);
        var dragging = false;
        divider.addEventListener('mousedown', function (ev) { ev.preventDefault(); dragging = true; document.body.classList.add('dragging'); });
        window.addEventListener('mousemove', function (ev) {
            if (!dragging) return;
            var rect = splitEl.getBoundingClientRect();
            lastLPct = Math.min(75, Math.max(20, ((ev.clientX - rect.left) / rect.width) * 100));
            applyHLayout();
        });
        window.addEventListener('mouseup', function () {
            if (!dragging) return;
            dragging = false;
            document.body.classList.remove('dragging');
            saveLayout({ lpct: lastLPct });
            if (editor._kind === 'cm') editor.refresh();
        });

        /* 编辑器 / 测试结果 拖拽调高：以编辑器窗格实际几何为基准并保持鼠标相对偏移，
         * 分隔条始终跟手（旧实现以上下两窗格外的整栏高度直接折算，一点击就瞬移） */
        var vDragging = false;
        var vDragOffset = 0;
        vDivider.addEventListener('mousedown', function (ev) {
            ev.preventDefault();
            vDragging = true;
            document.body.classList.add('dragging-v');
            var rc = right.getBoundingClientRect();
            lastVPct = Math.min(80, Math.max(20, (editorPane.getBoundingClientRect().height / rc.height) * 100));
            var dRect = vDivider.getBoundingClientRect();
            vDragOffset = ev.clientY - (dRect.top + dRect.height / 2);
        });
        window.addEventListener('mousemove', function (ev) {
            if (!vDragging) return;
            var rc = right.getBoundingClientRect();
            var above = editorPane.offsetTop;   /* 编辑器窗格上方的固定占用（提示条 + 工具行） */
            var want = ev.clientY - vDragOffset - rc.top - above;
            lastVPct = Math.min(80, Math.max(20, (want / rc.height) * 100));
            applyVertical();
        });
        window.addEventListener('mouseup', function () {
            if (!vDragging) return;
            vDragging = false;
            document.body.classList.remove('dragging-v');
            saveLayout({ vpct: lastVPct });
            if (editor._kind === 'cm') editor.refresh();
        });

        /* 结果面板收起 / 展开 */
        $('#console-toggle', resultPanel).addEventListener('click', function () {
            resultPanel.classList.toggle('collapsed');
            applyVertical();
            if (editor._kind === 'cm') editor.refresh();
        });

        /* 编辑器收起 / 展开 */
        $('#ed-collapse', editorHead).addEventListener('click', function () {
            edCollapsed = !edCollapsed;
            saveLayout({ edCollapsed: edCollapsed });
            applyEdCollapse();
        });

        /* 编辑器全屏（Esc 退出） */
        function toggleEdFs() {
            edFs = !edFs;
            applyEdFs();
        }
        $('#ed-fs', editorHead).addEventListener('click', toggleEdFs);

        /* 左栏收起 / 展开 */
        $('#left-collapse', tabRow).addEventListener('click', function () {
            leftCollapsed = true;
            saveLayout({ leftCollapsed: leftCollapsed });
            applyLeftCollapse();
        });
        leftRestore.addEventListener('click', function () {
            leftCollapsed = false;
            saveLayout({ leftCollapsed: leftCollapsed });
            applyLeftCollapse();
        });

        /* 全局快捷键：Ctrl+Enter 运行（不限焦点）、Esc 退出全屏并收起下拉 */
        document.addEventListener('keydown', function onDocKey(ev) {
            if (myToken !== runToken) {   // 已切到别的题目/页面，注销自己
                document.removeEventListener('keydown', onDocKey);
                return;
            }
            if ((ev.ctrlKey || ev.metaKey) && ev.key === 'Enter') {
                ev.preventDefault();
                var b = $('#btn-run', toolbar);
                if (b && !b.disabled) b.click();
            } else if (ev.key === 'Escape') {
                if (edFs) toggleEdFs();
                else closeAllDropdowns();
            }
        });

        return wrap;
    }

    /* ============================================================
     * 进度页
     * ============================================================ */
    function renderProgress() {
        var stat = progressStats();
        var wrap = h('div', 'page');
        var solvedAll = stat.easy[0] + stat.medium[0] + stat.hard[0];
        wrap.innerHTML = '<div class="page-head"><h1>进度</h1>' +
            '<div class="stat-line">已解决 <b>' + solvedAll + '</b> / ' + BANK.length + ' 题</div></div>' +
            '<div class="stat-cards" id="stat-cards"></div>' +
            '<h3 class="sec-title">最近做题记录</h3>' +
            '<div class="table-card"><table class="table"><thead><tr><th>题目</th><th class="col-diff">难度</th><th>状态</th><th>尝试次数</th><th>最后尝试时间</th></tr></thead>' +
            '<tbody id="recent-body"></tbody></table></div>';

        var cards = $('#stat-cards', wrap);
        DIFF_ORDER.forEach(function (d) {
            cards.appendChild(h('div', 'stat-card diff-card-' + d,
                '<div class="stat-num">' + stat[d][0] + ' / ' + stat[d][1] + '</div>' +
                '<div class="stat-name">' + DIFF_LABEL[d] + '</div>'));
        });

        var recents = [];
        BANK.forEach(function (q) {
            var p = STORE.getProgressOf(q.id);
            if (p && p.lastAttempt) recents.push({ q: q, p: p });
        });
        recents.sort(function (a, b) { return b.p.lastAttempt - a.p.lastAttempt; });
        var body = $('#recent-body', wrap);
        if (!recents.length) {
            body.innerHTML = '<tr><td colspan="5" class="empty">还没有做题记录，去题库开始第一题吧</td></tr>';
        } else {
            recents.forEach(function (r) {
                var tr = document.createElement('tr');
                tr.innerHTML =
                    '<td><a class="title-link" href="#/problem/' + r.q.id + '">' + esc(r.q.code) + ' ' + esc(r.q.title) + '</a></td>' +
                    '<td class="col-diff">' + diffBadge(r.q.difficulty) + '</td>' +
                    '<td>' + statusIcon(r.p) + ' ' + statusText(r.p) + '</td>' +
                    '<td>' + (r.p.attempts || 0) + '</td>' +
                    '<td>' + new Date(r.p.lastAttempt).toLocaleString('zh-CN') + '</td>';
                body.appendChild(tr);
            });
        }
        return wrap;
    }

    /* ---------- 设置弹窗（编辑器字号 / Tab 宽度 / 自动换行） ---------- */
    function applyAllEditors() {
        document.querySelectorAll('.CodeMirror').forEach(function (el) {
            if (el.CodeMirror) applyEditorSettings(el.CodeMirror);
        });
    }
    function openSettingsModal() {
        var s = getSettings();
        var overlay = h('div', 'overlay');
        var panel = h('div', 'dialog settings-dialog');
        panel.innerHTML =
            '<div class="settings-title">' + ic('gear', 16) + '设置</div>' +
            '<div class="settings-note">设置保存在本机浏览器，对所有题目生效。</div>';

        function makeRow(label, control) {
            var row = h('div', 'set-row');
            row.appendChild(h('span', 'set-label', esc(label)));
            row.appendChild(control);
            return row;
        }
        var ddFont = createDropdown({
            value: String(s.fontSize),
            options: ['12', '13', '13.5', '15', '17'].map(function (v) { return { value: v, label: v + ' px' }; }),
            onChange: function (v) { s.fontSize = parseFloat(v); saveSettings(); applyAllEditors(); }
        });
        var ddTab = createDropdown({
            value: String(s.tab),
            options: [{ value: '2', label: '2 个空格' }, { value: '4', label: '4 个空格' }],
            onChange: function (v) { s.tab = parseInt(v, 10); saveSettings(); applyAllEditors(); }
        });
        var wrapSwitch = h('button', 'switch' + (s.wrap ? ' on' : ''));
        wrapSwitch.type = 'button';
        wrapSwitch.title = '过长代码行是否自动折行';
        wrapSwitch.addEventListener('click', function () {
            s.wrap = !s.wrap;
            wrapSwitch.classList.toggle('on', s.wrap);
            saveSettings();
            applyAllEditors();
        });
        panel.appendChild(makeRow('代码字体大小', ddFont));
        panel.appendChild(makeRow('Tab 缩进宽度', ddTab));
        panel.appendChild(makeRow('自动换行', wrapSwitch));

        var rowBtns = h('div', 'dialog-row');
        var done = h('button', 'btn btn-primary', '完成');
        done.type = 'button';
        done.addEventListener('click', function () { overlay.remove(); });
        rowBtns.appendChild(done);
        panel.appendChild(rowBtns);

        overlay.appendChild(panel);
        document.body.appendChild(overlay);
        overlay.addEventListener('click', function (ev) { if (ev.target === overlay) overlay.remove(); });
        document.addEventListener('keydown', function onKey(ev) {
            if (ev.key === 'Escape') {
                overlay.remove();
                document.removeEventListener('keydown', onKey);
            }
        });
    }

    /* ============================================================
     * 主题与路由
     * ============================================================ */
    function applyTheme(t) {
        document.body.dataset.theme = t;
        STORE.setTheme(t);
        var btn = $('#theme-btn');
        if (btn) {
            btn.textContent = t === 'dark' ? '☀️' : '🌙';
            btn.title = t === 'dark' ? '切换到浅色主题' : '切换到深色主题';
        }
    }

    function route() {
        var hash = location.hash || '#/problems';
        var app = $('#app');
        runToken++;          // 中断进行中的判题渲染
        var isProblem = hash.indexOf('#/problem/') === 0;
        var page;
        if (isProblem) {
            page = renderProblem(parseInt(hash.slice('#/problem/'.length), 10) || 0);
        } else if (hash.indexOf('#/progress') === 0) {
            page = renderProgress();
        } else {
            page = renderList();
        }
        app.innerHTML = '';
        app.appendChild(page);
        var navs = document.querySelectorAll('.nav a');
        navs.forEach(function (a) {
            var target = a.getAttribute('href');
            var active = (hash.indexOf('#/problem/') === 0 && target === '#/problems') || hash === target;
            a.classList.toggle('active', active);
        });
        window.scrollTo(0, 0);
    }

    function init() {
        applyTheme(STORE.getTheme() === 'dark' ? 'dark' : 'light');
        $('#theme-btn').addEventListener('click', function () {
            applyTheme(document.body.dataset.theme === 'dark' ? 'light' : 'dark');
            route();   // 重建页面以同步编辑器主题等
        });
        var gear = $('#settings-btn');
        if (gear) gear.addEventListener('click', openSettingsModal);
        document.addEventListener('store-error', function (ev) {
            toast(ev.detail || '存储异常', 'warn');
        });
        /* 点击下拉组件外部时收起所有下拉 */
        document.addEventListener('mousedown', function (ev) {
            if (!ev.target.closest || !ev.target.closest('.dd')) closeAllDropdowns();
        });
        /* 导航滚动阴影 + 回到顶部 */
        window.addEventListener('scroll', function () {
            var tb = $('#topbar');
            if (tb) tb.classList.toggle('scrolled', window.scrollY > 4);
            var bt = document.getElementById('back-top');
            if (bt) bt.classList.toggle('show', window.scrollY > 400);
        }, { passive: true });
        var backTop = document.getElementById('back-top');
        if (backTop) backTop.addEventListener('click', function () {
            window.scrollTo({ top: 0, behavior: 'smooth' });
        });
        window.addEventListener('hashchange', route);
        route();
    }

    if (document.readyState === 'loading') {
        document.addEventListener('DOMContentLoaded', init);
    } else {
        init();
    }
})();
