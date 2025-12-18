const fs = require("fs");
const chars = require("./common3500.json");

function tn2(a, b) {
    let t = 0;
    t = (t * 31 + a.charCodeAt(0)) | 0;
    t = (t * 31 + b.charCodeAt(0)) | 0;
    return Math.abs(t) % 101;
}

const hit = [];

console.time("scan-2");

for (const a of chars) {
    for (const b of chars) {
        if (tn2(a, b) === 100) {
            hit.push(a + b);
        }
    }
}

console.timeEnd("scan-2");

console.log("🎯 2 字 100 分数量：", hit.length);

// ===== 保存到 TXT =====
const filename = "hit-2.txt";
fs.writeFileSync(filename, hit.join("\n"), "utf8");

console.log(`已保存到 ${filename}`);

// ===== 抽样校验 =====
for (let i = 0; i < 10 && hit.length; i++) {
    const s = hit[Math.floor(Math.random() * hit.length)];
    console.log(s, tn2(s[0], s[1]));
}
