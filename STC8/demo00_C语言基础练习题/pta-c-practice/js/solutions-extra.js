/* ============================================================
 * C++ 与 Python 参考答案（按题目编号 code 索引，与 questions.js 的
 * referenceSolution（C）配套）。三种语言的参考答案均经过实跑验证：
 * C/C++ 在 JSCPP 中执行、Python 用 CPython 3.11 执行，全部用例通过。
 * 注意：C++ 参考答案只使用本站解释器支持的子集（无 <string>、无 struct）。
 * ============================================================ */
window.REFERENCE_EXTRA = {

'1': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int a, b;
    cin >> a >> b;
    cout << a << " + " << b << " = " << a + b << "\n";
    cout << a << " - " << b << " = " << a - b << "\n";
    cout << a << " * " << b << " = " << a * b << "\n";
    cout << a << " / " << b << " = " << a / b << "\n";
    return 0;
}
`,
    python: String.raw`a, b = map(int, input().split())
print(f"{a} + {b} = {a + b}")
print(f"{a} - {b} = {a - b}")
print(f"{a} * {b} = {a * b}")
print(f"{a} / {b} = {a // b}")
`
},

'2': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int cm, foot, inch;
    cin >> cm;
    foot = cm / 30.48;
    inch = (cm / 30.48 - foot) * 12;
    cout << foot << " " << inch << "\n";
    return 0;
}
`,
    python: String.raw`cm = int(input())
foot = int(cm / 30.48)
inch = int((cm / 30.48 - foot) * 12)
print(foot, inch)
`
},

'3': {
    cpp: String.raw`#include <cstdio>

int main()
{
    int a, b, total;
    scanf("%d %d", &a, &b);
    total = (a / 100) * 60 + a % 100 + b;
    printf("%d%02d\n", total / 60, total % 60);
    return 0;
}
`,
    python: String.raw`a, b = map(int, input().split())
total = (a // 100) * 60 + a % 100 + b
print(f"{total // 60}{total % 60:02d}")
`
},

'4': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int n;
    cin >> n;
    int h = n / 100, t = n / 10 % 10, o = n % 10;
    cout << o * 100 + t * 10 + h << "\n";
    return 0;
}
`,
    python: String.raw`n = int(input())
print(n % 10 * 100 + n // 10 % 10 * 10 + n // 100)
`
},

'5': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int n;
    cin >> n;
    cout << n % 16 + n / 16 * 10 << "\n";
    return 0;
}
`,
    python: String.raw`n = int(input())
print(n % 16 + n // 16 * 10)
`
},

'6': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int v;
    cin >> v;
    if (v > 60) cout << "Speed: " << v << " - Speeding\n";
    else cout << "Speed: " << v << " - OK\n";
    return 0;
}
`,
    python: String.raw`v = int(input())
if v > 60:
    print(f"Speed: {v} - Speeding")
else:
    print(f"Speed: {v} - OK")
`
},

'7': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int n;
    cin >> n;
    if (n % 5 == 0 || n % 5 == 4)
        cout << "Drying in day " << n << "\n";
    else
        cout << "Fishing in day " << n << "\n";
    return 0;
}
`,
    python: String.raw`n = int(input())
if n % 5 == 0 or n % 5 == 4:
    print(f"Drying in day {n}")
else:
    print(f"Fishing in day {n}")
`
},

'8': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int a, b, c;
    cin >> a >> b >> c;
    if (a == b) cout << "C\n";
    else if (a == c) cout << "B\n";
    else cout << "A\n";
    return 0;
}
`,
    python: String.raw`a, b, c = map(int, input().split())
if a == b:
    print("C")
elif a == c:
    print("B")
else:
    print("A")
`
},

'9': {
    cpp: String.raw`#include <cstdio>

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
    python: String.raw`h, m = input().split(':')
h = int(h)
m = int(m)
if h >= 12:
    print(f"{h - 12 if h > 12 else h}:{m} PM")
else:
    print(f"{h}:{m} AM")
`
},

'10': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int n;
    cin >> n;
    if (n >= 90) cout << "A\n";
    else if (n >= 80) cout << "B\n";
    else if (n >= 70) cout << "C\n";
    else if (n >= 60) cout << "D\n";
    else cout << "E\n";
    return 0;
}
`,
    python: String.raw`n = int(input())
if n >= 90:
    print("A")
elif n >= 80:
    print("B")
elif n >= 70:
    print("C")
elif n >= 60:
    print("D")
else:
    print("E")
