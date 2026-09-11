/* ============================================================
 * 题库数据 —— 本地代码刷题集（38 题）
 * 题面依据原题改写；判题采用"标准输入 → 标准输出"模式。
 * 参考答案（referenceSolution）为完整 C 程序，已逐题在 JSCPP 解释器中实跑验证。
 * 注意：本站内置的 C 解释器只支持 C 语言的一个常用子集，
 *       参考答案刻意只使用该子集内的写法（详见 README「已知限制」）。
 * ============================================================ */

/* ============================================================
 * 代码模板生成器：从题面提取"输入格式 / 输出格式"段落，
 * 为 C / C++ / Python 生成只含注释（不含任何代码）的初始编辑器内容。
 * ============================================================ */
function stripMd(s) {
    return String(s).replace(/\*\*/g, '').replace(/`/g, '').trim();
}
function extractIO(desc) {
    var lines = String(desc).split('\n');
    var section = null;
    var io = { input: [], output: [] };
    for (var i = 0; i < lines.length; i++) {
        var line = lines[i];
        if (line.indexOf('**输入格式') >= 0) { section = 'input'; continue; }
        if (line.indexOf('**输出格式') >= 0) { section = 'output'; continue; }
        if (line.indexOf('**') === 0) { section = null; continue; }
        if (section && line.trim() !== '') io[section].push(stripMd(line));
    }
    return io;
}
function buildTemplate(lang, io) {
    var cm = (lang === 'python') ? '#' : '//';
    var extra = {
        c: '请写出完整的 C 程序：包含所需头文件，从 int main() 开始，结束时 return 0。',
        cpp: '请写出完整的 C++ 程序：包含所需头文件（如 <iostream> 或 <cstdio>），从 main 开始。',
        python: '请写出完整的 Python 程序：用 input() 读入、print() 输出。'
    }[lang];
    var out = [cm + ' ==================== 本题任务 ===================='];
    io.input.forEach(function (l) { out.push(cm + ' [输入] ' + l); });
    io.output.forEach(function (l) { out.push(cm + ' [输出] ' + l); });
    out.push(cm + '');
    out.push(cm + ' ' + extra);
    out.push(cm + ' ===================================================');
    out.push('');
    return out.join('\n');
}

window.QUESTION_BANK = [

/* ===================== 第 2 周 ===================== */
{
    id: 1,
    code: '1',
    slug: '1',
    title: '整数四则运算',
    difficulty: 'easy',
    score: 10,
    author: '乔林（清华大学）',
    timeLimit: 3000,
    tags: ['模拟', '算术运算'],
    description: [
        '本题要求编写程序，计算 2 个**正整数**的和、差、积、商并输出。题目保证输入和输出全部在整型范围内。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出 2 个正整数 A 和 B，以空格分隔。',
        '',
        '**输出格式：**',
        '',
        '在 4 行中按照 `A 运算符 B = 结果` 的格式顺序输出和、差、积、商。注意除法为整数除法（向零取整）。'
    ].join('\n'),
    examples: [
        { input: '3 2', output: '3 + 2 = 5\n3 - 2 = 1\n3 * 2 = 6\n3 / 2 = 1', explain: '3 加 2 得 5，3 减 2 得 1，3 乘 2 得 6，3 除以 2 的整数商为 1。' }
    ],
    constraints: [
        '1 <= A, B，且 A、B 及四则运算结果均在 int 范围内',
        '输入的两个数均为正整数'
    ],
    hints: [
        '四行输出完全对称，只是运算符和运算结果不同',
        'printf 的格式串里可以同时放普通字符（如空格、=、运算符）和 %d 占位符'
    ],
    solution: '读入两个整数后，连续调用 4 次 printf，格式串分别为 "%d + %d = %d\\n" 等，参数依次是 a、b 和运算结果。',
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int a, b;
    scanf("%d %d", &a, &b);
    printf("%d + %d = %d\n", a, b, a + b);
    printf("%d - %d = %d\n", a, b, a - b);
    printf("%d * %d = %d\n", a, b, a * b);
    printf("%d / %d = %d\n", a, b, a / b);
    return 0;
}
`,
    sampleCases: [
        { input: '3 2\n', expected: '3 + 2 = 5\n3 - 2 = 1\n3 * 2 = 6\n3 / 2 = 1\n' }
    ],
    hiddenCases: [
        { input: '100 7\n', expected: '100 + 7 = 107\n100 - 7 = 93\n100 * 7 = 700\n100 / 7 = 14\n' },
        { input: '6 6\n', expected: '6 + 6 = 12\n6 - 6 = 0\n6 * 6 = 36\n6 / 6 = 1\n' },
        { input: '2 9\n', expected: '2 + 9 = 11\n2 - 9 = -7\n2 * 9 = 18\n2 / 9 = 0\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int a, b;
    scanf("%d %d", &a, &b);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 2,
    code: '2',
    slug: '2',
    title: '厘米换算英尺英寸',
    difficulty: 'easy',
    score: 15,
    author: '乔林（清华大学）',
    timeLimit: 3000,
    tags: ['模拟', '算术运算'],
    description: [
        '如果已知英制长度的英尺 foot 和英寸 inch 的值，那么对应的米制长度是 (foot + inch / 12) × 0.3048 米。',
        '现在，如果用户输入的是厘米数，请你编写程序换算出对应的英制长度——取整数部分：不足 1 英尺、1 英寸的部分直接舍去。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出 1 个正整数，单位为厘米。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出这个厘米数对应英制长度的英尺和英寸的整数值，中间以空格分隔。'
    ].join('\n'),
    examples: [
        { input: '170', output: '5 6', explain: '170 厘米约合 5.58 英尺，取整数部分得 5 英尺；剩余部分约合 6.9 英寸，取整数部分得 6 英寸。' }
    ],
    constraints: [
        '输入为 1 个正整数（厘米数）',
        '换算关系：1 英尺 = 30.48 厘米，1 英尺 = 12 英寸'
    ],
    hints: [
        '先把厘米换算成英尺（除以 30.48），把商的整数部分作为英尺数',
        '用「厘米数 / 30.48 - 英尺数」得到小数部分，再乘 12 转成英寸，赋给整型变量时自动舍去小数'
    ],
    solution: '设 foot = (int)(cm / 30.48)，inch = (int)((cm / 30.48 - foot) × 12)。两次赋值给 int 变量时 C 会自动截断小数部分，注意中间量要用 double 参与运算。',
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int cm, foot, inch;
    scanf("%d", &cm);
    foot = cm / 30.48;
    inch = (cm / 30.48 - foot) * 12;
    printf("%d %d\n", foot, inch);
    return 0;
}
`,
    sampleCases: [
        { input: '170\n', expected: '5 6\n' }
    ],
    hiddenCases: [
        { input: '150\n', expected: '4 11\n' },
        { input: '30\n', expected: '0 11\n' },
        { input: '1\n', expected: '0 0\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int cm, foot, inch;
    scanf("%d", &cm);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 3,
    code: '3',
    slug: '3',
    title: '然后是几点',
    difficulty: 'easy',
    score: 15,
    author: '乔林（清华大学）',
    timeLimit: 3000,
    tags: ['模拟', '算术运算'],
    description: [
        '有时候人们用四位数字表示一个时间，比如 `1106` 表示 11 点零 6 分。现在，你的程序要根据起始时间和流逝的时间计算出终止的时间。',
        '',
        '读入两个数字，第一个数字以这样的四位数字表示当前时间，第二个数字表示分钟数，计算当前时间经过那么多分钟后是几点，结果也表示为四位数字。**当小时为个位数时，没有前导的零，即 5 点 30 分表示为 `530`；当分钟数为个位数时，有前导的零，即 9 点 5 分表示为 `905`。** 题目保证起始时间和终止时间在同一天内。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出 2 个整数，分别是四位数字表示的起始时间、以及流逝的分钟数，其间以空格分隔。**注意：在一天内，流逝的分钟数可能超过 60，也可能是负数。**',
        '',
        '**输出格式：**',
        '',
        '输出四位数字表示的终止时间，即小时没有前导的零、分钟有前导的零（不足两位补 0）。'
    ].join('\n'),
    examples: [
        { input: '1120 110', output: '1310', explain: '11 点 20 分加上 110 分钟是 13 点 10 分。' }
    ],
    constraints: [
        '起始时间为 0~2359 之间的四位表示（小时可是一位数，如 530）',
        '流逝分钟数可为负，结果保证在同一天内'
    ],
    hints: [
        '先把起始时间拆成小时和分钟，全部换算成「分钟总数」再加上流逝时间',
        '输出时小时直接打印，分钟用 %02d 补足两位'
    ],
    solution: 'total = (a/100)×60 + a%100 + b；然后小时 = total/60，分钟 = total%60，按 "%d%02d" 输出即可。',
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int a, b, total;
    scanf("%d %d", &a, &b);
    total = (a / 100) * 60 + a % 100 + b;
    printf("%d%02d\n", total / 60, total % 60);
    return 0;
}
`,
    sampleCases: [
        { input: '1120 110\n', expected: '1310\n' }
    ],
    hiddenCases: [
        { input: '1105 120\n', expected: '1305\n' },
        { input: '903 12\n', expected: '915\n' },
        { input: '1120 -110\n', expected: '930\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int a, b;
    scanf("%d %d", &a, &b);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 4,
    code: '4',
    slug: '4',
    title: '逆序的三位数',
    difficulty: 'easy',
    score: 10,
    author: '乔林（清华大学）',
    timeLimit: 3000,
    tags: ['模拟', '算术运算'],
    description: [
        '程序每次读入一个正 3 位数，然后输出按位逆序的数字。**注意：当输入的数字含结尾的 0 时，逆序输出不应带前导的 0。** 比如输入 `700`，输出应该是 `7`。',
        '',
        '**输入格式：**',
        '',
        '每个测试是一个 3 位的正整数。',
        '',
        '**输出格式：**',
        '',
        '输出按位逆序的数。'
    ].join('\n'),
    examples: [
        { input: '123', output: '321', explain: '百位 1、十位 2、个位 3，逆序后为 321。' }
    ],
    constraints: [
        '输入为 100 ~ 999 之间的正整数'
    ],
    hints: [
        '用取余 % 和整除 / 分别取出个位、十位、百位',
        '重新组合：结果 = 个位×100 + 十位×10 + 百位，前导零自然消失'
    ],
    solution: 'h = n/100、t = n/10%10、o = n%10，输出 o×100 + t×10 + h。数学上不需要任何字符串处理。',
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int n, h, t, o;
    scanf("%d", &n);
    h = n / 100;
    t = n / 10 % 10;
    o = n % 10;
    printf("%d\n", o * 100 + t * 10 + h);
    return 0;
}
`,
    sampleCases: [
        { input: '123\n', expected: '321\n' }
    ],
    hiddenCases: [
        { input: '700\n', expected: '7\n' },
        { input: '250\n', expected: '52\n' },
        { input: '801\n', expected: '108\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 5,
    code: '5',
    slug: '5',
    title: 'BCD解密',
    difficulty: 'easy',
    score: 10,
    author: '乔林（清华大学）',
    timeLimit: 3000,
    tags: ['模拟', '进制'],
    description: [
        'BCD 数是用一个字节来表达两位十进制的数，每四个比特表示一位。所以如果一个 BCD 数的十六进制是 `0x12`，它表达的就是十进制的 `12`。',
        '但是小明没学过 BCD，他所有的 BCD 数都转换成了十进制数输出，于是 BCD 的 `0x12` 被十进制输出成了 `18`！',
        '现在，你的程序要读入这个**错误的十进制数**，然后输出正确的十进制数值。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出一个 [0, 153] 范围内的正整数，保证能转换回有效的 BCD 数，也就是说这个数字转换成十六进制时不会出现 A~F 的十六进制字母。',
        '',
        '**输出格式：**',
        '',
        '输出对应的十进制数。'
    ].join('\n'),
    examples: [
        { input: '18', output: '12', explain: '错误的十进制数 18 写成十六进制是 12，这正是原来 BCD 数表达的正确十进制值。' }
    ],
    constraints: [
        '0 <= 输入 <= 153',
        '输入的十六进制表示只含数字 0~9（保证是合法 BCD）'
    ],
    hints: [
        'n/16 恰好是 BCD 的高位数字，n%16 恰好是低位数字',
        '答案 = (n/16)×10 + n%16'
    ],
    solution: '错误的十进制数 n 其实等于 高位×16 + 低位，所以高位 = n/16、低位 = n%16，拼回十进制输出 (n/16)×10 + (n%16)。',
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);
    printf("%d\n", n % 16 + n / 16 * 10);
    return 0;
}
`,
    sampleCases: [
        { input: '18\n', expected: '12\n' }
    ],
    hiddenCases: [
        { input: '153\n', expected: '99\n' },
        { input: '144\n', expected: '90\n' },
        { input: '1\n', expected: '1\n' },
        { input: '0\n', expected: '0\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 6,
    code: '6',
    slug: '6',
    title: '超速判断',
    difficulty: 'easy',
    score: 10,
    author: '杨起帆（浙江大学城市学院）',
    timeLimit: 3000,
    tags: ['条件判断'],
    description: [
        '模拟交通警察的雷达测速仪。输入汽车速度，如果速度**超出** 60 mph，则显示 "Speeding"，否则显示 "OK"。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出 1 个不超过 500 的非负整数，即雷达测到的车速。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出测速仪结果，格式为 `Speed: V - S`，其中 V 为车速，S 为 Speeding 或 OK。'
    ].join('\n'),
    examples: [
        { input: '40', output: 'Speed: 40 - OK', explain: '40 没有超过 60。' },
        { input: '75', output: 'Speed: 75 - Speeding', explain: '75 超过了 60。' }
    ],
    constraints: [
        '0 <= 车速 <= 500'
    ],
    hints: [
        '注意"超出 60"是严格大于 60，恰好 60 不算超速',
        '输出格式中的冒号、空格、短横线都要原样保留'
    ],
    solution: 'if (v > 60) 输出 Speeding 分支，否则输出 OK 分支，两个分支的格式串只差最后那个单词。',
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int v;
    scanf("%d", &v);
    if (v > 60) printf("Speed: %d - Speeding\n", v);
    else printf("Speed: %d - OK\n", v);
    return 0;
}
`,
    sampleCases: [
        { input: '40\n', expected: 'Speed: 40 - OK\n' },
        { input: '75\n', expected: 'Speed: 75 - Speeding\n' }
    ],
    hiddenCases: [
        { input: '60\n', expected: 'Speed: 60 - OK\n' },
        { input: '61\n', expected: 'Speed: 61 - Speeding\n' },
        { input: '0\n', expected: 'Speed: 0 - OK\n' },
        { input: '500\n', expected: 'Speed: 500 - Speeding\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int v;
    scanf("%d", &v);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 7,
    code: '7',
    slug: '7',
    title: '三天打鱼两天晒网',
    difficulty: 'medium',
    score: 15,
    author: '杨起帆（浙江大学城市学院）',
    timeLimit: 3000,
    tags: ['条件判断', '模拟'],
    description: [
        '中国有句俗语叫"三天打鱼两天晒网"。假设某人从某天起，开始"三天打鱼两天晒网"，问这个人在以后的第 N 天中是"打鱼"还是"晒网"。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出 1 个不超过 1000 的正整数 N。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出此人在第 N 天中是 "Fishing"（即"打鱼"）还是 "Drying"（即"晒网"），并输出第几天，格式为 `Fishing in day N` 或 `Drying in day N`。'
    ].join('\n'),
    examples: [
        { input: '103', output: 'Fishing in day 103', explain: '103 % 5 = 3，第 1~3 天打鱼，所以是打鱼日。' },
        { input: '34', output: 'Drying in day 34', explain: '34 % 5 = 4，第 4~5 天晒网。' }
    ],
    constraints: [
        '1 <= N <= 1000'
    ],
    hints: [
        '以 5 天为一个周期：第 1、2、3 天打鱼，第 4、5 天晒网',
        '只需看 N % 5 的余数落在哪个区间（注意余数为 0 相当于周期的第 5 天）'
    ],
    solution: '计算 r = N % 5；当 r 为 1、2、3 时输出 Fishing in day N，否则（r 为 0 或 4）输出 Drying in day N。',
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);
    if (n % 5 == 0 || n % 5 == 4)
        printf("Drying in day %d\n", n);
    else
        printf("Fishing in day %d\n", n);
    return 0;
}
`,
    sampleCases: [
        { input: '103\n', expected: 'Fishing in day 103\n' },
        { input: '34\n', expected: 'Drying in day 34\n' }
    ],
    hiddenCases: [
        { input: '1\n', expected: 'Fishing in day 1\n' },
        { input: '5\n', expected: 'Drying in day 5\n' },
        { input: '999\n', expected: 'Drying in day 999\n' },
        { input: '998\n', expected: 'Fishing in day 998\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 8,
    code: '8',
    slug: '8',
    title: '用天平找小球',
    difficulty: 'easy',
    score: 10,
    author: '杨起帆（浙江大学城市学院）',
    timeLimit: 3000,
    tags: ['条件判断'],
    description: [
        '三个球 A、B、C，大小相同，其中两个重量相同，另外一个的重量与它们不同。请找出这个重量不同的球。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出 3 个正整数，顺序对应球 A、B、C 的重量。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出重量不同于其他两个球的那个球的字母。'
    ].join('\n'),
    examples: [
        { input: '1 1 2', output: 'C', explain: 'A 和 B 重量相同，C 与它们不同。' }
    ],
    constraints: [
        '三个正整数中恰有两个相等'
    ],
    hints: [
        '先比较 A 和 B：如果相等，那么与众不同的只可能是 C',
        '否则再比较 A 和 C；如果 A 与 C 相等则答案是 B，否则答案是 A'
    ],
    solution: '两次 if/else 判断即可：a==b 输出 C；否则 a==c 输出 B；都不是就是 A。',
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int a, b, c;
    scanf("%d %d %d", &a, &b, &c);
    if (a == b) printf("C\n");
    else if (a == c) printf("B\n");
    else printf("A\n");
    return 0;
}
`,
    sampleCases: [
        { input: '1 1 2\n', expected: 'C\n' }
    ],
    hiddenCases: [
        { input: '2 1 2\n', expected: 'B\n' },
        { input: '3 2 3\n', expected: 'B\n' },
        { input: '5 4 4\n', expected: 'A\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int a, b, c;
    scanf("%d %d %d", &a, &b, &c);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 9,
    code: '9',
    slug: '9',
    title: '12-24小时制',
    difficulty: 'easy',
    score: 15,
    author: '杨起帆（浙江大学城市学院）',
    timeLimit: 3000,
    tags: ['条件判断', '模拟'],
    description: [
        '编写一个程序，要求用户输入 24 小时制的时间，然后显示 12 小时制的时间。',
        '',
        '**说明：**中午 12 点被认为是下午，所以 24 小时制的 `12:00` 就是 12 小时制的 `12:0 PM`；而 0 点被认为是第二天的时间，所以是 `0:0 AM`。输出中数字部分的格式与输入相同（小于 10 的数没有前导零，如输入 `5:6` 表示 5 点零 6 分）。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出 24 小时制的时间，格式为 `HH:MM`（冒号分隔）。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出这个时间对应的 12 小时制的时间，数字部分格式与输入的相同，然后跟上空格和 AM 或 PM（大写）。'
    ].join('\n'),
    examples: [
        { input: '21:11', output: '9:11 PM', explain: '21 点即晚上 9 点。' }
    ],
    constraints: [
        '0 <= 小时 <= 23，0 <= 分钟 <= 59'
    ],
    hints: [
        'scanf 的格式串里可以写字面字符：scanf("%d:%d", &h, &m) 会自动跳过冒号',
        '小时分三种情况讨论：小于 12（AM）、等于 12（PM，数字不变）、大于 12（减 12 后 PM）'
    ],
    solution: '按 "%d:%d" 读入 h 和 m；h > 12 时输出 h-12 和 PM，h == 12 时输出 12 和 PM，其余输出 h 和 AM。',
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int h, m;
    scanf("%d:%d", &h, &m);
    if (h > 12) printf("%d:%d PM\n", h - 12, m);
    else if (h == 12) printf("%d:%d PM\n", h, m);
    else printf("%d:%d AM\n", h, m);
    return 0;
}
`,
    sampleCases: [
        { input: '21:11\n', expected: '9:11 PM\n' },
        { input: '12:00\n', expected: '12:0 PM\n' }
    ],
    hiddenCases: [
        { input: '0:30\n', expected: '0:30 AM\n' },
        { input: '5:6\n', expected: '5:6 AM\n' },
        { input: '23:59\n', expected: '11:59 PM\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int h, m;
    scanf("%d:%d", &h, &m);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 10,
    code: '10',
    slug: '10',
    title: '成绩转换',
    difficulty: 'easy',
    score: 15,
    author: '杨起帆（浙江大学城市学院）',
    timeLimit: 3000,
    tags: ['条件判断'],
    description: [
        '本题要求编写程序将一个百分制成绩转换为五分制成绩。转换规则：',
        '',
        '- 大于等于 90 分为 A；',
        '- 小于 90 且大于等于 80 为 B；',
        '- 小于 80 且大于等于 70 为 C；',
        '- 小于 70 且大于等于 60 为 D；',
        '- 小于 60 为 E。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出 1 个整数的百分制成绩。',
        '',
        '**输出格式：**',
        '',
        '输出对应的五分制成绩字母。'
    ].join('\n'),
    examples: [
        { input: '90', output: 'A', explain: '90 分达到 A 的下限。' }
    ],
    constraints: [
        '输入为 0 ~ 100 之间的整数'
    ],
    hints: [
        '从高到低逐档判断，注意边界值：89 属于 B，90 属于 A',
        '最后一段（<60）不需要再判断条件，用 else 即可'
    ],
    solution: 'if-else if 链从 A 判到 D，剩下用 else 输出 E。',
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);
    if (n >= 90) printf("A\n");
    else if (n >= 80) printf("B\n");
    else if (n >= 70) printf("C\n");
    else if (n >= 60) printf("D\n");
    else printf("E\n");
    return 0;
}
`,
    sampleCases: [
        { input: '90\n', expected: 'A\n' }
    ],
    hiddenCases: [
        { input: '89\n', expected: 'B\n' },
        { input: '60\n', expected: 'D\n' },
        { input: '59\n', expected: 'E\n' },
        { input: '100\n', expected: 'A\n' },
        { input: '0\n', expected: 'E\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

/* ===================== 第 4 周（循环） ===================== */
{
    id: 11,
    code: '11',
    slug: '11',
    title: '求符合给定条件的整数集',
    difficulty: 'medium',
    score: 15,
    author: '杨起帆（浙江大学城市学院）',
    timeLimit: 3000,
    tags: ['循环', '枚举'],
    description: [
        '给定不超过 6 的正整数 A，考虑从 A 开始的连续 4 个数字。请输出所有由它们组成的**无重复数字**的 3 位数。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出 1 个不超过 6 的正整数 A。',
        '',
        '**输出格式：**',
        '',
        '输出满足条件的 3 位数，要求从小到大排列，**每 6 个整数占一行**，行间以空格分隔，但**行末不得有多余空格**。'
    ].join('\n'),
    examples: [
        { input: '2', output: '234 235 243 245 253 254\n324 325 342 345 352 354\n423 425 432 435 452 453\n523 524 532 534 542 543', explain: '由 2、3、4、5 组成的无重复数字的三位数共 24 个，按从小到大排列。' }
    ],
    constraints: [
        '1 <= A <= 6'
    ],
    hints: [
        '三重循环枚举百位、十位、个位，再判断三个数字互不相同',
        '用一个计数器数已输出的个数：每输出 1 个先判断是否为该行第 6 个，决定输出空格还是换行'
    ],
    solution: 'i、j、k 分别从 a 到 a+3 枚举，i!=j 且 i!=k 且 j!=k 时输出 ijk；用 count 计数，count%6==0 时换行否则输出空格（行末自然没有多余空格，因为每行恰好 6 个）。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int a, count = 0;
    scanf("%d", &a);
    for (int i = a; i < a + 4; i++)
        for (int j = a; j < a + 4; j++)
            for (int k = a; k < a + 4; k++)
                if (i != j && i != k && j != k) {
                    count++;
                    printf("%d%d%d", i, j, k);
                    if (count % 6 == 0) printf("\n");
                    else printf(" ");
                }
    return 0;
}
`,
    sampleCases: [
        { input: '2\n', expected: '234 235 243 245 253 254\n324 325 342 345 352 354\n423 425 432 435 452 453\n523 524 532 534 542 543\n' }
    ],
    hiddenCases: [
        { input: '1\n', expected: '123 124 132 134 142 143\n213 214 231 234 241 243\n312 314 321 324 341 342\n412 413 421 423 431 432\n' },
        { input: '6\n', expected: '678 679 687 689 697 698\n768 769 786 789 796 798\n867 869 876 879 896 897\n967 968 976 978 986 987\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int a;
    scanf("%d", &a);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 12,
    code: '12',
    slug: '12',
    title: '水仙花数',
    difficulty: 'medium',
    score: 20,
    author: '杨起帆（浙江大学城市学院）',
    timeLimit: 6000,
    tags: ['循环', '数学'],
    description: [
        '水仙花数是指一个 N 位正整数（N ≥ 3），它的每个位上的数字的 N 次幂之和等于它本身。例如：153 = 1³ + 5³ + 3³。',
        '本题要求编写程序，计算所有 N 位水仙花数。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出 1 个正整数 N（3 ≤ N ≤ 7）。',
        '',
        '**输出格式：**',
        '',
        '按递增顺序输出所有 N 位水仙花数，每个数占一行。'
    ].join('\n'),
    examples: [
        { input: '3', output: '153\n370\n371\n407', explain: '三位水仙花数共 4 个。' }
    ],
    constraints: [
        '3 <= N <= 7',
        '本站因浏览器内 C 解释器性能限制，内置测试数据仅覆盖 N = 3、4；更大的 N 可用自定义用例试（可能超出时限）'
    ],
    hints: [
        'N 位数范围是 10^(N-1) ~ 10^N - 1，逐个检验',
        '把 0~9 的 N 次幂先用数组算好，避免重复计算；检验时逐位取出 f%10 并 f/=10'
    ],
    solution: '先用数组 p[d] 存 d 的 N 次幂；对区间内每个 i，把各位数字的 N 次幂求和与 i 比较，相等则输出。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int n, p[10];
    scanf("%d", &n);
    for (int d = 0; d < 10; d++) {
        p[d] = 1;
        for (int k = 0; k < n; k++) p[d] *= d;
    }
    int start = 1, end = 1;
    for (int k = 0; k < n; k++) start *= 10;
    end = start;
    start = start / 10;
    for (int i = start; i < end; i++) {
        int f = i, sum = 0;
        for (int k = 0; k < n; k++) {
            sum += p[f % 10];
            f = f / 10;
        }
        if (sum == i) printf("%d\n", i);
    }
    return 0;
}
`,
    sampleCases: [
        { input: '3\n', expected: '153\n370\n371\n407\n' }
    ],
    hiddenCases: [
        { input: '4\n', expected: '1634\n8208\n9474\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 13,
    code: '13',
    slug: '13',
    title: '打印九九口诀表',
    difficulty: 'medium',
    score: 15,
    author: '杨起帆（浙江大学城市学院）',
    timeLimit: 3000,
    tags: ['循环', '格式化输出'],
    description: [
        '下面是一个完整的下三角九九口诀表：',
        '',
        '    1*1=1',
        '    1*2=2   2*2=4',
        '    1*3=3   2*3=6   3*3=9',
        '    ……',
        '',
        '本题要求对任意给定的一位正整数 N，输出从 `1*1` 到 `N*N` 的部分口诀表。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出 1 个正整数 N（1 ≤ N ≤ 9）。',
        '',
        '**输出格式：**',
        '',
        '输出下三角 N*N 部分口诀表，其中**等号右边数字占 4 位、左对齐**（即 printf 格式 `%-4d`）。'
    ].join('\n'),
    examples: [
        { input: '4', output: '1*1=1\n1*2=2   2*2=4\n1*3=3   2*3=6   3*3=9\n1*4=4   2*4=8   3*4=12  4*4=16', explain: '每行输出 j 从 1 到 i 的口诀，乘积用 %-4d 左对齐占 4 列（行末多余空格不影响判题）。' }
    ],
    constraints: [
        '1 <= N <= 9'
    ],
    hints: [
        '双重循环：外层 i 从 1 到 N，内层 j 从 1 到 i',
        'printf("%d*%d=%-4d", j, i, j*i) 可以直接完成左对齐 4 位的要求'
    ],
    solution: '外层 i 控制行，内层 j 从 1 到 i 输出 "j*i=积"（积用 %-4d），每行结束输出换行。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= i; j++)
            printf("%d*%d=%-4d", j, i, j * i);
        printf("\n");
    }
    return 0;
}
`,
    sampleCases: [
        { input: '4\n', expected: '1*1=1\n1*2=2   2*2=4\n1*3=3   2*3=6   3*3=9\n1*4=4   2*4=8   3*4=12  4*4=16\n' }
    ],
    hiddenCases: [
        { input: '1\n', expected: '1*1=1\n' },
        { input: '5\n', expected: '1*1=1\n1*2=2   2*2=4\n1*3=3   2*3=6   3*3=9\n1*4=4   2*4=8   3*4=12  4*4=16\n1*5=5   2*5=10  3*5=15  4*5=20  5*5=25\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 14,
    code: '14',
    slug: '14',
    title: '统计素数并求和',
    difficulty: 'medium',
    score: 20,
    author: '杨起帆（浙江大学城市学院）',
    timeLimit: 3000,
    tags: ['循环', '素数'],
    description: [
        '本题要求统计给定整数 M 和 N 区间内素数的个数并对它们求和。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出 2 个正整数 M 和 N（1 ≤ M ≤ N ≤ 500），以空格分隔。',
        '',
        '**输出格式：**',
        '',
        '在一行中顺序输出 M 和 N 区间内素数的个数以及它们的和，数字间以空格分隔。'
    ].join('\n'),
    examples: [
        { input: '10 31', output: '7 143', explain: '10~31 之间的素数有 11、13、17、19、23、29、31，共 7 个，和为 143。' }
    ],
    constraints: [
        '1 <= M <= N <= 500'
    ],
    hints: [
        '判断单个数 i 是否为素数：用 2~√i 逐个试除',
        '1 不是素数，要单独排除'
    ],
    solution: '对区间内每个 i 先排除 i<2，再用 j*j<=i 的试除法判断，统计个数并累加。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int m, n;
    scanf("%d %d", &m, &n);
    int count = 0, sum = 0;
    for (int i = m; i <= n; i++) {
        int isPrime = 1;
        if (i < 2) isPrime = 0;
        for (int j = 2; j * j <= i; j++)
            if (i % j == 0) { isPrime = 0; break; }
        if (isPrime) {
            count++;
            sum += i;
        }
    }
    printf("%d %d\n", count, sum);
    return 0;
}
`,
    sampleCases: [
        { input: '10 31\n', expected: '7 143\n' }
    ],
    hiddenCases: [
        { input: '1 10\n', expected: '4 17\n' },
        { input: '2 2\n', expected: '1 2\n' },
        { input: '1 100\n', expected: '25 1060\n' },
        { input: '490 500\n', expected: '2 990\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int m, n;
    scanf("%d %d", &m, &n);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 15,
    code: '15',
    slug: '15',
    title: '猜数字游戏',
    difficulty: 'medium',
    score: 15,
    author: '杨起帆（浙江大学城市学院）',
    timeLimit: 3000,
    tags: ['循环', '模拟', '状态机'],
    description: [
        '猜数字游戏是令游戏机随机产生一个 100 以内的正整数，用户输入一个数对其进行猜测，需要你编写程序自动对其与随机产生的被猜数进行比较，并提示大了（Too big），还是小了（Too small），相等表示猜到了。如果猜到，则结束程序输出：',
        '',
        '- 第 1 次猜到：`Bingo!`',
        '- 3 次以内（不含第 1 次）猜到：`Lucky You!`',
        '- 超过 3 次但在 N 次（含第 N 次）以内猜到：`Good Guess!`',
        '',
        '如果超过 N 次都没有猜到，则输出 `Game Over`，并结束程序。如果在到达 N 次之前，用户输入了一个负数，也输出 `Game Over`，并结束程序。',
        '',
        '**输入格式：**',
        '',
        '输入第一行中给出 2 个不超过 100 的正整数，分别是游戏机自己产生的随机数、以及猜测的最大次数 N。随后每行给出一个用户的输入，直到出现负数为止。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出每次猜测相应的提示，直到输出正确的猜测或超过限制。'
    ].join('\n'),
    examples: [
        { input: '58 4\n70\n50\n56\n58\n60\n-2\n', output: 'Too big\nToo small\nToo small\nGood Guess!', explain: '第 4 次猜中（恰好等于 N），输出 Good Guess!，随后的输入不再处理。' }
    ],
    constraints: [
        '被猜数与 N 均为不超过 100 的正整数'
    ],
    hints: [
        '每读入一个猜测先给计数器加 1，再判断"次数超限或负数 → Game Over"',
        '判断猜中时要先于"次数超限"吗？注意：第 N+1 次即使猜对也算失败'
    ],
    solution: '循环读入猜测：count 先自增；若 count > N 或猜测为负，输出 Game Over 并结束；若等于被猜数，按 count 的范围输出 Bingo! / Lucky You! / Good Guess! 并结束；否则输出 Too big 或 Too small 继续读入。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int num, n;
    scanf("%d %d", &num, &n);
    int count = 0, guess;
    scanf("%d", &guess);
    while (1) {
        count++;
        if (guess < 0 || count > n) {
            printf("Game Over\n");
            break;
        }
        if (guess == num) {
            if (count == 1) printf("Bingo!\n");
            else if (count <= 3) printf("Lucky You!\n");
            else printf("Good Guess!\n");
            break;
        }
        if (guess > num) printf("Too big\n");
        else printf("Too small\n");
        scanf("%d", &guess);
    }
    return 0;
}
`,
    sampleCases: [
        { input: '58 4\n70\n50\n56\n58\n60\n-2\n', expected: 'Too big\nToo small\nToo small\nGood Guess!\n' }
    ],
    hiddenCases: [
        { input: '42 4\n42\n', expected: 'Bingo!\n' },
        { input: '42 5\n1\n100\n42\n', expected: 'Too small\nToo big\nLucky You!\n' },
        { input: '42 2\n1\n2\n3\n-1\n', expected: 'Too small\nToo small\nGame Over\n' },
        { input: '42 3\n-5\n', expected: 'Game Over\n' },
        { input: '42 1\n99\n42\n', expected: 'Too big\nGame Over\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int num, n;
    scanf("%d %d", &num, &n);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

/* ===================== 第 5 周 ===================== */
{
    id: 16,
    code: '16',
    slug: '16',
    title: '求序列前N项和',
    difficulty: 'medium',
    score: 15,
    author: '张高燕（浙江大学城市学院）',
    timeLimit: 3000,
    tags: ['循环', '数学', '双精度'],
    description: [
        '本题要求编写程序，计算序列 `2/1 + 3/2 + 5/3 + ...` 的前 N 项之和。注意该序列从第 2 项起，每一项的分子是前一项分子与分母的和，分母是前一项的分子。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出一个正整数 N。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出部分和的值，精确到小数点后 2 位。题目保证计算结果不超过双精度范围。'
    ].join('\n'),
    examples: [
        { input: '20', output: '32.66', explain: '前 20 项之和约为 32.66。' }
    ],
    constraints: [
        '输入为正整数 N',
        '注意 2/1、3/2 这些分数在 C 里要按浮点除法计算'
    ],
    hints: [
        '用三个 double 变量滚动保存分子、分母与临时值：新分母 = 旧分子，新分子 = 旧分子 + 旧分母',
        '累加时写 up / down（两个都是 double），不能写整数除法'
    ],
    solution: 'up=2、down=1 开始，每轮 sum += up/down，然后 t=down、down=up、up+=t，循环 N 次后按 %.2f 输出 sum。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int n;
    double sum = 0, up = 2, down = 1, t;
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
        sum += up / down;
        t = down;
        down = up;
        up = up + t;
    }
    printf("%.2f\n", sum);
    return 0;
}
`,
    sampleCases: [
        { input: '20\n', expected: '32.66\n' }
    ],
    hiddenCases: [
        { input: '1\n', expected: '2.00\n' },
        { input: '2\n', expected: '3.50\n' },
        { input: '3\n', expected: '5.17\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 17,
    code: '17',
    slug: '17',
    title: '约分最简分式',
    difficulty: 'medium',
    score: 15,
    author: '张高燕（浙江大学城市学院）',
    timeLimit: 3000,
    tags: ['数学', '循环'],
    description: [
        '分数可以表示为 `分子/分母` 的形式。编写一个程序，要求用户输入一个分数，然后将其约分为最简分式。**最简分式是指分子和分母不具有公约数的分式**。例如 6/12 可以约分为 1/2；当分子分母相等时，仍然表达为 1/1 的形式。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出一个分数，分子和分母以 `/` 分隔，如 `60/120`。分子和分母均为正整数（不含 0，也不含负号）。题目保证最简分式分母不为 0。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出这个分数约分后的结果，格式与输入的相同，即 `分子/分母` 的最简分式。'
    ].join('\n'),
    examples: [
        { input: '60/120', output: '1/2', explain: '60 和 120 的最大公约数是 60。' }
    ],
    constraints: [
        '分子、分母均为正整数且以 / 分隔',
        '分子分母相等时输出形如 1/1'
    ],
    hints: [
        'scanf 格式串可以写成 "%d/%d"，斜杠会被自动匹配',
        '求最大公约数用辗转相除法：while (b != 0) { r = a % b; a = b; b = r; }，结束后 a 即最大公约数'
    ],
    solution: '读入分子分母的副本 a、b 做辗转相除求最大公约数 g，然后输出 (分子/g)/(分母/g)，格式串 "%d/%d"。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int up, down;
    scanf("%d/%d", &up, &down);
    int a = up, b = down;
    while (b != 0) {
        int r = a % b;
        a = b;
        b = r;
    }
    printf("%d/%d\n", up / a, down / a);
    return 0;
}
`,
    sampleCases: [
        { input: '60/120\n', expected: '1/2\n' }
    ],
    hiddenCases: [
        { input: '6/6\n', expected: '1/1\n' },
        { input: '11/8\n', expected: '11/8\n' },
        { input: '12/18\n', expected: '2/3\n' },
        { input: '100/25\n', expected: '4/1\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int up, down;
    scanf("%d/%d", &up, &down);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 18,
    code: '18',
    slug: '18',
    title: '念数字',
    difficulty: 'medium',
    score: 15,
    author: '张高燕（浙江大学城市学院）',
    timeLimit: 3000,
    tags: ['字符串', '条件判断'],
    description: [
        '输入一个整数，输出每个数字对应的拼音。当整数为负数时，先输出 `fu` 字样。十个数字的拼音对照如下：',
        '',
        '    0: ling   1: yi     2: er     3: san    4: si',
        '    5: wu     6: liu    7: qi     8: ba     9: jiu',
        '',
        '**注意：是逐位念出每个数字**，中间的 0 也要念出来，如 `-600` 输出 `fu liu ling ling`。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出一个整数，如 `1234`。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出这个整数对应的拼音，每个数字的拼音之间用空格分开，**行末没有最后的空格**。'
    ].join('\n'),
    examples: [
        { input: '-600', output: 'fu liu ling ling', explain: '负号念 fu，然后逐位念 6、0、0。' }
    ],
    constraints: [
        '输入为一个整数（int 范围内）'
    ],
    hints: [
        '把输入当作字符串读入最方便（scanf("%s", s)），这样 int 范围边缘也不会出错',
        '负号处理后，剩余每个字符用 switch 或 if 输出对应拼音；拼音之间用空格分隔（用一个"是否已输出过"的标志处理行末空格）'
    ],
    solution: '用 %s 读入字符串；若首字符是 - 输出 fu 并从下标 1 开始；逐个字符 switch 输出拼音，输出之间插入空格（第一个拼音前不加）。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    char s[16];
    scanf("%s", s);
    int i = 0, printed = 0;
    if (s[0] == '-') {
        printf("fu");
        printed = 1;
        i = 1;
    }
    for (; s[i] != 0; i++) {
        if (printed) printf(" ");
        printed = 1;
        switch (s[i]) {
            case '0': printf("ling"); break;
            case '1': printf("yi"); break;
            case '2': printf("er"); break;
            case '3': printf("san"); break;
            case '4': printf("si"); break;
            case '5': printf("wu"); break;
            case '6': printf("liu"); break;
            case '7': printf("qi"); break;
            case '8': printf("ba"); break;
            case '9': printf("jiu"); break;
        }
    }
    printf("\n");
    return 0;
}
`,
    sampleCases: [
        { input: '-600\n', expected: 'fu liu ling ling\n' }
    ],
    hiddenCases: [
        { input: '0\n', expected: 'ling\n' },
        { input: '1234\n', expected: 'yi er san si\n' },
        { input: '-12\n', expected: 'fu yi er\n' },
        { input: '100000\n', expected: 'yi ling ling ling ling ling\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    char s[16];
    scanf("%s", s);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 19,
    code: '19',
    slug: '19',
    title: '求a的连续和',
    difficulty: 'medium',
    score: 15,
    author: '张高燕（浙江大学城市学院）',
    timeLimit: 3000,
    tags: ['循环', '数学'],
    description: [
        '给定两个均不超过 9 的正整数 a 和 n，要求编写程序求 `a + aa + aaa + ... + aa...a`（n 个 a）之和。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出不超过 9 的两个正整数 a 和 n，以空格分隔。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出 `a + aa + aaa + ... + aa...a`（n 个 a）的和。'
    ].join('\n'),
    examples: [
        { input: '2 4', output: '2468', explain: '2 + 22 + 222 + 2222 = 2468。' }
    ],
    constraints: [
        '1 <= a <= 9，1 <= n <= 8（a 也可为 0，此时和为 0）',
        '保证结果在 int 范围内'
    ],
    hints: [
        'x 从 a 开始，每轮 x = x*10 + a 就得到下一项',
        '和与 x 都要初始化为 0/初值，累加 n 次即可'
    ],
    solution: 'x 初值为 a，循环 n 次：sum += x; x = x*10 + a; 最后输出 sum。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int a, n, x, sum = 0;
    scanf("%d %d", &a, &n);
    x = a;
    for (int i = 0; i < n; i++) {
        sum += x;
        x = x * 10 + a;
    }
    printf("%d\n", sum);
    return 0;
}
`,
    sampleCases: [
        { input: '2 4\n', expected: '2468\n' }
    ],
    hiddenCases: [
        { input: '9 8\n', expected: '111111102\n' },
        { input: '1 1\n', expected: '1\n' },
        { input: '5 2\n', expected: '60\n' },
        { input: '0 8\n', expected: '0\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int a, n;
    scanf("%d %d", &a, &n);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

/* ===================== 第 6 周 ===================== */
{
    id: 20,
    code: '20',
    slug: '20',
    title: '混合类型数据格式化输入',
    difficulty: 'easy',
    score: 5,
    author: '沈鑫（浙江万里学院）',
    timeLimit: 3000,
    tags: ['格式化输出', '模拟'],
    description: [
        '本题要求编写程序，顺序读入浮点数 1、整数、字符、浮点数 2，再按照**字符、整数、浮点数 1、浮点数 2** 的顺序输出。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中顺序给出浮点数 1、整数、字符、浮点数 2，其间以 1 个空格分隔。',
        '',
        '**输出格式：**',
        '',
        '在一行中按照 `字符 整数 浮点数1 浮点数2` 的格式顺序输出，其中浮点数保留小数点后 2 位。'
    ].join('\n'),
    examples: [
        { input: '2.12 88 c 4.7', output: 'c 88 2.12 4.70', explain: '按输出格式要求重排并保留两位小数。' }
    ],
    constraints: [
        '输入的浮点数在 double 范围内，整数为 int 范围内',
        '字符为单个可见字符'
    ],
    hints: [
        'scanf 可以混合多种类型："%f %d %s %f"（字符按字符串读入再取第一个字符即可）',
        'printf 混合输出时注意占位符与参数一一对应'
    ],
    solution: '按 "%f %d %s %f" 读入（字符用长度为 4 的字符数组接，取 cs[0]），然后按 "%c %d %.2f %.2f" 输出。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    double f1, f2;
    int n;
    char cs[4], c;
    scanf("%f %d %s %f", &f1, &n, cs, &f2);
    c = cs[0];
    printf("%c %d %.2f %.2f\n", c, n, f1, f2);
    return 0;
}
`,
    sampleCases: [
        { input: '2.12 88 c 4.7\n', expected: 'c 88 2.12 4.70\n' }
    ],
    hiddenCases: [
        { input: '1.5 7 X 9.25\n', expected: 'X 7 1.50 9.25\n' },
        { input: '-3.456 100 Z 0\n', expected: 'Z 100 -3.46 0.00\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    double f1, f2;
    int n;
    char cs[4], c;
    scanf("%f %d %s %f", &f1, &n, cs, &f2);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 21,
    code: '21',
    slug: '21',
    title: '简单计算器',
    difficulty: 'medium',
    score: 20,
    author: '张彤（绍兴文理学院）',
    timeLimit: 3000,
    tags: ['模拟', '字符串'],
    description: [
        '模拟简单运算器的工作。假设计算器只能进行加、减、乘、除运算，运算数和结果都是**整数**（操作数可能为负），4 种运算符的优先级相同，按从左到右的顺序计算。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出一个四则运算算式，**没有空格**，且至少有一个操作数。遇等号 `=` 说明输入结束。例如 `1+2*10-10/2=`。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出算式的运算结果，或者如果除法分母为 0 或有非法运算符（不是 `+ - * /`），则在一行中输出 `ERROR`。'
    ].join('\n'),
    examples: [
        { input: '1+2*10-10/2=', output: '10', explain: '从左到右：1+2=3，3*10=30，30-10=20，20/2=10。' }
    ],
    constraints: [
        '输入无空格，以 = 结尾',
        '除数为 0 或出现非法运算符时输出 ERROR'
    ],
    hints: [
        '把整行用 getchar 逐字符读入：数字部分用 n = n*10 + (c-\'0\') 累积',
        '遇到运算符后接着读下一个操作数；出现除 0 或非法运算符时先记住（flag），把输入读完再统一输出 ERROR'
    ],
    solution: '先读第一个操作数（支持负号），然后循环：读运算符，若是 = 则结束；再读操作数；根据运算符更新结果，除 0 或非法运算符置 flag。最后按 flag 输出结果或 ERROR。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int n = 0, k = 0, flag = 0, neg;
    char c, op;
    c = getchar();
    if (c == '-') { neg = -1; c = getchar(); }
    else neg = 1;
    while (c >= '0' && c <= '9') {
        n = n * 10 + (c - '0');
        c = getchar();
    }
    n = n * neg;
    while (c != '=') {
        op = c;
        if (op != '+' && op != '-' && op != '*' && op != '/') flag = 1;
        k = 0;
        neg = 1;
        c = getchar();
        if (c == '-') { neg = -1; c = getchar(); }
        while (c >= '0' && c <= '9') {
            k = k * 10 + (c - '0');
            c = getchar();
        }
        k = k * neg;
        if (!flag) {
            if (op == '+') n += k;
            else if (op == '-') n -= k;
            else if (op == '*') n *= k;
            else if (op == '/') {
                if (k == 0) flag = 1;
                else n = n / k;
            }
        }
    }
    if (flag) printf("ERROR\n");
    else printf("%d\n", n);
    return 0;
}
`,
    sampleCases: [
        { input: '1+2*10-10/2=\n', expected: '10\n' }
    ],
    hiddenCases: [
        { input: '5=\n', expected: '5\n' },
        { input: '10/0=\n', expected: 'ERROR\n' },
        { input: '1+2^3=\n', expected: 'ERROR\n' },
        { input: '5*-2=\n', expected: '-10\n' },
        { input: '100/3=\n', expected: '33\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    // 提示：用 getchar() 逐字符读入，数字用 n = n*10 + (c-'0') 累积

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 22,
    code: '22',
    slug: '22',
    title: '字符串字母大小写转换',
    difficulty: 'easy',
    score: 10,
    author: '白洪欢（浙江大学）',
    timeLimit: 3000,
    tags: ['字符串', '循环'],
    description: [
        '本题要求提取一个以 `#` 结束的字符串，把其中的**大小写英文字母互换**后输出（非英文字母的字符原样输出）。',
        '',
        '**输入格式：**',
        '',
        '输入为一个以 `#` 结束的字符串（不超过 40 个字符）。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出大小写互换后的字符串。'
    ].join('\n'),
    examples: [
        { input: 'Hello World! 123#', output: 'hELLO wORLD! 123', explain: '字母互换大小写，空格、感叹号、数字原样保留。' }
    ],
    constraints: [
        '字符串长度不超过 40，以 # 结束（# 不属于字符串）'
    ],
    hints: [
        '用 getchar 逐字符读，读到 # 停止',
        '小写转大写：c - \'a\' + \'A\'；大写转小写：c - \'A\' + \'a\''
    ],
    solution: 'while ((c = getchar()) != \'#\')：先判断大小写并转换，再 printf("%c") 输出，最后补一个换行。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    char c;
    while ((c = getchar()) != '#') {
        if (c >= 'a' && c <= 'z') c = c - 'a' + 'A';
        else if (c >= 'A' && c <= 'Z') c = c - 'A' + 'a';
        printf("%c", c);
    }
    printf("\n");
    return 0;
}
`,
    sampleCases: [
        { input: 'Hello World! 123#\n', expected: 'hELLO wORLD! 123\n' }
    ],
    hiddenCases: [
        { input: 'aBcD#\n', expected: 'AbCd\n' },
        { input: 'ZZ zz#\n', expected: 'zz ZZ\n' },
        { input: '#\n', expected: '\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    char c;

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 23,
    code: '23',
    slug: '23',
    title: '单词长度',
    difficulty: 'medium',
    score: 15,
    author: '白洪欢（浙江大学）',
    timeLimit: 3000,
    tags: ['字符串', '模拟'],
    description: [
        '你的程序要读入一行文本，其中以**空格**分隔为若干个单词，以 `.` 结束。你要输出这行文本中每个单词的长度。这里单词与语言无关，可以包括各种符号，比如 `it\'s` 算一个单词，长度为 4。注意，行中可能出现**连续的空格**；最后的 `.` 不计算在内。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出一行文本，以 `.` 结束。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出这行文本对应的单词的长度，每个长度之间以空格隔开，**行末没有最后的空格**。'
    ].join('\n'),
    examples: [
        { input: "It's great to see you here.", output: '4 5 2 3 3 4', explain: "It's(4) great(5) to(2) see(3) you(3) here(4)。"}
    ],
    constraints: [
        '文本以 . 结束，单词间以空格分隔（可能有连续空格，也可能行首有空格）'
    ],
    hints: [
        '用 getchar 逐字符读，读到 . 停止',
        '维护当前单词长度 len：遇到普通字符 len++；遇到空格且 len>0 则结算一个单词',
        '输出用"是否已输出过"标志，避免行末多余空格'
    ],
    solution: '状态机扫描：非空格字符使 len 增长；空格时若 len>0 输出并清零；读到 . 后把尚未结算的 len 输出（若有）。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int len = 0, printed = 0;
    char c;
    while ((c = getchar()) != '.') {
        if (c == ' ') {
            if (len > 0) {
                if (printed) printf(" ");
                printf("%d", len);
                printed = 1;
                len = 0;
            }
        } else {
            len++;
        }
    }
    if (len > 0) {
        if (printed) printf(" ");
        printf("%d", len);
    }
    printf("\n");
    return 0;
}
`,
    sampleCases: [
        { input: "It's great to see you here.\n", expected: '4 5 2 3 3 4\n' }
    ],
    hiddenCases: [
        { input: 'one two  three.\n', expected: '3 3 5\n' },
        { input: '  leading spaces.\n', expected: '7 6\n' },
        { input: 'a b c d.\n', expected: '1 1 1 1\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    char c;
    int len = 0;

    // 在这里写你的代码

    return 0;
}
`,
    }
},

/* ===================== 第 7 周 ===================== */
{
    id: 24,
    code: '24',
    slug: '24',
    title: '写出这个数',
    difficulty: 'medium',
    score: 20,
    author: '乔林（清华大学）',
    timeLimit: 3000,
    tags: ['字符串', '数学'],
    description: [
        '读入一个正整数 n，计算其各位数字之和，用汉语拼音写出和的每一位数字。',
        '',
        '**输入格式：**',
        '',
        '每个测试输入包含 1 个测试用例，即给出自然数 n 的值。这里保证 n 小于 10^100（超出 int 范围，请按字符串/字符处理）。',
        '',
        '**输出格式：**',
        '',
        '在一行内输出 n 的各位数字之和的每一位，拼音数字间有 1 个空格，但一行中最后一个拼音数字后没有空格。'
    ].join('\n'),
    examples: [
        { input: '1234567890987654321123456789', output: 'yi san wu', explain: '各位数字之和为 135，念作 yi san wu。' }
    ],
    constraints: [
        'n < 10^100，即最多 100 位',
        '拼音对照：0 ling 1 yi 2 er 3 san 4 si 5 wu 6 liu 7 qi 8 ba 9 jiu'
    ],
    hints: [
        '逐字符 getchar 读入，遇到数字字符就 sum += c - \'0\'，100 位也不用任何数组',
        '100 位数字每位最多 9，和不超过 900，再用"除 10 取位"的方法逐位念出 sum'
    ],
    solution: 'getchar 累加各位得 sum（≤900）；从最高位开始，用 len（10 的幂）逐位取出 sum 的数字并 switch 输出拼音，拼音间以空格分隔（sum=0 时输出 ling）。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int sum = 0;
    char c;
    while ((c = getchar()) != '\n') {
        if (c >= '0' && c <= '9') sum += c - '0';
    }
    int len = 1;
    while (sum / len >= 10) len *= 10;
    while (len > 0) {
        int d = sum / len;
        switch (d) {
            case 0: printf("ling"); break;
            case 1: printf("yi"); break;
            case 2: printf("er"); break;
            case 3: printf("san"); break;
            case 4: printf("si"); break;
            case 5: printf("wu"); break;
            case 6: printf("liu"); break;
            case 7: printf("qi"); break;
            case 8: printf("ba"); break;
            case 9: printf("jiu"); break;
        }
        if (len >= 10) printf(" ");
        sum -= d * len;
        len = len / 10;
    }
    printf("\n");
    return 0;
}
`,
    sampleCases: [
        { input: '1234567890987654321123456789\n', expected: 'yi san wu\n' }
    ],
    hiddenCases: [
        { input: '0\n', expected: 'ling\n' },
        { input: '9\n', expected: 'jiu\n' },
        { input: '9999999999\n', expected: 'jiu ling\n' },
        { input: '18\n', expected: 'jiu\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    char c;
    int sum = 0;

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 25,
    code: '25',
    slug: '25',
    title: '换个格式输出整数',
    difficulty: 'easy',
    score: 15,
    author: '乔林（清华大学）',
    timeLimit: 3000,
    tags: ['模拟', '格式化输出'],
    description: [
        '让我们用字母 `B` 来表示"百"、字母 `S` 表示"十"，用 `12...n` 来表示不为零的个位数字 n（n < 10），换个格式来输出任一个不超过 3 位的正整数。例如 `234` 应该被输出为 `BBSSS1234`，因为它有 2 个"百"、3 个"十"、以及个位的 4。',
        '',
        '**输入格式：**',
        '',
        '每个测试输入包含 1 个测试用例，给出正整数 n（< 1000）。',
        '',
        '**输出格式：**',
        '',
        '每个测试用例的输出占一行，输出规定的格式。'
    ].join('\n'),
    examples: [
        { input: '234', output: 'BBSSS1234', explain: '2 个百 → BB，3 个十 → SSS，个位 4 → 1234。' },
        { input: '23', output: 'SS123', explain: '23 没有"百"，有 2 个"十"（SS），个位是 3（123）。' }
    ],
    constraints: [
        'n < 1000'
    ],
    hints: [
        '百位 n/100 个 B，十位 n/10%10 个 S',
        '个位输出 1 到 n%10 的连续数字（个位为 0 时不输出任何数字）'
    ],
    solution: '三个循环分别输出 B、S 和 1..g（g 为个位），最后换行。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int n, i;
    scanf("%d", &n);
    if (n >= 100)
        for (i = 0; i < n / 100; i++) printf("B");
    if (n % 100 >= 10)
        for (i = 0; i < n / 10 % 10; i++) printf("S");
    for (i = 1; i <= n % 10; i++) printf("%d", i);
    printf("\n");
    return 0;
}
`,
    sampleCases: [
        { input: '234\n', expected: 'BBSSS1234\n' },
        { input: '23\n', expected: 'SS123\n' }
    ],
    hiddenCases: [
        { input: '1\n', expected: '1\n' },
        { input: '100\n', expected: 'B\n' },
        { input: '999\n', expected: 'BBBBBBBBBSSSSSSSSS123456789\n' },
        { input: '10\n', expected: 'S\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 26,
    code: '26',
    slug: '26',
    title: 'A+B和C',
    difficulty: 'easy',
    score: 15,
    author: '乔林（清华大学）',
    timeLimit: 3000,
    tags: ['模拟', '数学'],
    description: [
        '给定区间 [-2^31, 2^31] 内的 3 个整数 A、B 和 C，请判断 A + B 是否大于 C。',
        '',
        '**输入格式：**',
        '',
        '输入第 1 行给出正整数 T（≤ 10），是测试用例的个数。随后给出 T 组测试用例，每组占一行，顺序给出 A、B 和 C。整数间以空格分隔。',
        '',
        '**输出格式：**',
        '',
        '对每组测试用例，在一行中输出 `Case #X: true` 如果 A + B > C，否则输出 `Case #X: false`，其中 X 是测试用例的编号（从 1 开始）。'
    ].join('\n'),
    examples: [
        { input: '4\n1 2 3\n2 3 4\n2147483647 0 2147483646\n0 -2147483648 -2147483647', output: 'Case #1: false\nCase #2: true\nCase #3: true\nCase #4: false', explain: '注意 int 边界：A、B 相加可能超出 int，C 语言中要用更宽的类型（或先转换）保存中间结果。' }
    ],
    constraints: [
        'T ≤ 10',
        'A、B、C ∈ [-2^31, 2^31]',
        '本站测试数据已避开超出双精度精确范围的极端值'
    ],
    hints: [
        'A+B 最大约 2^32，超出 int（±2^31）的表达范围',
        '把 A、B、C 读入 double（或 long long）再比较即可'
    ],
    solution: '循环 T 次，把三个数按 double 读入（scanf("%f %f %f") 配 double 变量），比较 a+b>c 输出 Case #i 的结果。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int t;
    double a, b, c;
    scanf("%d", &t);
    for (int i = 1; i <= t; i++) {
        scanf("%f %f %f", &a, &b, &c);
        if (a + b > c) printf("Case #%d: true\n", i);
        else printf("Case #%d: false\n", i);
    }
    return 0;
}
`,
    sampleCases: [
        { input: '4\n1 2 3\n2 3 4\n2147483647 0 2147483646\n0 -2147483648 -2147483647\n', expected: 'Case #1: false\nCase #2: true\nCase #3: true\nCase #4: false\n' }
    ],
    hiddenCases: [
        { input: '3\n2147483647 2147483647 0\n-2147483648 -1 2147483647\n-2147483648 -2147483648 0\n', expected: 'Case #1: true\nCase #2: false\nCase #3: false\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int t;
    scanf("%d", &t);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 27,
    code: '27',
    slug: '27',
    title: '数素数',
    difficulty: 'hard',
    score: 20,
    author: '乔林（清华大学）',
    timeLimit: 10000,
    tags: ['素数', '循环', '格式化输出'],
    description: [
        '令 Pᵢ 表示第 i 个素数。现任给两个正整数 M ≤ N ≤ 10⁴，请输出 P_M 到 P_N 的所有素数。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出 M 和 N，其间以空格分隔。',
        '',
        '**输出格式：**',
        '',
        '输出从 P_M 到 P_N 的所有素数，**每 10 个数字占 1 行**，其间以空格分隔，但**行末不得有多余空格**。'
    ].join('\n'),
    examples: [
        { input: '5 27', output: '11 13 17 19 23 29 31 37 41 43\n47 53 59 61 67 71 73 79 83 89\n97 101 103', explain: '第 5~27 个素数共 23 个，按每 10 个一行输出。' }
    ],
    constraints: [
        '1 <= M <= N <= 10^4',
        '第 10000 个素数是 104729，筛表开到 105000 即可'
    ],
    hints: [
        '先筛出 2~105000 内的所有素数（布尔数组 + 埃氏筛），边筛边计数',
        '输出时用"已输出个数"决定下一个数前面是空格还是换行：每行第 1 个数之前换行'
    ],
    solution: '埃氏筛标记 105000 以内素数；再按序数出第 m~n 个素数，输出时对行内位置取模控制空格/换行，行末自然无多余空格。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int m, n;
    scanf("%d %d", &m, &n);
    int s = 105000;
    int check[105001];
    for (int i = 2; i <= s; i++) check[i] = 1;
    for (int i = 2; i * i <= s; i++)
        if (check[i])
            for (int j = i; j * i <= s; j++) check[i * j] = 0;
    int count = 0, printed = 0;
    for (int i = 2; i <= s; i++) {
        if (!check[i]) continue;
        count++;
        if (count > n) break;
        if (count >= m) {
            if (printed > 0) {
                if (printed % 10 == 0) printf("\n");
                else printf(" ");
            }
            printf("%d", i);
            printed++;
        }
    }
    printf("\n");
    return 0;
}
`,
    sampleCases: [
        { input: '5 27\n', expected: '11 13 17 19 23 29 31 37 41 43\n47 53 59 61 67 71 73 79 83 89\n97 101 103\n' }
    ],
    hiddenCases: [
        { input: '1 10\n', expected: '2 3 5 7 11 13 17 19 23 29\n' },
        { input: '1 1\n', expected: '2\n' },
        { input: '95 100\n', expected: '499 503 509 521 523 541\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int m, n;
    scanf("%d %d", &m, &n);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 28,
    code: '28',
    slug: '28',
    title: '查找整数',
    difficulty: 'easy',
    score: 10,
    author: '杨起帆（浙江大学城市学院）',
    timeLimit: 3000,
    tags: ['查找', '循环'],
    description: [
        '本题要求从输入的 N 个整数中查找给定的 X。如果找到，输出 X 的位置（**从 0 开始**计数）；如果没有找到，输出 "Not Found"。',
        '',
        '**输入格式：**',
        '',
        '输入在第一行中给出 2 个正整数 N（≤ 20）和 X，第二行给出 N 个整数（均不超过 int 范围）。数字均以空格分隔。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出 X 的位置，或者输出 "Not Found"。'
    ].join('\n'),
    examples: [
        { input: '10 5\n1 2 3 4 5 6 7 8 9 10', output: '4', explain: '5 在第 5 个位置，下标为 4。' }
    ],
    constraints: [
        'N ≤ 20',
        '多个 X 时输出最先出现的那个'
    ],
    hints: [
        '边读边判断即可，找到后记住下标（不要提前 break 掉剩余输入的读取）',
        '用 -1 或一个标志表示"没找到"'
    ],
    solution: '循环读 N 个数，用 pos 记录第一个等于 x 的下标（初值 -1），最后按 pos 输出下标或 Not Found。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int n, x, a, pos = -1;
    scanf("%d %d", &n, &x);
    for (int i = 0; i < n; i++) {
        scanf("%d", &a);
        if (pos == -1 && a == x) pos = i;
    }
    if (pos == -1) printf("Not Found\n");
    else printf("%d\n", pos);
    return 0;
}
`,
    sampleCases: [
        { input: '10 5\n1 2 3 4 5 6 7 8 9 10\n', expected: '4\n' }
    ],
    hiddenCases: [
        { input: '3 9\n1 2 3\n', expected: 'Not Found\n' },
        { input: '1 7\n7\n', expected: '0\n' },
        { input: '5 3\n3 3 3 1 2\n', expected: '0\n' },
        { input: '4 9\n1 2 3 9\n', expected: '3\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int n, x;
    scanf("%d %d", &n, &x);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 29,
    code: '29',
    slug: '29',
    title: '求一批整数中出现最多的个位数字',
    difficulty: 'medium',
    score: 20,
    author: '颜晖（浙江大学城市学院）',
    timeLimit: 3000,
    tags: ['数组', '模拟'],
    description: [
        '给定一批整数，分析每个整数的每一位数字，求出现次数最多的个位数字。例如给定 3 个整数 1234、2345、3456，其中出现最多的（出现 3 次）个位数字是 3 和 4。',
        '',
        '**输入格式：**',
        '',
        '输入在第一行中给出正整数 N（≤ 1000）；第二行给出 N 个不超过整型范围的**非负整数**，数字间以空格分隔。',
        '',
        '**输出格式：**',
        '',
        '在一行中按格式 `M: n1 n2 ...` 输出，其中 M 是最大次数，n1、n2、…为出现次数最多的个位数字，**按从小到大**顺序排列，数字间以空格分隔。'
    ].join('\n'),
    examples: [
        { input: '3\n1234 2345 3456', output: '3: 3 4', explain: '数字 3 和 4 各出现 3 次。' }
    ],
    constraints: [
        'N ≤ 1000',
        '整数可为 0（注意 0 也要统计出个位数字 0）'
    ],
    hints: [
        '开一个长度为 10 的计数数组 cnt[d] 统计数字 d 出现的次数',
        '拆位用 do-while 更稳：先取 x%10 再 x/=10，这样 x=0 也能统计出一位'
    ],
    solution: '对每个数 do-while 拆位累计 cnt[]，求最大次数 max，再按 d 从 0 到 9 输出 cnt[d]==max 的数字。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int n, x;
    int cnt[10];
    scanf("%d", &n);
    for (int d = 0; d < 10; d++) cnt[d] = 0;
    for (int i = 0; i < n; i++) {
        scanf("%d", &x);
        do {
            cnt[x % 10]++;
            x = x / 10;
        } while (x > 0);
    }
    int max = 0;
    for (int d = 0; d < 10; d++)
        if (cnt[d] > max) max = cnt[d];
    printf("%d:", max);
    for (int d = 0; d < 10; d++)
        if (cnt[d] == max) printf(" %d", d);
    printf("\n");
    return 0;
}
`,
    sampleCases: [
        { input: '3\n1234 2345 3456\n', expected: '3: 3 4\n' }
    ],
    hiddenCases: [
        { input: '3\n0 10 100\n', expected: '4: 0\n' },
        { input: '1\n7\n', expected: '1: 7\n' },
        { input: '5\n12 34 56 78 90\n', expected: '1: 0 1 2 3 4 5 6 7 8 9\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int n;
    scanf("%d", &n);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 30,
    code: '30',
    slug: '30',
    title: '求矩阵的局部极大值',
    difficulty: 'medium',
    score: 15,
    author: '岑纬（浙江大学城市学院）',
    timeLimit: 3000,
    tags: ['二维数组', '查找'],
    description: [
        '给定一个 M×N 的整数矩阵 A，求其"局部极大值"。**局部极大值**指的是它比相邻的上下左右 4 个元素都大。本题要求输出所有的局部极大值及其所在的行列位置。',
        '',
        '**输入格式：**',
        '',
        '输入第一行中给出 2 个正整数 M 和 N（3 ≤ M, N ≤ 20）；随后 M 行，每行给出 N 个整数，其间以空格分隔。',
        '',
        '**输出格式：**',
        '',
        '每个局部极大值在一行中输出，格式为 `元素值 行号 列号`，其中行号、列号都**从 1 开始**。要求按照行号递增、若同行则按列号递增的顺序输出。若没有局部极大值，则输出 `None 总行数 总列数`。'
    ].join('\n'),
    examples: [
        { input: '5 5\n1 1 1 1 1\n1 3 9 3 1\n1 5 3 5 1\n1 1 1 1 1\n1 1 1 1 1', output: '9 2 3\n5 3 2\n5 3 4', explain: '9 位于第 2 行第 3 列；两个 5 分别位于第 3 行第 2、4 列，均比四邻大。' }
    ],
    constraints: [
        '3 <= M, N <= 20',
        '只判断内部元素（不含四条边界）'
    ],
    hints: [
        '数组开成固定大小 a[20][20] 即可',
        '内部元素下标范围是 i ∈ [1, m-2]、j ∈ [1, n-2]；比较四个方向的邻居',
        '输出行列号时记得 +1（题目从 1 数起）'
    ],
    solution: '双重循环遍历内部元素，与上下左右四个邻居比较，都严格大则输出"值 行号+1 列号+1"；用 found 标志在最后决定输出 None。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int m, n, t;
    int a[20][20];
    scanf("%d %d", &m, &n);
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) {
            scanf("%d", &t);
            a[i][j] = t;
        }
    int found = 0;
    for (int i = 1; i < m - 1; i++)
        for (int j = 1; j < n - 1; j++)
            if (a[i][j] > a[i][j-1] && a[i][j] > a[i][j+1]
                && a[i][j] > a[i-1][j] && a[i][j] > a[i+1][j]) {
                printf("%d %d %d\n", a[i][j], i + 1, j + 1);
                found = 1;
            }
    if (!found) printf("None %d %d\n", m, n);
    return 0;
}
`,
    sampleCases: [
        { input: '5 5\n1 1 1 1 1\n1 3 9 3 1\n1 5 3 5 1\n1 1 1 1 1\n1 1 1 1 1\n', expected: '9 2 3\n5 3 2\n5 3 4\n' }
    ],
    hiddenCases: [
        { input: '3 3\n1 2 1\n2 9 2\n1 2 1\n', expected: '9 2 2\n' },
        { input: '3 4\n5 5 5 5\n5 5 5 5\n5 5 5 5\n', expected: 'None 3 4\n' },
        { input: '3 3\n1 2 3\n4 5 6\n7 8 9\n', expected: 'None 3 3\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int m, n;
    scanf("%d %d", &m, &n);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 31,
    code: '31',
    slug: '31',
    title: '组个最小数',
    difficulty: 'medium',
    score: 20,
    author: '乔林（清华大学）',
    timeLimit: 3000,
    tags: ['贪心', '模拟'],
    description: [
        '给定数字 0~9 各若干个。你可以以任意顺序排列这些数字，但必须全部使用。目标是使得最后得到的数尽可能小（注意 0 不能做首位）。例如：给定两个 0、两个 1、三个 5、一个 8，我们得到的最小的数就是 10015558。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出 10 个非负整数，顺序表示数字 0、数字 1、……、数字 9 的个数（总个数不超过 50）。题目保证至少有一个数字的个数非零。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出能组成的最小的数。'
    ].join('\n'),
    examples: [
        { input: '2 2 0 0 0 3 0 0 1 0', output: '10015558', explain: '先拿一个最小的非零数字 1 做首位，再把剩余数字从小到大排列。' }
    ],
    constraints: [
        '10 个计数之和 ≤ 50',
        '题目保证至少有一个数字的个数非零'
    ],
    hints: [
        '首位必须是 1~9 中最小的有货数字，输出一个后它的计数减一',
        '然后从 0 到 9 依次把剩余数字全部输出'
    ],
    solution: '先从 d=1 找到第一个 a[d]>0，输出 d 并 a[d]--；再双重循环按 0→9、每个 d 输出 a[d] 遍，最后换行。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    int a[10], t;
    for (int d = 0; d < 10; d++) {
        scanf("%d", &t);
        a[d] = t;
    }
    int k = 1;
    while (a[k] == 0) k++;
    printf("%d", k);
    a[k]--;
    for (int d = 0; d < 10; d++)
        for (int i = 0; i < a[d]; i++)
            printf("%d", d);
    printf("\n");
    return 0;
}
`,
    sampleCases: [
        { input: '2 2 0 0 0 3 0 0 1 0\n', expected: '10015558\n' }
    ],
    hiddenCases: [
        { input: '3 0 1 0 0 0 0 0 0 0\n', expected: '2000\n' },
        { input: '0 2 3 0 0 0 0 0 0 0\n', expected: '11222\n' },
        { input: '1 1 1 1 1 1 1 1 1 1\n', expected: '1023456789\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    int a[10];
    for (int d = 0; d < 10; d++) scanf("%d", &a[d]);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

/* ===================== 第 10 周（字符串） ===================== */
{
    id: 32,
    code: '32',
    slug: '32',
    title: '说反话',
    difficulty: 'hard',
    score: 20,
    author: '乔林（清华大学）',
    timeLimit: 3000,
    tags: ['字符串', '模拟'],
    description: [
        '给定一句英语，要求你编写程序，将句中所有单词的顺序**颠倒输出**。',
        '',
        '**输入格式：**',
        '',
        '测试输入包含一个测试用例，在一行内给出总长度不超过 80 的字符串。字符串由若干单词和若干空格组成，其中**单词是由英文字母组成的字符串**，单词之间以 1 个空格分开，保证句首、句末没有多余的空格。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出按顺序颠倒后的句子，单词之间以 1 个空格分开。'
    ].join('\n'),
    examples: [
        { input: 'Hello World Here I Come', output: 'Come I Here World Hello', explain: '单词顺序整个颠倒。' }
    ],
    constraints: [
        '字符串长度不超过 80，单词间恰好 1 个空格，首尾无空格'
    ],
    hints: [
        '把整行读入字符数组，从后往前扫描',
        '记录每个单词的结束位置，向前找单词起点，逐词输出；单词之间补一个空格'
    ],
    solution: '从串尾向串头扫描：跳过空格后确定一个单词的右端点，继续向左找到左端点，输出该单词（用 %c 逐字符或先存后打），并用"是否已输出"标志控制单词间空格。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    char s[85];
    int l = 0;
    while ((s[l] = getchar()) != '\n') l++;
    s[l] = 0;
    int i = l - 1, printed = 0;
    while (i >= 0) {
        while (i >= 0 && s[i] == ' ') i--;
        if (i < 0) break;
        int end = i;
        while (i >= 0 && s[i] != ' ') i--;
        if (printed) printf(" ");
        for (int j = i + 1; j <= end; j++) printf("%c", s[j]);
        printed = 1;
    }
    printf("\n");
    return 0;
}
`,
    sampleCases: [
        { input: 'Hello World Here I Come\n', expected: 'Come I Here World Hello\n' }
    ],
    hiddenCases: [
        { input: 'a\n', expected: 'a\n' },
        { input: 'one two\n', expected: 'two one\n' },
        { input: 'a b c d e f g\n', expected: 'g f e d c b a\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    char s[85];
    int l = 0;
    while ((s[l] = getchar()) != '\n') l++;
    s[l] = 0;

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 33,
    code: '33',
    slug: '33',
    title: '在字符串中查找指定字符',
    difficulty: 'medium',
    score: 15,
    author: '白洪欢（浙江大学）',
    timeLimit: 3000,
    tags: ['字符串', '查找'],
    description: [
        '本题要求编写程序，在给定的字符串中查找一个指定的字符，并输出该字符在字符串中**第一次**出现时，其后（含该字符）的所有字符。',
        '',
        '**输入格式：**',
        '',
        '输入的第一行是一个待查找的字符（单个字符）。第二行是一个以回车结束的**非空字符串**（不超过 80 个字符）。',
        '',
        '**输出格式：**',
        '',
        '如果找到，在一行内输出该字符及其后剩余的所有字符；如果未找到，则输出 `Not found`（注意 f 小写）。'
    ].join('\n'),
    examples: [
        { input: 'b\nIt is a black box', output: 'black box', explain: '字符 b 第一次出现在 black 的 b 处。' },
        { input: 'B\nIt is a black box', output: 'Not found', explain: '大小写敏感，B 未出现。' }
    ],
    constraints: [
        '字符串非空、不超过 80 个字符，以回车结束'
    ],
    hints: [
        '先 getchar 读入待查字符，再用 while (getchar() != \'\\n\'); 把这一行行尾吃掉',
        '找到下标后，从该下标循环输出到字符串末尾'
    ],
    solution: '先读入待查字符（getchar）并吃掉行尾，再 getchar 逐字符读入第二行的字符串（含空格）；顺序扫描找第一个匹配下标，找到就从此下标输出到末尾，否则输出 Not found。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    char s[85], c;
    int l = 0;
    c = getchar();
    while (getchar() != '\n');
    while ((s[l] = getchar()) != '\n') l++;
    s[l] = 0;
    int pos = -1;
    for (int i = 0; s[i] != 0; i++) {
        if (s[i] == c) {
            pos = i;
            break;
        }
    }
    if (pos == -1) printf("Not found\n");
    else {
        for (int i = pos; s[i] != 0; i++) printf("%c", s[i]);
        printf("\n");
    }
    return 0;
}
`,
    sampleCases: [
        { input: 'b\nIt is a black box\n', expected: 'black box\n' },
        { input: 'B\nIt is a black box\n', expected: 'Not found\n' }
    ],
    hiddenCases: [
        { input: 'c\nabcdef\n', expected: 'cdef\n' },
        { input: 'z\nhello\n', expected: 'Not found\n' },
        { input: 'n\nbanana\n', expected: 'nana\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    char s[85], c;
    int l = 0;
    while ((s[l] = getchar()) != '\n') l++;
    s[l] = 0;
    c = getchar();

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 34,
    code: '34',
    slug: '34',
    title: '删除字符串中的子串',
    difficulty: 'hard',
    score: 20,
    author: '张高燕（浙江大学城市学院）',
    timeLimit: 3000,
    tags: ['字符串', '模拟'],
    description: [
        '输入 2 个字符串 S1 和 S2，要求**删除字符串 S1 中出现的所有子串 S2**，输出删除后的结果。',
        '',
        '**注意：**删除后可能再出现新的 S2（如 S1 = `ccatat`、S2 = `cat`，删掉中间的 cat 后剩下的字符又拼成 cat），需要**反复删除**直到 S1 中不再出现 S2。',
        '',
        '**输入格式：**',
        '',
        '输入在 2 行中先后给出字符串 S1 和 S2（都不超过 80 个字符，以回车结束）。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出删除子串后的字符串。'
    ].join('\n'),
    examples: [
        { input: 'Tomcat is a male ccatat\ncat', output: 'Tom is a male', explain: '先删掉 Tomcat 中的 cat，再删 ccatat 中先后出现的两次 cat，最终不再有 cat。' }
    ],
    constraints: [
        'S1、S2 长度不超过 80',
        'S2 为非空字符串'
    ],
    hints: [
        '写一个函数式的逻辑：从左到右找 S2 在 S1 中的第一次出现，找到就把后面整体前移 S2 长度',
        '每删除一次后回到开头重新找，直到找不到为止'
    ],
    solution: '双重循环匹配 + 整体前移：外层 while 有删除发生就重来；内层 i 从 0 扫描，命中则把 s[i+l2..] 前移 l2 位、长度减 l2。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    char s[85], p[85];
    int l1 = 0, l2 = 0;
    while ((s[l1] = getchar()) != '\n') l1++;
    s[l1] = 0;
    while ((p[l2] = getchar()) != '\n') l2++;
    p[l2] = 0;
    int removed = 1;
    while (removed && l2 > 0) {
        removed = 0;
        for (int i = 0; i + l2 <= l1; i++) {
            int match = 1;
            for (int j = 0; j < l2; j++)
                if (s[i + j] != p[j]) { match = 0; break; }
            if (match) {
                for (int k = i; k + l2 <= l1; k++) s[k] = s[k + l2];
                l1 -= l2;
                s[l1] = 0;
                removed = 1;
                i = -1;
            }
        }
    }
    printf("%s\n", s);
    return 0;
}
`,
    sampleCases: [
        { input: 'Tomcat is a male ccatat\ncat\n', expected: 'Tom is a male\n' }
    ],
    hiddenCases: [
        { input: 'aaaa\naa\n', expected: '\n' },
        { input: 'hello world\nxyz\n', expected: 'hello world\n' },
        { input: 'abababa\naba\n', expected: 'b\n' },
        { input: 'abcabc\nabc\n', expected: '\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    char s[85], p[85];
    int l1 = 0, l2 = 0;
    while ((s[l1] = getchar()) != '\n') l1++;
    s[l1] = 0;
    while ((p[l2] = getchar()) != '\n') l2++;
    p[l2] = 0;

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 35,
    code: '35',
    slug: '35',
    title: '字符串逆序',
    difficulty: 'medium',
    score: 15,
    author: '乔林（清华大学）',
    timeLimit: 3000,
    tags: ['字符串', '循环'],
    description: [
        '输入一个字符串，对该字符串进行**逆序**，输出逆序后的字符串。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中给出一个不超过 80 个字符长度的、以回车结束的非空字符串。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出逆序后的字符串。'
    ].join('\n'),
    examples: [
        { input: 'Hello World!', output: '!dlroW olleH', explain: '整串字符倒过来。' }
    ],
    constraints: [
        '字符串非空、不超过 80 个字符'
    ],
    hints: [
        '读入时记下长度 l，从 l-1 倒着输出到 0',
        '一行内可能有空格，必须用 getchar 逐字符读'
    ],
    solution: 'getchar 读到回车记录长度 l，然后 for (i = l-1; i >= 0; i--) printf("%c", s[i])，最后换行。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    char s[85];
    int l = 0;
    while ((s[l] = getchar()) != '\n') l++;
    s[l] = 0;
    for (int i = l - 1; i >= 0; i--) printf("%c", s[i]);
    printf("\n");
    return 0;
}
`,
    sampleCases: [
        { input: 'Hello World!\n', expected: '!dlroW olleH\n' }
    ],
    hiddenCases: [
        { input: 'a\n', expected: 'a\n' },
        { input: 'ab\n', expected: 'ba\n' },
        { input: '12345\n', expected: '54321\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    char s[85];
    int l = 0;
    while ((s[l] = getchar()) != '\n') l++;
    s[l] = 0;

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 36,
    code: '36',
    slug: '36',
    title: '字符串循环左移',
    difficulty: 'medium',
    score: 20,
    author: '白洪欢（浙江大学）',
    timeLimit: 3000,
    tags: ['字符串', '模拟'],
    description: [
        '输入一个字符串和一个非负整数 N，要求将字符串**循环左移 N 次**（即把最前面的 N 个字符移到串尾）。',
        '',
        '**输入格式：**',
        '',
        '输入在第 1 行中给出一个不超过 100 个字符长度的、以回车结束的非空字符串；第 2 行给出非负整数 N。',
        '',
        '**输出格式：**',
        '',
        '在一行中输出循环左移 N 次后的字符串。'
    ].join('\n'),
    examples: [
        { input: 'Hello World!\n2', output: 'llo World!He', explain: '左移 2 次：先变 ello World!H，再变 llo World!He。' }
    ],
    constraints: [
        '字符串非空、不超过 100 个字符',
        'N 可能大于字符串长度，此时按周期取模处理'
    ],
    hints: [
        'N 对长度 l 取模即可等价化简',
        '先输出下标 n..l-1，再输出下标 0..n-1'
    ],
    solution: 'n %= l；两段循环用 %c 输出：先 i 从 n 到 l-1，再 i 从 0 到 n-1，最后换行。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    char s[105];
    int n, l = 0;
    while ((s[l] = getchar()) != '\n') l++;
    s[l] = 0;
    scanf("%d", &n);
    n = n % l;
    for (int i = n; i < l; i++) printf("%c", s[i]);
    for (int i = 0; i < n; i++) printf("%c", s[i]);
    printf("\n");
    return 0;
}
`,
    sampleCases: [
        { input: 'Hello World!\n2\n', expected: 'llo World!He\n' }
    ],
    hiddenCases: [
        { input: 'abcde\n7\n', expected: 'cdeab\n' },
        { input: 'abc\n0\n', expected: 'abc\n' },
        { input: 'abc\n3\n', expected: 'abc\n' },
        { input: 'x\n5\n', expected: 'x\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    char s[105];
    int n, l = 0;
    while ((s[l] = getchar()) != '\n') l++;
    s[l] = 0;
    scanf("%d", &n);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

/* ===================== 第 11 周 ===================== */
{
    id: 37,
    code: '37',
    slug: '37',
    title: '平面向量加法',
    difficulty: 'hard',
    score: 10,
    author: '乔林（清华大学）',
    timeLimit: 3000,
    tags: ['双精度', '格式化输出'],
    description: [
        '本题要求编写程序，计算两个二维平面向量的和向量。',
        '',
        '**输入格式：**',
        '',
        '输入在一行中按照 `x1 y1 x2 y2` 的格式给出两个二维平面向量 v1 = (x1, y1) 和 v2 = (x2, y2) 的分量，其间以空格分隔。',
        '',
        '**输出格式：**',
        '',
        '在一行中按照 `(x, y)` 的格式输出和向量，坐标保留小数点后 1 位。**注意：不能输出 -0.0**（例如分量为 -0.04 时，四舍五入会得到 -0.0，此时应输出 0.0）。'
    ].join('\n'),
    examples: [
        { input: '3.5 -2.7 -13.9 8.7', output: '(-10.4, 6.0)', explain: 'x = 3.5 + (-13.9) = -10.4，y = -2.7 + 8.7 = 6.0。' }
    ],
    constraints: [
        '输入分量在 double 范围内',
        '输出保留 1 位小数；-0.0 必须输出为 0.0'
    ],
    hints: [
        '处理 -0.0 的常用技巧：若结果 x 满足 -0.05 < x < 0，则令 x = 0.0',
        '输出格式串里的括号和逗号是普通字符，直接写在格式串里'
    ],
    solution: '读入 4 个 double，求 x = x1+x2、y = y1+y2；对 x、y 做 -0.05 < x < 0 → 0.0 的修正后按 "(%.1f, %.1f)" 输出。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    double x1, y1, x2, y2;
    scanf("%f %f %f %f", &x1, &y1, &x2, &y2);
    double x = x1 + x2, y = y1 + y2;
    if (x > -0.05 && x < 0) x = 0.0;
    if (y > -0.05 && y < 0) y = 0.0;
    /* 逗号和空格用 %c 输出，避免格式串解析问题 */
    printf("(%.1f%c%c%.1f)\n", x, ',', ' ', y);
    return 0;
}
`,
    sampleCases: [
        { input: '3.5 -2.7 -13.9 8.7\n', expected: '(-10.4, 6.0)\n' }
    ],
    hiddenCases: [
        { input: '1 2 3 4\n', expected: '(4.0, 6.0)\n' },
        { input: '-0.03 -0.02 -0.01 -0.01\n', expected: '(0.0, 0.0)\n' },
        { input: '-1.5 2.5 1.5 -2.5\n', expected: '(0.0, 0.0)\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    double x1, y1, x2, y2;
    scanf("%f %f %f %f", &x1, &y1, &x2, &y2);

    // 在这里写你的代码

    return 0;
}
`,
    }
},

{
    id: 38,
    code: '38',
    slug: '38',
    title: '通讯录的录入与显示',
    difficulty: 'hard',
    score: 10,
    author: '白洪欢（浙江大学）',
    timeLimit: 3000,
    tags: ['结构化数据', '模拟', '字符串'],
    description: [
        '通讯录中的一条记录包含下述信息：姓名、生日、性别、固定电话号码、移动电话号码。本题要求编写程序，录入 N 条记录，并且根据命令显示指定第 k 条记录。',
        '',
        '**输入格式：**',
        '',
        '输入在第 1 行中给出正整数 N（≤ 10）。随后 N 行，每行按照 `姓名 生日 性别 固话 手机` 的格式给出一条记录。其中姓名是不超过 10 个字符的不含空格的非空字符串；生日按 `yyyy/mm/dd` 的格式给出（保证是有效日期）；性别为 `M` 或 `F`；固话和手机均为不超过 15 位的连续数字，**前面有可能出现 `+`**（如 +8618618623333）。记录按输入顺序**从 1 开始编号**。最后一行给出正整数 K，并且随后给出 K 个整数（可重复、可超出范围），表示要查询的记录编号。',
        '',
        '**输出格式：**',
        '',
        '对每一条要查询的记录编号，在一行中按照 `姓名 固话 手机 性别 生日` 的格式输出该记录（注意：**输出顺序与输入顺序不同**，生日在最后）。若查询的编号不存在，则相应输出 `Not Found`。'
    ].join('\n'),
    examples: [
        { input: '3\nLaoLao 1967/11/30 F 057187951100 +8618618623333\nLaoXiao 1988/10/22 M 057187951101 +8618618623334\nFeifei 1995/3/2 F 057187951102 +8618618623335\n3\n2 1 7', output: 'LaoXiao 057187951101 +8618618623334 M 1988/10/22\nLaoLao 057187951100 +8618618623333 F 1967/11/30\nNot Found', explain: '查询 2、1 号记录正常输出（注意字段顺序），7 号不存在。' }
    ],
    constraints: [
        'N ≤ 10，各字段均不含空格',
        '输出字段顺序为：姓名 固话 手机 性别 生日'
    ],
    hints: [
        '用 5 个平行的二维字符数组分别存 5 个字段（本站解释器不支持 struct）',
        '性别可以用长度为 3 的字符数组当字符串读入，输出时取第一个字符用 %c 打印',
        'scanf 的 %s 以空白分隔，一行 5 个字段一次读完',
        '查询编号 q 从 1 数起，对应数组下标 q-1；q 不在 [1, N] 范围内则输出 Not Found'
    ],
    solution: '平行数组 name/birth/sex/home/cell 各存一列；循环 %s 读入；查询编号 q 检查 1 ≤ q ≤ N 后按下标 q-1 取记录，输出时按"姓名 固话 手机 性别 生日"重排。'
    ,
    referenceSolution: String.raw`#include <stdio.h>

int main()
{
    char name[10][11], birth[10][11], sex[10][3], home[10][17], cell[10][17];
    int n, k, q;
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
        scanf("%s %s %s %s %s", name[i], birth[i], sex[i], home[i], cell[i]);
    scanf("%d", &k);
    for (int j = 0; j < k; j++) {
        scanf("%d", &q);
        if (q < 1 || q > n)
            printf("Not Found\n");
        else
            printf("%s %s %s %c %s\n", name[q-1], home[q-1], cell[q-1], sex[q-1][0], birth[q-1]);
    }
    return 0;
}
`,
    sampleCases: [
        { input: '3\nLaoLao 1967/11/30 F 057187951100 +8618618623333\nLaoXiao 1988/10/22 M 057187951101 +8618618623334\nFeifei 1995/3/2 F 057187951102 +8618618623335\n3\n2 1 7\n', expected: 'LaoXiao 057187951101 +8618618623334 M 1988/10/22\nLaoLao 057187951100 +8618618623333 F 1967/11/30\nNot Found\n' }
    ],
    hiddenCases: [
        { input: '2\nAlice 1990/01/01 F 010-8888 +8613800138000\nBob 2000/12/31 M 021-6666 13900000000\n4\n1 2 3 -1\n', expected: 'Alice 010-8888 +8613800138000 F 1990/01/01\nBob 021-6666 13900000000 M 2000/12/31\nNot Found\nNot Found\n' },
        { input: '1\nTom 2001/07/04 M 12345678 +8613800000000\n2\n1 2\n', expected: 'Tom 12345678 +8613800000000 M 2001/07/04\nNot Found\n' }
    ],
    langTemplates: {
        c: String.raw`#include <stdio.h>

int main()
{
    char name[10][11], birth[10][11], sex[10][3], home[10][17], cell[10][17];
    int n;
    scanf("%d", &n);

    // 在这里写你的代码

    return 0;
}
`,
    }
}
];

/* ---- 为每道题统一生成 C / C++ / Python 三种语言的纯注释模板（覆盖题内旧模板字段） ---- */
(function () {
    var bank = window.QUESTION_BANK;
    for (var i = 0; i < bank.length; i++) {
        var io = extractIO(bank[i].description);
        bank[i].langTemplates = {
            c: buildTemplate('c', io),
            cpp: buildTemplate('cpp', io),
            python: buildTemplate('python', io)
        };
    }
})();
