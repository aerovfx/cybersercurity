// Tuần 03 · Bài 05: std::array cố định.
// Mục tiêu: dùng std::array khi số phần tử đã biết trước, và thấy kích thước
//   được mang theo trong kiểu nên không bao giờ bị thất lạc.
// Đầu vào: bảng port dịch vụ mẫu, cố định 5 phần tử.
// Đầu ra: phần tử đầu/cuối, số phần tử, một truy cập có kiểm biên và tổng.
// An toàn: không cấp phát động, không chỉ số ngoài biên; mọi truy cập qua .at().

#include <array>     // std::array
#include <cstddef>   // std::size_t
#include <iostream>  // std::cout

// Tham số là std::array<int, 5>: độ dài là MỘT PHẦN CỦA KIỂU. Truyền nhầm mảng
// 3 phần tử vào đây là lỗi biên dịch, không phải lỗi lúc chạy. Mảng C thô khi
// truyền đi sẽ suy biến thành con trỏ và mất sạch thông tin độ dài.
int tong_cua(const std::array<int, 5>& port) {
    int tong = 0;
    for (const int& p : port) tong += p;
    return tong;
}

int main() {
    // Số phần tử cố định lúc biên dịch, dữ liệu nằm trên stack — không có new,
    // không có delete, nên không có gì để rò rỉ.
    const std::array<int, 5> port_dich_vu{22, 53, 80, 443, 8080};

    // .size() lấy thẳng từ kiểu; không cần biến `n` đi kèm và do đó không có
    // nguy cơ biến đó lệch khỏi mảng thật sau một lần sửa code.
    const std::size_t so_phan_tu = port_dich_vu.size();

    // .front() / .back() đọc ra ý định rõ hơn hẳn [0] và [n - 1] — nhất là
    // [n - 1], nơi một lỗi trừ thiếu một đơn vị rất dễ lọt qua mắt người đọc.
    std::cout << "  phần tử đầu=" << port_dich_vu.front()
              << ", phần tử cuối=" << port_dich_vu.back()
              << ", số phần tử=" << so_phan_tu << '\n';

    // Truy cập theo chỉ số vẫn dùng .at(): nó kiểm tra biên khi chạy và ném
    // std::out_of_range nếu sai, thay vì đọc trộm vùng nhớ bên cạnh như [].
    std::cout << "  port ở vị trí 2 = " << port_dich_vu.at(2) << '\n';

    std::cout << "05 - std::array cố định: tổng " << tong_cua(port_dich_vu)
              << " từ " << so_phan_tu << " phần tử\n";

    return 0;
}