`
},

/* ===================== 第 4 周（循环） ===================== */
'11': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int a, count = 0;
    cin >> a;
    for (int i = a; i < a + 4; i++)
        for (int j = a; j < a + 4; j++)
            for (int k = a; k < a + 4; k++)
                if (i != j && i != k && j != k) {
                    count++;
                    cout << i << j << k;
                    if (count % 6 == 0) cout << "\n";
                    else cout << " ";
                }
    return 0;
}
`,
    python: String.raw`a = int(input())
count = 0
line = []
for i in range(a, a + 4):
    for j in range(a, a + 4):
        for k in range(a, a + 4):
            if i != j and i != k and j != k:
                line.append(f"{i}{j}{k}")
                count += 1
                if count % 6 == 0:
                    print(' '.join(line))
                    line = []
if line:
    print(' '.join(line))
`
},

'12': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int n, p[10];
    cin >> n;
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
        if (sum == i) cout << i << "\n";
    }
    return 0;
}
`,
    python: String.raw`n = int(input())
p = [d ** n for d in range(10)]
for i in range(10 ** (n - 1), 10 ** n):
    if i == sum(p[int(c)] for c in str(i)):
        print(i)
`
},

'13': {
    cpp: String.raw`#include <cstdio>

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
    python: String.raw`n = int(input())
for i in range(1, n + 1):
    row = ''.join(f"{j}*{i}={j * i:<4}" for j in range(1, i + 1))
    print(row)
`
},

'14': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int m, n;
    cin >> m >> n;
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
    cout << count << " " << sum << "\n";
    return 0;
}
`,
    python: String.raw`m, n = map(int, input().split())
count = 0
total = 0
for i in range(m, n + 1):
    if i < 2:
        continue
    is_prime = True
    j = 2
    while j * j <= i:
        if i % j == 0:
            is_prime = False
            break
        j += 1
    if is_prime:
        count += 1
        total += i
print(count, total)
`
},

'15': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int num, n;
    cin >> num >> n;
    int count = 0, guess;
    cin >> guess;
    while (true) {
        count++;
        if (guess < 0 || count > n) {
            cout << "Game Over\n";
            break;
        }
        if (guess == num) {
            if (count == 1) cout << "Bingo!\n";
            else if (count <= 3) cout << "Lucky You!\n";
            else cout << "Good Guess!\n";
            break;
        }
        if (guess > num) cout << "Too big\n";
        else cout << "Too small\n";
        cin >> guess;
    }
    return 0;
}
`,
    python: String.raw`num, n = map(int, input().split())
count = 0
while True:
    guess = int(input())
    count += 1
    if guess < 0 or count > n:
        print("Game Over")
        break
    if guess == num:
        if count == 1:
            print("Bingo!")
        elif count <= 3:
            print("Lucky You!")
        else:
            print("Good Guess!")
        break
    if guess > num:
        print("Too big")
    else:
        print("Too small")
`
},

/* ===================== 第 5 周 ===================== */
'16': {
    cpp: String.raw`#include <cstdio>

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
    python: String.raw`n = int(input())
total = 0.0
up, down = 2.0, 1.0
for _ in range(n):
    total += up / down
    up, down = up + down, up
print(f"{total:.2f}")
`
},

'17': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int up, down;
    char slash;
    cin >> up >> slash >> down;
    int a = up, b = down;
    while (b != 0) {
        int r = a % b;
        a = b;
        b = r;
    }
    cout << up / a << "/" << down / a << "\n";
    return 0;
}
`,
    python: String.raw`import math

up, down = map(int, input().split('/'))
g = math.gcd(up, down)
print(f"{up // g}/{down // g}")
`
},

'18': {
    cpp: String.raw`#include <cstdio>

int main()
{
    char s[16];
    scanf("%s", s);
    int printed = 0;
    for (int i = 0; s[i] != 0; i++) {
        if (printed) printf(" ");
        printed = 1;
        switch (s[i]) {
            case '-': printf("fu"); break;
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
    python: String.raw`s = input()
pin = ['ling', 'yi', 'er', 'san', 'si', 'wu', 'liu', 'qi', 'ba', 'jiu']
parts = []
for ch in s:
    if ch == '-':
        parts.append('fu')
    else:
        parts.append(pin[int(ch)])
print(' '.join(parts))
`
},

'19': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int a, n, x, sum = 0;
    cin >> a >> n;
    x = a;
    for (int i = 0; i < n; i++) {
        sum += x;
        x = x * 10 + a;
    }
    cout << sum << "\n";
    return 0;
}
`,
    python: String.raw`a, n = map(int, input().split())
x = a
total = 0
for _ in range(n):
    total += x
    x = x * 10 + a
print(total)
`
},

