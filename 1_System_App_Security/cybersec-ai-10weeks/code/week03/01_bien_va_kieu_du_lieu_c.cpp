// Tuần 03 · Bài 01: Biến và kiểu dữ liệu C++.
// Mục tiêu: phân biệt số nguyên, số thực, luận lý, chuỗi và container kích thước
//   cố định; thấy mỗi kiểu dùng để biểu diễn loại thông tin nào.
// Đầu vào: hằng số viết sẵn trong mã, không đọc bàn phím, file hay mạng.
// Đầu ra: một dòng tóm tắt các giá trị mẫu và kích thước của kiểu int.
// An toàn: dữ liệu giả lập cục bộ; không cấp phát thủ công, không I/O ra ngoài.

#include <array>     // std::array: container có số phần tử cố định, biết lúc biên dịch
#include <cstddef>   // std::size_t: kiểu chỉ số/kích thước thư viện chuẩn trả về
#include <iostream>  // std::cout: ghi ra thiết bị đầu ra chuẩn
#include <string>    // std::string: chuỗi tự quản lý vùng nhớ của chính nó

int main() {
    // const khoá giá trị sau khi khởi tạo. Dữ liệu đầu vào của một bài học không
    // có lý do gì để đổi, và khoá lại thì trình biên dịch bắt được lỗi sửa nhầm.
    const int so_su_kien = 3;                     // đếm được, không có phần lẻ
    const double ti_le_canh_bao = 0.66;           // cần phần thập phân
    const bool da_kiem_chung = false;             // chỉ đúng hoặc sai
    const std::string ten_nguon = "lab-fixture";  // độ dài thay đổi được

    // Kích thước nằm ngay trong KIỂU (std::array<int, 3>), nên không thể truyền
    // mảng đi mà quên mất độ dài — khác mảng C thô phải mang theo biến size riêng.
    const std::array<int, 3> diem_rui_ro{20, 55, 80};

    int tong = 0;
    // .size() trả std::size_t; dùng đúng kiểu đó cho biến đếm để tránh cảnh báo
    // so sánh giữa số có dấu và không dấu.
    for (std::size_t i = 0; i < diem_rui_ro.size(); ++i) {
        tong += diem_rui_ro.at(i);  // .at() kiểm tra biên khi chạy
    }

    std::cout << "01 - Biến và kiểu dữ liệu C++: "
              << "nguồn=" << ten_nguon
              << ", số sự kiện=" << so_su_kien
              << ", tổng điểm=" << tong
              << ", tỉ lệ cảnh báo=" << ti_le_canh_bao
              << ", đã kiểm chứng=" << (da_kiem_chung ? "có" : "chưa")
              << ", sizeof(int)=" << sizeof(int) << " byte"
              << '\n';

    return 0;  // 0 báo cho hệ điều hành biết chương trình kết thúc bình thường
}
