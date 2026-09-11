/* 本地静态服务器（仅开发/自测用）：node serve.js 后访问 http://localhost:8123 */
const http = require("http");
const fs = require("fs");
const path = require("path");

const root = path.join(__dirname, "..");
const port = 8123;
const types = {
    ".html": "text/html; charset=utf-8",
    ".css": "text/css; charset=utf-8",
    ".js": "text/javascript; charset=utf-8",
    ".md": "text/plain; charset=utf-8"
};

http.createServer(function (req, res) {
    let p = decodeURIComponent((req.url || "/").split("?")[0]);
    if (p === "/") p = "/index.html";
    const file = path.normalize(path.join(root, p));
    if (!file.startsWith(root)) { res.writeHead(403); return res.end(); }
    fs.readFile(file, function (err, data) {
        if (err) { res.writeHead(404); return res.end("not found"); }
        res.writeHead(200, { "Content-Type": types[path.extname(file)] || "application/octet-stream" });
        res.end(data);
    });
}).listen(port, function () {
    console.log("serving " + root + " at http://localhost:" + port);
});
