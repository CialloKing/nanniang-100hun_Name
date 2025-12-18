#include <iostream>
#include <fstream>
#include <vector>
#include <codecvt>
#include <locale>
#include <cstdint>
#include <cmath>
#include <string>
#include <ctime>




// 
// 以下是2字版
//完全模拟 JS 32 位整数累积
//
//保留溢出行为，保证 tn2 值与 JS 一致
//
//UTF - 8 输入 / 输出
//
//level - 1.txt 为 UTF - 8 编码
//
//输出 hit - 2.txt 也是 UTF - 8
//
//抽样校验
//
//随机抽 10 条组合检查 tn2 是否等于 100

//这个版本成功正确生成
//// 模拟 JS tn2 函数
//int tn2(char32_t a, char32_t b) {
//    int32_t t = 0;
//    t = t * 31 + a;      // 32 位有符号整数累积
//    t = (t * 31 + b);    // 保持32位溢出特性
//    return std::abs(t) % 101;
//}
//
//// 读取 UTF-8 汉字文件到 wchar_t 向量
//std::vector<wchar_t> read_chars(const std::string& filename, size_t limit = 3500) {
//    std::wifstream fin(filename);
//    fin.imbue(std::locale(fin.getloc(), new std::codecvt_utf8<wchar_t>));
//    std::vector<wchar_t> chars;
//    wchar_t ch;
//    while (chars.size() < limit && fin.get(ch)) {
//        if (ch != L'\r' && ch != L'\n')
//            chars.push_back(ch);
//    }
//    return chars;
//}
//
//int main() {
//    const std::string INPUT_FILE = "level-1.txt"; // UTF-8汉字文件
//    const std::string OUTPUT_FILE = "hit-2.txt";
//
//    auto chars = read_chars(INPUT_FILE);           // 默认前3500个汉字
//    std::cout << "使用汉字数量: " << chars.size() << std::endl;
//
//    std::vector<std::wstring> hit;
//    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
//
//    std::cout << "开始扫描 2 字组合..." << std::endl;
//    std::clock_t start = std::clock();
//
//    for (auto a : chars) {
//        for (auto b : chars) {
//            if (tn2(a, b) == 100) {
//                hit.push_back(std::wstring{ a, b });
//            }
//        }
//    }
//
//    double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
//    std::cout << "扫描完成，耗时: " << duration << " 秒" << std::endl;
//    std::cout << "🎯 2 字 100 分数量：" << hit.size() << std::endl;
//
//    // 保存到 TXT
//    std::ofstream fout(OUTPUT_FILE, std::ios::binary);
//    for (auto& w : hit) {
//        std::string line = conv.to_bytes(w) + "\n";
//        fout.write(line.data(), line.size());
//    }
//    fout.close();
//    std::cout << "已保存到 " << OUTPUT_FILE << std::endl;
//
//    // 抽样校验
//    std::srand((unsigned int)std::time(nullptr));
//    std::cout << "抽样校验 (10 条):" << std::endl;
//    for (int i = 0; i < 10 && !hit.empty(); i++) {
//        size_t idx = std::rand() % hit.size();
//        const auto& s = hit[idx];
//        char32_t c1 = s[0];
//        char32_t c2 = s[1];
//        std::cout << conv.to_bytes(s) << " => " << tn2(c1, c2) << std::endl;
//    }
//
//    return 0;
//}
//



