const fs = require("fs");

const ASCII = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const A = [...ASCII];
const CODES = A.map(c => c.charCodeAt(0));

const ASCII_LEN = 1; // ← 改这里：1～8

// tn 原始等价（raw）
function tnRawPrefix(s) {
    let t = 0;
    for (let i = 0; i < s.length; i++) {
        t = (t << 5) - t + s.charCodeAt(i);
        t |= 0;
    }
    return t;
}

// ===== 参数策略 =====
const BRUTE = ASCII_LEN <= 4 ? ASCII_LEN : Math.floor(ASCII_LEN / 2);
const FILL  = ASCII_LEN - BRUTE;

console.log(`ASCII=${ASCII_LEN}, 暴力=${BRUTE}, 补位=${FILL}`);

const out = fs.createWriteStream(`hit-ascii-${ASCII_LEN}.txt`, "utf8");
let count = 0;

// ===== 递归暴力前半 =====
function brute(t, depth, buf) {
    if (depth === BRUTE) {
        fill(t, 0, buf);
        return;
    }
    for (let i = 0; i < CODES.length; i++) {
        buf[depth] = A[i];
        brute((t * 31 + CODES[i]) | 0, depth + 1, buf);
    }
}

// ===== 递归补位后半 =====
function fill(t, depth, buf) {
    if (depth === FILL) {
        if (Math.abs(t) % 101 === 100) {
            out.write(buf.join("") + "\n");
            count++;
        }
        return;
    }
    for (let i = 0; i < CODES.length; i++) {
        buf[BRUTE + depth] = A[i];
        fill((t * 31 + CODES[i]) | 0, depth + 1, buf);
    }
}

// ===== 执行 =====
console.time("run");
brute(0, 0, new Array(ASCII_LEN));
out.end();

console.timeEnd("run");
console.log("🎯 命中数量：", count);
