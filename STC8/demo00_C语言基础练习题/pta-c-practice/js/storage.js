/* ============================================================
 * 本地存储 storage.js
 * - 所有 key 统一加 ctv1: 前缀
 * - JSON 解析全部 try/catch 兜底
 * - localStorage 不可用（隐私模式等）时派发 store-error 事件，不阻断刷题
 * ============================================================ */
(function () {
    'use strict';

    var PREFIX = 'ctv1:';

    function emitError(msg) {
        try {
            document.dispatchEvent(new CustomEvent('store-error', { detail: msg }));
        } catch (e) { /* 忽略 */ }
    }

    function getRaw(key) {
        try {
            return localStorage.getItem(PREFIX + key);
        } catch (e) {
            emitError('浏览器存储不可用（隐私模式？），做题进度与代码将无法保存');
            return null;
        }
    }

    function get(key, fallback) {
        var raw = getRaw(key);
        if (raw === null || raw === undefined) return fallback;
        try {
            return JSON.parse(raw);
        } catch (e) {
            return fallback;
        }
    }

    function set(key, value) {
        try {
            localStorage.setItem(PREFIX + key, JSON.stringify(value));
            return true;
        } catch (e) {
            emitError('保存失败：浏览器存储不可用或已满');
            return false;
        }
    }

    /* ---------- 做题进度 ---------- */
    var PROGRESS_KEY = 'progress';

    function getProgress() {
        return get(PROGRESS_KEY, {}) || {};
    }

    function getProgressOf(id) {
        return getProgress()[String(id)] || null;
    }

    /* status: 'solved' | 'attempted' */
    function markProgress(id, status) {
        var all = getProgress();
        var key = String(id);
        var cur = all[key] || { status: null, attempts: 0, lastAttempt: 0 };
        cur.status = (cur.status === 'solved' || status === 'solved') ? 'solved' : status;
        cur.attempts = (cur.attempts || 0) + 1;
        cur.lastAttempt = Date.now();
        all[key] = cur;
        set(PROGRESS_KEY, all);
        return cur;
    }

    /* ---------- 代码保存 ---------- */
    function codeKey(id, lang) {
        return 'code:' + id + ':' + lang;
    }

    function saveCode(id, lang, code) {
        set(codeKey(id, lang), code);
    }

    function loadCode(id, lang) {
        return get(codeKey(id, lang), null);
    }

    function resetCode(id, lang) {
        try { localStorage.removeItem(PREFIX + codeKey(id, lang)); } catch (e) { /* 忽略 */ }
    }

    /* ---------- 历史运行/提交记录 ---------- */
    var HISTORY_CAP = 50;

    function getHistory(id) {
        var h = get('history:' + id, []);
        return Array.isArray(h) ? h : [];
    }

    function addHistory(id, record) {
        var h = getHistory(id);
        h.unshift(record);
        if (h.length > HISTORY_CAP) h = h.slice(0, HISTORY_CAP);
        set('history:' + id, h);
        return h;
    }

    function clearHistory(id) {
        try { localStorage.removeItem(PREFIX + 'history:' + id); } catch (e) { /* 忽略 */ }
    }

    /* ---------- 主题 ---------- */
    function getTheme() {
        return get('theme', 'light');
    }

    function setTheme(theme) {
        set('theme', theme);
    }

    window.STORE = {
        get: get,
        set: set,
        getProgress: getProgress,
        getProgressOf: getProgressOf,
        markProgress: markProgress,
        saveCode: saveCode,
        loadCode: loadCode,
        resetCode: resetCode,
        getHistory: getHistory,
        addHistory: addHistory,
        clearHistory: clearHistory,
        getTheme: getTheme,
        setTheme: setTheme
    };
})();
