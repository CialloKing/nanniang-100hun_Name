// ============================
// 中文单字100分查找脚本（每个结果换行）
// ============================

const fs = require('fs');

// ===== 原始打分函数 =====
function tn(e) {
    if (!e || e.trim() === ``) return 0;
    let t = 0;
    let n = e.trim();
    for (let i = 0; i < n.length; i++) {
        t = (t << 5) - t + n.charCodeAt(i);
        t &= t;
    }
    return Math.abs(t) % 101;
}

// ===== 主函数 =====
function main() {
    console.log("开始查找中文单字100分...");
    
    // 1. 读取JSON文件
    if (!fs.existsSync('common3500.json')) {
        console.error("错误: common3500.json 文件不存在");
        return;
    }
    
    let chars = [];
    try {
        const data = JSON.parse(fs.readFileSync('common3500.json', 'utf8'));
        if (Array.isArray(data)) {
            chars = data;
            console.log(`读取 ${chars.length} 个汉字`);
        } else {
            console.error("错误: JSON格式不是数组");
            return;
        }
    } catch (error) {
        console.error("读取JSON文件失败:", error.message);
        return;
    }
    
    // 2. 筛选单字汉字
    const chineseChars = chars.filter(char => 
        typeof char === 'string' && 
        char.length === 1 && 
        char.charCodeAt(0) >= 0x4E00 && 
        char.charCodeAt(0) <= 0x9FFF
    );
    
    console.log(`有效汉字: ${chineseChars.length} 个`);
    
    // 3. 查找100分汉字
    const results = [];
    for (const char of chineseChars) {
        if (tn(char) === 100) {
            results.push(char);
        }
    }
    
    // 4. 输出结果
    console.log(`\n找到 ${results.length} 个100分中文单字`);
    
    // 控制台输出每行显示10个
    console.log("\n100分中文单字:");
    for (let i = 0; i < results.length; i += 10) {
        console.log(results.slice(i, i + 10).join(' '));
    }
    
    // 5. 保存到txt文件，每个结果换行
    if (results.length > 0) {
        const content = results.join('\n');  // 每个汉字单独一行
        fs.writeFileSync('chinese-100.txt', content);
        console.log(`\n已保存到 chinese-100.txt，共 ${results.length} 行`);
        
        // 显示文件内容预览
        console.log("\n文件内容预览（前10行）：");
        console.log("---------------------");
        results.slice(0, 10).forEach((char, index) => {
            console.log(`${index + 1}. ${char}`);
        });
        if (results.length > 10) {
            console.log(`... 还有 ${results.length - 10} 行`);
        }
    } else {
        console.log("没有找到100分汉字，不保存文件");
    }
}

// ===== 执行 =====
main();