//前 1000 个常用汉字，3 字组合 tn() = 100 的 C++ 版本
//
//// 模拟 JS tn() 函数，32 位整数累积
//inline int32_t tn_step(int32_t t, char32_t code) {
//    return (t << 5) - t + code;  // t*31 + code，32 位有符号整数溢出
//}
//
//// 读取 UTF-8 汉字文件到 wchar_t 向量（限制数量）
//std::vector<wchar_t> read_chars(const std::string& filename, size_t limit = 1000) {
//    std::wifstream fin(filename);
//    fin.imbue(std::locale(fin.getloc(), new std::codecvt_utf8<wchar_t>));
//    std::vector<wchar_t> chars;
//    wchar_t ch;
//    while (chars.size() < limit && fin.get(ch)) {
//        if (ch != L'\r' && ch != L'\n')
//            chars.push_back(ch);
//    }
//    return chars;
//}
//
//int main() {
//    const std::string INPUT_FILE = "level-1.txt"; // UTF-8 汉字文件
//    const std::string OUTPUT_FILE = "hit-zh-3.txt";
//
//    auto chars = read_chars(INPUT_FILE, 1000);   // 前 1000 个汉字
//    std::cout << "使用汉字数量: " << chars.size() << std::endl;
//
//    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
//    std::ofstream fout(OUTPUT_FILE, std::ios::binary);
//    if (!fout) {
//        std::cerr << "无法创建输出文件\n";
//        return 1;
//    }
//
//    uint64_t hit = 0;
//    std::cout << "开始扫描 3 字组合..." << std::endl;
//    std::clock_t start = std::clock();
//
//    // 枚举前两字 c1, c2，反推 c3
//    for (auto c1 : chars) {
//        int32_t t1 = tn_step(0, c1);
//        for (auto c2 : chars) {
//            int32_t t2 = tn_step(t1, c2);
//            for (auto c3 : chars) {
//                int32_t t3 = tn_step(t2, c3);
//                if (std::abs(t3) % 101 == 100) {
//                    std::wstring wline = { c1, c2, c3, L'\n' };
//                    std::string utf8 = conv.to_bytes(wline);
//                    fout.write(utf8.data(), utf8.size());
//                    hit++;
//                }
//            }
//        }
//    }
//
//    fout.close();
//    double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
//    std::cout << "扫描完成，耗时: " << duration << " 秒" << std::endl;
//    std::cout << "总命中数: " << hit << std::endl;
//    std::cout << "结果写入: " << OUTPUT_FILE << std::endl;
//
//    // 随机抽样校验
//    std::srand((unsigned int)std::time(nullptr));
//    std::wifstream fin_check(OUTPUT_FILE);
//    fin_check.imbue(std::locale(fin_check.getloc(), new std::codecvt_utf8<wchar_t>));
//    std::vector<std::wstring> sample_lines;
//    std::wstring line;
//    while (std::getline(fin_check, line)) sample_lines.push_back(line);
//    fin_check.close();
//
//    std::cout << "抽样校验 (10 条):" << std::endl;
//    for (int i = 0; i < 10 && !sample_lines.empty(); i++) {
//        size_t idx = std::rand() % sample_lines.size();
//        const auto& s = sample_lines[idx];
//        int32_t t = 0;
//        t = tn_step(t, s[0]);
//        t = tn_step(t, s[1]);
//        t = tn_step(t, s[2]);
//        std::wcout << s << L" => " << std::abs(t) % 101 << std::endl;
//    }
//
//    return 0;
//}
//






// ===================== tn 哈希函数 =====================
inline int32_t tn_step(int32_t t, char32_t code) {
    return (t << 5) - t + code; // t*31 + code，32位有符号整数溢出
}

// ===================== UTF-8 汉字读取 =====================
std::vector<wchar_t> read_chars(const std::string& filename, size_t limit) {
    std::wifstream fin(filename);
    fin.imbue(std::locale(fin.getloc(), new std::codecvt_utf8<wchar_t>));
    std::vector<wchar_t> chars;
    wchar_t ch;
    while (chars.size() < limit && fin.get(ch)) {
        if (ch != L'\r' && ch != L'\n')
            chars.push_back(ch);
    }
    return chars;
}
// ===================== 1 字组合（3500 常用字） =====================
void scan1char(const std::string& inputFile, const std::string& outputFile, size_t limit = 3500) {
    auto chars = read_chars(inputFile, limit);
    std::cout << "[1字] 使用汉字数量: " << chars.size() << std::endl;

    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    std::ofstream fout(outputFile, std::ios::binary);
    uint64_t hit = 0;

    for (auto c : chars) {
        int32_t t = tn_step(0, c);
        if (std::abs(t) % 101 == 100) {
            std::wstring wline = { c, L'\n' };
            fout.write(conv.to_bytes(wline).data(), conv.to_bytes(wline).size());
            hit++;
        }
    }

    fout.close();
    std::cout << "[1字] 命中数量: " << hit << ", 输出: " << outputFile << std::endl;
}

// ===================== 2 字组合（3500 常用字） =====================
void scan2chars(const std::string& inputFile, const std::string& outputFile, size_t limit = 3500) {
    auto chars = read_chars(inputFile, limit);
    std::cout << "[2字] 使用汉字数量: " << chars.size() << std::endl;

    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    std::ofstream fout(outputFile, std::ios::binary);
    uint64_t hit = 0;

    for (auto a : chars) {
        for (auto b : chars) {
            int32_t t = 0;
            t = tn_step(t, a);
            t = tn_step(t, b);
            if (std::abs(t) % 101 == 100) {
                std::wstring wline = { a, b, L'\n' };
                fout.write(conv.to_bytes(wline).data(), conv.to_bytes(wline).size());
                hit++;
            }
        }
    }
    fout.close();
    std::cout << "[2字] 命中数量: " << hit << ", 输出: " << outputFile << std::endl;
}

