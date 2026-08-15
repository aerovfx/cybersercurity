// Tuần 04 · Bài 15: Parse số an toàn.
// Mục tiêu: chuyển chuỗi thành số mà xử lý đủ ba kiểu hỏng — không phải số,
//   vượt phạm vi kiểu, và có rác ở đuôi.
// Đầu vào: các chuỗi giả lập gồm cả hợp lệ lẫn hỏng theo từng kiểu.
// Đầu ra: giá trị đọc được hoặc lý do từ chối cho từng chuỗi.
// An toàn: không dùng atoi() — hàm này không có cách nào báo lỗi; mọi kết quả
//   đều được kiểm tra trước khi dùng.

#include <iostream>   // std::cout
#include <stdexcept>  // std::invalid_argument, std::out_of_range
#include <string>     // std::string, std::stoi
#include <vector>     // std::vector

struct KetQuaParse {
    bool ok;
    int gia_tri;
    std::string ly_do;
};

KetQuaParse doc_so_nguyen(const std::string& s) {
    // atoi("abc") trả về 0. atoi("0") cũng trả về 0. Không có cách nào phân biệt
    // hai trường hợp đó, nên atoi() không dùng được cho dữ liệu chưa tin cậy.
    // std::stoi thì NÉM ngoại lệ, tức là lỗi không thể bị bỏ qua trong im lặng.
    try {
        std::size_t da_doc = 0;  // stoi ghi vào đây số ký tự nó đã tiêu thụ
        const int v = std::stoi(s, &da_doc);

        // Nhánh rác ở đuôi: stoi("80abc") trả về 80 và KHÔNG ném. Nếu không so
        // da_doc với độ dài chuỗi thì "80abc" lặng lẽ thành cổng 80 — dữ liệu
        // hỏng đi tiếp vào hệ thống mang bộ mặt dữ liệu sạch.
        if (da_doc != s.size()) return {false, 0, "có ký tự thừa sau số"};

        return {true, v, "hợp lệ"};
    } catch (const std::invalid_argument&) {
        return {false, 0, "không phải số"};
    } catch (const std::out_of_range&) {
        // Vượt phạm vi int. Không bắt nhánh này thì ngoại lệ lan lên và giết
        // chương trình vì một dòng dữ liệu xấu.
        return {false, 0, "vượt phạm vi kiểu int"};
    }
}

// Kiểm tra phạm vi NGHIỆP VỤ là một bước riêng: 99999 là số nguyên hợp lệ nhưng
// không phải cổng hợp lệ. Đọc được và chấp nhận được là hai câu hỏi khác nhau.
bool la_cong_hop_le(int v) { return v >= 1 && v <= 65535; }

int main() {
    const std::vector<std::string> dau_vao{
        "443", "abc", "99999999999999999999", "80abc", "-1", "0", "8080",
    };

    int nhan = 0;
    for (const std::string& s : dau_vao) {
        const KetQuaParse kq = doc_so_nguyen(s);

        if (!kq.ok) {
            std::cout << "  \"" << s << "\" -> TỪ CHỐI (" << kq.ly_do << ")\n";
            continue;
        }
        if (!la_cong_hop_le(kq.gia_tri)) {
            std::cout << "  \"" << s << "\" -> đọc được " << kq.gia_tri
                      << " nhưng ngoài dải cổng 1–65535\n";
            continue;
        }
        ++nhan;
        std::cout << "  \"" << s << "\" -> cổng " << kq.gia_tri << '\n';
    }

    std::cout << "15 - Parse số an toàn: " << nhan << "/" << dau_vao.size()
              << " chuỗi cho ra cổng hợp lệ; 0 lần dùng atoi()\n";

    return 0;
}
