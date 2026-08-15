// Tuần 04 · Bài 17: Compiler warnings.
// Mục tiêu: biết trình biên dịch bắt được những lỗi nào khi bật cảnh báo, và
//   viết code sao cho không còn cảnh báo nào — thay vì tập quen chịu đựng chúng.
// Đầu vào: không có; các tình huống được viết sẵn dưới dạng đã sửa đúng.
// Đầu ra: mô tả từng loại cảnh báo và cách viết tránh được nó.
// An toàn: file này biên dịch SẠCH với -Wall -Wextra; mỗi tình huống ghi kèm
//   dạng code sai (trong chú thích) và dạng đã sửa (trong code chạy được).
// Cách kiểm chứng:
//   c++ -std=c++17 -Wall -Wextra -Werror 17_compiler_warnings.cpp -o /tmp/demo

#include <cstddef>   // std::size_t
#include <iostream>  // std::cout
#include <vector>    // std::vector

// TÌNH HUỐNG 1 — so sánh có dấu với không dấu (-Wsign-compare).
//
//   for (int i = 0; i < v.size(); ++i)      // v.size() là size_t (không dấu)
//
// Trình biên dịch phải đổi `i` sang không dấu để so sánh. Với i âm, phép đổi
// biến -1 thành một số khổng lồ và điều kiện thành đúng — vòng lặp chạy quá biên.
// Cách sửa: dùng đúng kiểu chỉ số mà container trả về.
int tong_dung_kieu(const std::vector<int>& v) {
    int t = 0;
    for (std::size_t i = 0; i < v.size(); ++i) t += v[i];
    return t;
}

// TÌNH HUỐNG 2 — tham số không dùng (-Wunused-parameter).
//
// Đôi khi một tham số phải có mặt vì chữ ký hàm bị ràng buộc, nhưng thân hàm
// chưa dùng tới. [[maybe_unused]] nói rõ đó là chủ ý, thay vì để cảnh báo nằm
// đó cho tới khi người đọc quen mắt và bỏ qua cả những cảnh báo thật.
int luon_tra_ve_khong([[maybe_unused]] int chua_dung) { return 0; }

// TÌNH HUỐNG 3 — thiếu nhánh return (-Wreturn-type).
//
//   int phan_loai(int x) { if (x > 0) return 1; }   // đường x <= 0 không trả gì
//
// Dùng giá trị trả về của một hàm đi tới cuối thân mà không return là hành vi
// không xác định. Cách sửa: bảo đảm MỌI đường đi đều return.
int phan_loai(int x) {
    if (x > 0) return 1;
    if (x < 0) return -1;
    return 0;
}

// TÌNH HUỐNG 4 — biến che khuất biến ngoài (-Wshadow).
//
// Đặt tên khác nhau cho hai thứ khác nhau. Che khuất khiến người đọc tưởng đang
// sửa biến ngoài trong khi thật ra chỉ sửa bản bên trong.
int nguong_toan_cuc = 50;

bool vuot_nguong(int diem) {
    const int nguong_cuc_bo = nguong_toan_cuc;  // tên riêng, không che khuất
    return diem > nguong_cuc_bo;
}

int main() {
    const std::vector<int> diem{10, 20, 30};

    std::cout << "  1) chỉ số đúng kiểu size_t: tổng=" << tong_dung_kieu(diem) << '\n';
    std::cout << "  2) tham số [[maybe_unused]]: " << luon_tra_ve_khong(7) << '\n';
    std::cout << "  3) mọi đường đều return: " << phan_loai(-5) << ", " << phan_loai(0)
              << ", " << phan_loai(5) << '\n';
    std::cout << "  4) không che khuất tên: " << (vuot_nguong(60) ? "vượt" : "chưa vượt")
              << '\n';

    // -Werror biến mọi cảnh báo thành lỗi biên dịch. Nên bật trong CI: cảnh báo
    // bị bỏ qua sẽ tích lại tới mức không ai đọc nữa, và lỗi thật lẫn vào đó.
    std::cout << "17 - Compiler warnings: 4 tình huống, file này biên dịch sạch với "
              << "-Wall -Wextra -Werror\n";

    return 0;
}
