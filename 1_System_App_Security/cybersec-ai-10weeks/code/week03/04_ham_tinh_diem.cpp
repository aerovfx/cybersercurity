// Tuần 03 · Bài 04: Hàm tính điểm.
// Mục tiêu: tách phần lõi tính điểm thành một hàm tái sử dụng, gọi lại nhiều lần
//   với dữ liệu khác nhau thay vì chép đi chép lại cùng một vòng lặp.
// Đầu vào: ba tập điểm giả lập khác nhau, truyền vào cùng một hàm.
// Đầu ra: tổng và điểm trung bình của từng tập.
// An toàn: hàm thuần tính toán, không trạng thái toàn cục, không I/O bên trong.

#include <iostream>  // std::cout
#include <string>    // std::string: tên tập dữ liệu
#include <vector>    // std::vector: tập điểm có kích thước biết lúc chạy

// Nhận const& để không sao chép cả vector khi gọi, và để hàm không sửa được dữ
// liệu của người gọi. Trả về int nên chỗ gọi tự quyết định làm gì với kết quả —
// hàm không tự in ra, nhờ vậy nó còn dùng lại được ở nơi không cần in.
int tong_diem(const std::vector<int>& diem) {
    int tong = 0;
    for (const int& d : diem) tong += d;
    return tong;
}

// Trả double vì trung bình của các số nguyên thường không phải số nguyên.
double diem_trung_binh(const std::vector<int>& diem) {
    // Nhánh lỗi: chia cho 0 là hành vi không xác định. Tập rỗng phải bị chặn ở
    // đây, không phải hy vọng người gọi nhớ kiểm tra hộ.
    if (diem.empty()) return 0.0;

    // static_cast<double> ép về số thực TRƯỚC khi chia; chia hai số nguyên thì
    // C++ làm phép chia nguyên và 7/2 ra 3, mất phần lẻ một cách im lặng.
    return static_cast<double>(tong_diem(diem)) / static_cast<double>(diem.size());
}

// Một chỗ in duy nhất, dùng cho mọi tập dữ liệu — cùng lý do như hai hàm trên.
void bao_cao(const std::string& ten, const std::vector<int>& diem) {
    std::cout << "  " << ten << ": tổng=" << tong_diem(diem)
              << ", trung bình=" << diem_trung_binh(diem) << '\n';
}

int main() {
    // Ba tập khác nhau, cùng một hàm. Đó chính là ý nghĩa của "tái sử dụng".
    const std::vector<int> ca_sang{10, 40, 70};
    const std::vector<int> ca_chieu{55, 55, 80, 90};
    const std::vector<int> khong_co_du_lieu{};  // chạy thử nhánh tập rỗng

    bao_cao("ca sáng", ca_sang);
    bao_cao("ca chiều", ca_chieu);
    bao_cao("ca đêm (không có dữ liệu)", khong_co_du_lieu);

    std::cout << "04 - Hàm tính điểm: tổng toàn bộ = "
              << tong_diem(ca_sang) + tong_diem(ca_chieu) << '\n';

    return 0;
}