/* ===================== 第 6 周 ===================== */
'20': {
    cpp: String.raw`#include <cstdio>

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
    python: String.raw`parts = input().split()
f1 = float(parts[0])
n = int(parts[1])
c = parts[2]
f2 = float(parts[3])
print(f"{c} {n} {f1:.2f} {f2:.2f}")
`
},

'21': {
    cpp: String.raw`#include <cstdio>

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
    python: String.raw`s = input()

def read_num(i):
    neg = 1
    if s[i] == '-':
        neg = -1
        i += 1
    v = 0
    while i < len(s) and s[i].isdigit():
        v = v * 10 + int(s[i])
        i += 1
    return neg * v, i

n, i = read_num(0)
flag = False
while i < len(s) and s[i] != '=':
    op = s[i]
    i += 1
    k, i = read_num(i)
    if op not in '+-*/':
        flag = True
    elif not flag:
        if op == '+':
            n += k
        elif op == '-':
            n -= k
        elif op == '*':
            n *= k
        elif op == '/':
            if k == 0:
                flag = True
            else:
                n = int(n / k)
print("ERROR" if flag else n)
`
},

'22': {
    cpp: String.raw`#include <cstdio>

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
    python: String.raw`s = input()
out = []
for ch in s:
    if ch == '#':
        break
    if 'a' <= ch <= 'z':
        out.append(ch.upper())
    elif 'A' <= ch <= 'Z':
        out.append(ch.lower())
    else:
        out.append(ch)
print(''.join(out))
`
},

'23': {
    cpp: String.raw`#include <cstdio>

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
    python: String.raw`s = input()
if s.endswith('.'):
    s = s[:-1]
words = [w for w in s.split(' ') if w]
print(' '.join(str(len(w)) for w in words))
`
},

/* ===================== 第 7 周 ===================== */
'24': {
    cpp: String.raw`#include <cstdio>

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
    python: String.raw`s = input()
total = sum(int(ch) for ch in s)
pin = ['ling', 'yi', 'er', 'san', 'si', 'wu', 'liu', 'qi', 'ba', 'jiu']
print(' '.join(pin[int(ch)] for ch in str(total)))
`
},

'25': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int n;
    cin >> n;
    for (int i = 0; i < n / 100; i++) cout << "B";
    for (int i = 0; i < n / 10 % 10; i++) cout << "S";
    for (int i = 1; i <= n % 10; i++) cout << i;
    cout << "\n";
    return 0;
}
`,
    python: String.raw`n = int(input())
out = 'B' * (n // 100) + 'S' * (n // 10 % 10)
out += ''.join(str(i) for i in range(1, n % 10 + 1))
print(out)
`
},

'26': {
    cpp: String.raw`#include <cstdio>

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
    python: String.raw`t = int(input())
for i in range(1, t + 1):
    a, b, c = map(int, input().split())
    print(f"Case #{i}: {'true' if a + b > c else 'false'}")
`
},

'27': {
    cpp: String.raw`#include <cstdio>

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
    python: String.raw`m, n = map(int, input().split())
LIMIT = 105000
sieve = [True] * (LIMIT + 1)
sieve[0] = sieve[1] = False
i = 2
while i * i <= LIMIT:
    if sieve[i]:
        for j in range(i * i, LIMIT + 1, i):
            sieve[j] = False
    i += 1
primes = []
cnt = 0
for i in range(2, LIMIT + 1):
    if sieve[i]:
        cnt += 1
        if m <= cnt <= n:
            primes.append(i)
        if cnt > n:
            break
lines = []
for start in range(0, len(primes), 10):
    lines.append(' '.join(str(x) for x in primes[start:start + 10]))
print('\n'.join(lines))
`
},

/* ===================== 第 8 周 ===================== */
'28': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int n, x, a, pos = -1;
    cin >> n >> x;
    for (int i = 0; i < n; i++) {
        cin >> a;
        if (pos == -1 && a == x) pos = i;
    }
    if (pos == -1) cout << "Not Found\n";
    else cout << pos << "\n";
    return 0;
}
`,
    python: String.raw`n, x = map(int, input().split())
vals = list(map(int, input().split()))
pos = -1
for i in range(n):
    if pos == -1 and vals[i] == x:
        pos = i