// ===================== 3 字组合（前 1000 常用字） =====================
void scan3chars(const std::string& inputFile, const std::string& outputFile, size_t limit = 1000) {
    auto chars = read_chars(inputFile, limit);
    std::cout << "[3字] 使用汉字数量: " << chars.size() << std::endl;

    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    std::ofstream fout(outputFile, std::ios::binary);
    uint64_t hit = 0;

    for (auto c1 : chars) {
        int32_t t1 = tn_step(0, c1);
        for (auto c2 : chars) {
            int32_t t2 = tn_step(t1, c2);
            for (auto c3 : chars) {
                int32_t t3 = tn_step(t2, c3);
                if (std::abs(t3) % 101 == 100) {
                    std::wstring wline = { c1, c2, c3, L'\n' };
                    fout.write(conv.to_bytes(wline).data(), conv.to_bytes(wline).size());
                    hit++;
                }
            }
        }
    }
    fout.close();
    std::cout << "[3字] 命中数量: " << hit << ", 输出: " << outputFile << std::endl;
}

// ===================== ASCII 组合递归 =====================
std::vector<char> ASCII_CHARS;
size_t ASCII_LEN = 1;
size_t BRUTE = 0;
size_t FILL = 0;
std::ofstream fout_ascii;
std::vector<char32_t> ASCII_CODES;

void fill_ascii(int32_t t, size_t depth, std::vector<char>& buf);
void brute_ascii(int32_t t, size_t depth, std::vector<char>& buf) {
    if (depth == BRUTE) {
        fill_ascii(t, 0, buf);
        return;
    }
    for (size_t i = 0; i < ASCII_CODES.size(); i++) {
        buf[depth] = ASCII_CHARS[i];
        brute_ascii((t * 31 + ASCII_CODES[i]) | 0, depth + 1, buf);
    }
}

void fill_ascii(int32_t t, size_t depth, std::vector<char>& buf) {
    if (depth == FILL) {
        if (std::abs(t) % 101 == 100) {
            fout_ascii.write(buf.data(), buf.size());
            fout_ascii.put('\n');
        }
        return;
    }
    for (size_t i = 0; i < ASCII_CODES.size(); i++) {
        buf[BRUTE + depth] = ASCII_CHARS[i];
        fill_ascii((t * 31 + ASCII_CODES[i]) | 0, depth + 1, buf);
    }
}

void scan_ascii(size_t asciiLen, const std::string& outputFile) {
    std::string ASCII = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    ASCII_CHARS = std::vector<char>(ASCII.begin(), ASCII.end());
    ASCII_CODES.clear();
    for (auto c : ASCII_CHARS) ASCII_CODES.push_back((char32_t)c);

    ASCII_LEN = asciiLen;
    BRUTE = ASCII_LEN <= 4 ? ASCII_LEN : ASCII_LEN / 2;
    FILL = ASCII_LEN - BRUTE;
    std::cout << "[ASCII] ASCII_LEN=" << ASCII_LEN << ", BRUTE=" << BRUTE << ", FILL=" << FILL << std::endl;

    fout_ascii.open(outputFile, std::ios::binary);
    if (!fout_ascii) {
        std::cerr << "[ASCII] 无法创建输出文件\n";
        return;
    }

    std::vector<char> buf(ASCII_LEN);
    brute_ascii(0, 0, buf);

    fout_ascii.close();
    std::cout << "[ASCII] 生成完成: " << outputFile << std::endl;
}

// ===================== main =====================
int main() {

    // 3500 常用字 1 字组合
    //scan1char("level-1.txt", "hit-1-3500.txt", 3500);

    // 3500 常用字 2 字组合
    //scan2chars("level-1.txt", "hit-2-3500.txt", 3500);

    // 前 1000 常用字 3 字组合
    //scan3chars("level-1.txt", "hit-3-1000.txt", 1000);

    // ASCII 组合，长度可调
    // 可以改成 1~8
    //scan_ascii(1, "hit-ascii-1.txt"); 
    //scan_ascii(2, "hit-ascii-2.txt");
    //scan_ascii(3, "hit-ascii-3.txt");
    //scan_ascii(4, "hit-ascii-4.txt");

    //1生成文件实际期望大小 0.0000012 MB
    //2生成文件实际期望大小 0.00011 MB
    //3生成文件实际期望大小 0.0090 MB
    //4生成文件实际期望大小 0.697 MB
    //5生成文件实际期望大小 51.9 MB
    //6生成文件实际期望大小 3, 755 MB ≈ 3.66 GB
    //7生成文件实际期望大小 266, 000 MB ≈ 260 GB
    //8生成文件实际期望大小 18, 500, 000 MB ≈ 18 TB



    return 0;
}


