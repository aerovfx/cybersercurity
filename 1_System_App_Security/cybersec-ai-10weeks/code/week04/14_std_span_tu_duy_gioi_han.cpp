// Tuần 04 · Bài 14: std::span — tư duy giới hạn.
// Mục tiêu: truyền "con trỏ + độ dài" như MỘT đối tượng duy nhất, để không thể
//   truyền đúng con trỏ mà sai độ dài.
// Đầu vào: dữ liệu từ std::array và std::vector, cùng đi qua một hàm duy nhất.
// Đầu ra: tổng và giá trị lớn nhất tính được từ từng vùng dữ liệu.
// An toàn: mọi truy cập đều qua .at() có kiểm biên; view không sở hữu dữ liệu
//   nên không bao giờ giải phóng thứ nó chỉ được cho mượn.
// Ghi chú: std::span là C++20. Khoá này biên dịch ở C++17 nên bài dùng một bản
//   Span tối giản tự viết — ý tưởng và cách dùng giống hệt, chỉ thiếu tiện ích.

#include <array>      // std::array
#include <cstddef>    // std::size_t
#include <iostream>   // std::cout
#include <stdexcept>  // std::out_of_range
#include <vector>     // std::vector

// View không sở hữu: chỉ giữ con trỏ và độ dài, không cấp phát, không giải phóng.
// Vòng đời dữ liệu vẫn thuộc về array/vector gốc — Span chỉ hợp lệ chừng nào
// vùng đó còn sống, đúng như con trỏ observer ở tuần 03.
class Span {
public:
    Span(const int* du_lieu, std::size_t so_phan_tu) : p_(du_lieu), n_(so_phan_tu) {}

    // Hai hàm dựng tiện lợi: gọi được thẳng với array hay vector mà chỗ gọi
    // không phải tự moi ra .data() và .size() — chính chỗ dễ ghép nhầm nhất.
    template <std::size_t N>
    Span(const std::array<int, N>& a) : p_(a.data()), n_(N) {}
    Span(const std::vector<int>& v) : p_(v.data()), n_(v.size()) {}

    std::size_t size() const { return n_; }

    // .at() kiểm biên như container chuẩn. Đây là điểm khác biệt so với việc
    // truyền int* trần: ở đó không có gì để kiểm tra, vì không ai biết biên ở đâu.
    int at(std::size_t i) const {
        if (i >= n_) throw std::out_of_range("Span::at vượt biên");
        return p_[i];
    }

private:
    const int* p_;
    std::size_t n_;
};

// MỘT hàm dùng được cho mọi nguồn dữ liệu, và không có tham số `n` rời để
// truyền sai. So với `int tong(const int* p, size_t n)`, chữ ký này không có
// cách nào bị gọi với độ dài lớn hơn dữ liệu thật.
int tong(Span v) {
    int t = 0;
    for (std::size_t i = 0; i < v.size(); ++i) t += v.at(i);
    return t;
}

int main() {
    const std::array<int, 4> tu_array{10, 20, 30, 40};
    const std::vector<int> tu_vector{1, 2, 3, 4, 5, 6};

    // Cùng một hàm, hai loại container, không chuyển đổi thủ công.
    std::cout << "  từ std::array  (" << tu_array.size() << " phần tử): tổng="
              << tong(tu_array) << '\n';
    std::cout << "  từ std::vector (" << tu_vector.size() << " phần tử): tổng="
              << tong(tu_vector) << '\n';

    // Span cũng nhìn được một PHẦN của vùng dữ liệu — vẫn mang theo độ dài đúng
    // của phần đó, nên hàm nhận nó không thể đọc quá.
    const Span nua_dau(tu_vector.data(), 3);
    std::cout << "  nửa đầu vector (3 phần tử): tổng=" << tong(nua_dau) << '\n';

    // Nhánh lỗi: chỉ số vượt biên bị bắt, không đọc trộm vùng nhớ bên cạnh.
    try {
        (void)nua_dau.at(99);
    } catch (const std::out_of_range& e) {
        std::cout << "  truy cập vị trí 99: bị chặn (" << e.what() << ")\n";
    }

    std::cout << "14 - std::span tư duy giới hạn: con trỏ và độ dài đi cùng nhau, "
              << "không có tham số rời để truyền lệch\n";

    return 0;
}