print(pos if pos != -1 else "Not Found")
`
},

'29': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int n, x;
    int cnt[10];
    cin >> n;
    for (int d = 0; d < 10; d++) cnt[d] = 0;
    for (int i = 0; i < n; i++) {
        cin >> x;
        do {
            cnt[x % 10]++;
            x = x / 10;
        } while (x > 0);
    }
    int mx = 0;
    for (int d = 0; d < 10; d++)
        if (cnt[d] > mx) mx = cnt[d];
    cout << mx << ":";
    for (int d = 0; d < 10; d++)
        if (cnt[d] == mx) cout << " " << d;
    cout << "\n";
    return 0;
}
`,
    python: String.raw`n = int(input())
cnt = {}
for tok in input().split():
    for ch in tok:
        cnt[ch] = cnt.get(ch, 0) + 1
mx = max(cnt.values())
digs = sorted(int(d) for d in cnt if cnt[d] == mx)
print(f"{mx}:", *digs)
`
},

'30': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int m, n, t;
    int a[20][20];
    cin >> m >> n;
    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++) {
            cin >> t;
            a[i][j] = t;
        }
    int found = 0;
    for (int i = 1; i < m - 1; i++)
        for (int j = 1; j < n - 1; j++)
            if (a[i][j] > a[i][j-1] && a[i][j] > a[i][j+1]
                && a[i][j] > a[i-1][j] && a[i][j] > a[i+1][j]) {
                cout << a[i][j] << " " << i + 1 << " " << j + 1 << "\n";
                found = 1;
            }
    if (!found) cout << "None " << m << " " << n << "\n";
    return 0;
}
`,
    python: String.raw`m, n = map(int, input().split())
a = [list(map(int, input().split())) for _ in range(m)]
found = False
for i in range(1, m - 1):
    for j in range(1, n - 1):
        v = a[i][j]
        if v > a[i][j-1] and v > a[i][j+1] and v > a[i-1][j] and v > a[i+1][j]:
            print(v, i + 1, j + 1)
            found = True
if not found:
    print(f"None {m} {n}")
`
},

'31': {
    cpp: String.raw`#include <iostream>
using namespace std;

int main()
{
    int a[10], t;
    for (int d = 0; d < 10; d++) {
        cin >> t;
        a[d] = t;
    }
    int k = 1;
    while (a[k] == 0) k++;
    cout << k;
    a[k]--;
    for (int d = 0; d < 10; d++)
        for (int i = 0; i < a[d]; i++)
            cout << d;
    cout << "\n";
    return 0;
}
`,
    python: String.raw`counts = list(map(int, input().split()))
lead = 1
while counts[lead] == 0:
    lead += 1
result = str(lead)
counts[lead] -= 1
for d in range(10):
    result += str(d) * counts[d]
print(result)
`
},

/* ===================== 第 10 周（字符串） ===================== */
'32': {
    cpp: String.raw`#include <cstdio>

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
    python: String.raw`words = input().split()
print(' '.join(reversed(words)))
`
},

'33': {
    cpp: String.raw`#include <cstdio>

int main()
{
    char s[85], c;
    c = getchar();
    while (getchar() != '\n');
    int l = 0;
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
    python: String.raw`c = input()
s = input()
idx = s.find(c)
if idx == -1:
    print("Not found")
else:
    print(s[idx:])
`
},

'34': {
    cpp: String.raw`#include <cstdio>

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
    python: String.raw`s1 = input()
s2 = input()
while s2 in s1:
    s1 = s1.replace(s2, '', 1)
print(s1)
`
},

'35': {
    cpp: String.raw`#include <cstdio>

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
    python: String.raw`print(input()[::-1])
`
},

'36': {
    cpp: String.raw`#include <cstdio>

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
    python: String.raw`s = input()
n = int(input())
n = n % len(s)
print(s[n:] + s[:n])
`
},

/* ===================== 第 11 周 ===================== */
'37': {
    cpp: String.raw`#include <cstdio>

int main()
{
    double x1, y1, x2, y2;
    scanf("%f %f %f %f", &x1, &y1, &x2, &y2);
    double x = x1 + x2, y = y1 + y2;
    if (x > -0.05 && x < 0) x = 0.0;
    if (y > -0.05 && y < 0) y = 0.0;
    printf("(%.1f%c%c%.1f)\n", x, ',', ' ', y);
    return 0;
}
`,
    python: String.raw`x1, y1, x2, y2 = map(float, input().split())
x = x1 + x2
y = y1 + y2
if -0.05 < x < 0:
    x = 0.0
if -0.05 < y < 0:
    y = 0.0
print(f"({x:.1f}, {y:.1f})")
`
},

'38': {
    cpp: String.raw`#include <cstdio>

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
    python: String.raw`n = int(input())
recs = [input().split() for _ in range(n)]
k = int(input())
queries = input().split()
for q in queries:
    qi = int(q)
    if 1 <= qi <= n:
        r = recs[qi - 1]
        print(f"{r[0]} {r[3]} {r[4]} {r[2]} {r[1]}")
    else:
        print("Not Found")
`
}

};
