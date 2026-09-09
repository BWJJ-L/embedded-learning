"use strict";
/* JSCPP 引擎限制基线/回归试验台
 * 用法：node jscpp-limits.js [node_modules路径]
 * 每项 = 一个 C/C++ 片段 + 输入 + 期望输出；打印 PASS/FAIL 明细。
 * 修复引擎后重跑本脚本，应全部 PASS。
 */
const path = require("path");
const fs = require("fs");

/* 优先用补丁版引擎（tests/engine-src），没有则回退 node_modules（未修补） */
let JSCPP;
const patched = path.join(__dirname, "engine-src", "commonjs.js");
if (fs.existsSync(patched)) {
    JSCPP = require(patched);
    console.log("（使用补丁版引擎 engine-src）");
} else {
    JSCPP = require("JSCPP");
    console.log("（使用 node_modules 原始引擎，未修补）");
}

function runLang(lang, code, input) {
    let out = "";
    let err = null;
    try {
        JSCPP.run(code, input, {
            stdio: {
                drain() { return input == null ? "" : input; },
                write(s) { out += s; }
            }
        });
    } catch (e) {
        err = String((e && e.message) || e);
    }
    return { out, err };
}

const cases = [
    // ---------- scanf 系列 ----------
    { id: "scanf %d", lang: "c", code: '#include <stdio.h>\nint main(){int a;scanf("%d",&a);printf("%d",a);return 0;}', input: "42", want: "42" },
    { id: "scanf %f float", lang: "c", code: '#include <stdio.h>\nint main(){float a;scanf("%f",&a);printf("%.2f",a);return 0;}', input: "3.5", want: "3.50" },
    { id: "scanf %lf double", lang: "c", code: '#include <stdio.h>\nint main(){double a;scanf("%lf",&a);printf("%.2f",a);return 0;}', input: "3.5", want: "3.50" },
    { id: "scanf %lf 负数", lang: "c", code: '#include <stdio.h>\nint main(){double a;scanf("%lf",&a);printf("%.2f",a);return 0;}', input: "-12.25", want: "-12.25" },
    { id: "scanf %ld long", lang: "c", code: '#include <stdio.h>\nint main(){long a;scanf("%ld",&a);printf("%ld",a);return 0;}', input: "123456", want: "123456" },
    { id: "scanf %lld long long", lang: "c", code: '#include <stdio.h>\nint main(){long long a;scanf("%lld",&a);printf("%lld",a);return 0;}', input: "123456789", want: "123456789" },
    { id: "scanf %c", lang: "c", code: '#include <stdio.h>\nint main(){char c;scanf("%c",&c);printf("%c",c);return 0;}', input: "K", want: "K" },
    { id: "scanf 空格+%c", lang: "c", code: '#include <stdio.h>\nint main(){char c;scanf(" %c",&c);printf("%c",c);return 0;}', input: "  K", want: "K" },
    { id: "scanf %s", lang: "c", code: '#include <stdio.h>\nint main(){char s[20];scanf("%s",s);printf("%s",s);return 0;}', input: "hello", want: "hello" },
    { id: "scanf %d,%d 逗号分隔", lang: "c", code: '#include <stdio.h>\nint main(){int a,b;scanf("%d,%d",&a,&b);printf("%d %d",a,b);return 0;}', input: "1,23", want: "1 23" },
    { id: "scanf 混合读 %d%lf", lang: "c", code: '#include <stdio.h>\nint main(){int a;double b;scanf("%d%lf",&a,&b);printf("%d %.1f",a,b);return 0;}', input: "3 2.5", want: "3 2.5" },
    { id: "cin >> int", lang: "cpp", code: '#include <iostream>\nusing namespace std;\nint main(){int a;cin>>a;cout<<a;return 0;}', input: "42", want: "42" },
    { id: "cin >> double 负数", lang: "cpp", code: '#include <iostream>\nusing namespace std;\nint main(){double a;cin>>a;cout<<a;return 0;}', input: "-5.5", want: "-5.5" },
    { id: "cin >> double 整数输入", lang: "cpp", code: '#include <iostream>\nusing namespace std;\nint main(){double a;cin>>a;cout<<a;return 0;}', input: "-5", want: "-5" },
    // ---------- 负数除法 / 取模 ----------
    { id: "负数除法 -7/2", lang: "c", code: '#include <stdio.h>\nint main(){printf("%d",-7/2);return 0;}', input: "", want: "-3" },
    { id: "负数除法 7/-2", lang: "c", code: '#include <stdio.h>\nint main(){printf("%d",7/-2);return 0;}', input: "", want: "-3" },
    { id: "负数取模 -7%2", lang: "c", code: '#include <stdio.h>\nint main(){printf("%d",-7%2);return 0;}', input: "", want: "-1" },
    { id: "float转int截断", lang: "c", code: '#include <stdio.h>\nint main(){printf("%d",(int)-5.9);return 0;}', input: "", want: "-5" },
    // ---------- printf 系列 ----------
    { id: "printf 逗号空格", lang: "c", code: '#include <stdio.h>\nint main(){printf("%.1f, %.1f",1.5,2.5);return 0;}', input: "", want: "1.5, 2.5" },
    { id: "printf 括号空格", lang: "c", code: '#include <stdio.h>\nint main(){printf("(%d, %d)",1,2);return 0;}', input: "", want: "(1, 2)" },
    { id: "printf %s 指针偏移", lang: "c", code: '#include <stdio.h>\n#include <string.h>\nint main(){char s[10]="hello";char*p=s+2;printf("%s",p);return 0;}', input: "", want: "llo" },
    { id: "printf %c", lang: "c", code: '#include <stdio.h>\nint main(){printf("%c%c",\'A\',66);return 0;}', input: "", want: "AB" },
    { id: "printf %5.2f", lang: "c", code: '#include <stdio.h>\nint main(){printf("%5.2f",3.14159);return 0;}', input: "", want: " 3.14" },
    // ---------- 混用 iostream + cstdio ----------
    { id: "iostream+cstdio 混用", lang: "cpp", code: '#include <iostream>\n#include <cstdio>\nusing namespace std;\nint main(){int a;scanf("%d",&a);cout<<"ok "<<a;return 0;}', input: "7", want: "ok 7" },
    { id: "混用连续读 scanf→cin", lang: "cpp", code: '#include <iostream>\n#include <cstdio>\nusing namespace std;\nint main(){int a,b;scanf("%d",&a);cin>>b;printf("%d %d",a,b);return 0;}', input: "3 7", want: "3 7" },
    { id: "混用连续读 cin→scanf", lang: "cpp", code: '#include <iostream>\n#include <cstdio>\nusing namespace std;\nint main(){int a,b;cin>>a;scanf("%d",&b);printf("%d %d",a,b);return 0;}', input: "3 7", want: "3 7" },
    { id: "混用 scanf→cin→scanf", lang: "cpp", code: '#include <iostream>\n#include <cstdio>\nusing namespace std;\nint main(){int a,b,c;scanf("%d",&a);cin>>b;scanf("%d",&c);printf("%d %d %d",a,b,c);return 0;}', input: "1 2 3", want: "1 2 3" },
    { id: "混用 getchar→cin", lang: "cpp", code: '#include <iostream>\n#include <cstdio>\nusing namespace std;\nint main(){int c=getchar();int x;cin>>x;printf("%c %d",(char)c,x);return 0;}', input: "A 42\n", want: "A 42" },
    // ---------- string.h ----------
    { id: "strlen", lang: "c", code: '#include <stdio.h>\n#include <string.h>\nint main(){printf("%d",(int)strlen("hello"));return 0;}', input: "", want: "5" },
    { id: "strlen 变量", lang: "c", code: '#include <stdio.h>\n#include <string.h>\nint main(){char s[20]="abcd";printf("%d",(int)strlen(s));return 0;}', input: "", want: "4" },
    { id: "strcpy", lang: "c", code: '#include <stdio.h>\n#include <string.h>\nint main(){char a[20];strcpy(a,"hi");printf("%s",a);return 0;}', input: "", want: "hi" },
    { id: "strcat", lang: "c", code: '#include <stdio.h>\n#include <string.h>\nint main(){char a[20]="foo";strcat(a,"bar");printf("%s",a);return 0;}', input: "", want: "foobar" },
    { id: "strcmp", lang: "c", code: '#include <stdio.h>\n#include <string.h>\nint main(){printf("%d %d",strcmp("abc","abc")==0,strcmp("abc","abd")<0);return 0;}', input: "", want: "1 1" },
    { id: "strchr", lang: "c", code: '#include <stdio.h>\n#include <string.h>\nint main(){char s[]="hello";char*p=strchr(s,\'l\');printf("%s",(char*)p);return 0;}', input: "", want: "llo" },
    { id: "strstr", lang: "c", code: '#include <stdio.h>\n#include <string.h>\nint main(){char s[]="hello world";char*p=strstr(s,"wor");printf("%s",(char*)p);return 0;}', input: "", want: "world" },
    { id: "strncpy", lang: "c", code: '#include <stdio.h>\n#include <string.h>\nint main(){char a[10];strncpy(a,"abc",5);printf("%s",a);return 0;}', input: "", want: "abc" },
    { id: "strncat", lang: "c", code: '#include <stdio.h>\n#include <string.h>\nint main(){char a[20]="foo";strncat(a,"barxx",3);printf("%s",a);return 0;}', input: "", want: "foobar" },
    { id: "strrchr", lang: "c", code: '#include <stdio.h>\n#include <string.h>\nint main(){char s[]="aXbXc";char*p=strrchr(s,\'X\');printf("%s",(char*)p);return 0;}', input: "", want: "Xc" },
    { id: "memset", lang: "c", code: '#include <stdio.h>\n#include <string.h>\nint main(){char a[10];memset(a,\'x\',5);a[5]=0;printf("%s",a);return 0;}', input: "", want: "xxxxx" },
    { id: "memcpy", lang: "c", code: '#include <stdio.h>\n#include <string.h>\nint main(){char a[10]="abc";char b[10];memcpy(b,a,4);printf("%s",b);return 0;}', input: "", want: "abc" },
    { id: "strtok", lang: "c", code: '#include <stdio.h>\n#include <string.h>\nint main(){char s[]="a,b,,c";char*p=strtok(s,",");while(p){printf("[%s]",p);p=strtok(NULL,",");}return 0;}', input: "", want: "[a][b][c]" },
    { id: "sprintf", lang: "c", code: '#include <stdio.h>\nint main(){char b[20];sprintf(b,"%d-%s",5,"x");printf("%s",b);return 0;}', input: "", want: "5-x" },
    // ---------- struct ----------
    { id: "struct 基础", lang: "c", code: '#include <stdio.h>\nstruct P{int x;int y;};\nint main(){struct P p;p.x=3;p.y=4;printf("%d %d",p.x,p.y);return 0;}', input: "", want: "3 4" },
    { id: "struct 数组", lang: "c", code: '#include <stdio.h>\nstruct P{int x;int y;};\nint main(){struct P a[2];a[0].x=1;a[1].x=2;printf("%d%d",a[0].x,a[1].x);return 0;}', input: "", want: "12" },
    // ---------- 未初始化 ----------
    { id: "未初始化局部变量读0", lang: "c", code: '#include <stdio.h>\nint main(){int a;printf("%d",a);return 0;}', input: "", want: "0" },
    { id: "未初始化数组元素读0", lang: "c", code: '#include <stdio.h>\nint main(){int a[3];a[1]=5;printf("%d%d%d",a[0],a[1],a[2]);return 0;}', input: "", want: "050" },
    // ---------- 控制组（原本就能用） ----------
    { id: "getchar/putchar", lang: "c", code: '#include <stdio.h>\nint main(){int c=getchar();putchar(c);return 0;}', input: "Z\n", want: "Z" },
    { id: "gets/puts", lang: "c", code: '#include <stdio.h>\nint main(){char s[20];gets(s);puts(s);return 0;}', input: "hi there\n", want: "hi there\n" },
    { id: "cout<<double", lang: "cpp", code: '#include <iostream>\nusing namespace std;\nint main(){cout<<2.5;return 0;}', input: "", want: "2.5" }
];

let pass = 0, fail = 0;
const lines = [];
for (const c of cases) {
    const r = runLang(c.lang, c.code, c.input);
    const got = r.err ? ("ERR: " + r.err.slice(0, 90)) : r.out;
    const ok = !r.err && r.out === c.want;
    if (ok) pass++; else fail++;
    lines.push((ok ? "PASS " : "FAIL ") + c.id + (ok ? "" : ("  期望[" + c.want + "] 实际[" + got + "]")));
}
console.log(lines.join("\n"));
console.log("\n通过 " + pass + " / " + (pass + fail));
process.exit(0);
