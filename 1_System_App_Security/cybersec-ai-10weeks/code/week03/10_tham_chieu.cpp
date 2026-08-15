// Tuần 03 · Bài 10: Tham chiếu.
// Mục tiêu: dùng tham chiếu để truy cập dữ liệu mà không tạo bản sao thừa, và
//   chứng minh "không sao chép" bằng địa chỉ chứ không bằng lời khẳng định.
// Đầu vào: một bản ghi phát hiện giả lập có chứa chuỗi (thứ tốn kém khi sao chép).
// Đầu ra: địa chỉ của bản gốc so với địa chỉ nhìn thấy bên trong từng kiểu tham số.
// An toàn: tham chiếu luôn gắn với đối tượng đang sống; không có tham chiếu treo.

#include <iostream>  // std::cout
#include <string>    // std::string
#include <vector>    // std::vector

struct PhatHien {
    std::string ma;
    std::string mo_ta;
    int diem;
};

// Tham TRỊ: tạo bản sao toàn bộ struct, gồm cả hai std::string bên trong. Với
// dữ liệu lớn hoặc trong vòng lặp, đây là chi phí thật và hoàn toàn tránh được.
void xem_ban_sao(PhatHien p) {
    std::cout << "    tham trị     -> địa chỉ " << &p << " (bản sao)\n";
}

// Tham chiếu HẰNG: không sao chép, và const bảo đảm hàm không sửa bản gốc. Đây
// là mặc định nên dùng khi chỉ cần đọc một đối tượng không phải kiểu nhỏ gọn.
void xem_tham_chieu(const PhatHien& p) {
    std::cout << "    const&       -> địa chỉ " << &p << " (chính bản gốc)\n";
}

// Tham chiếu KHÔNG hằng: dùng khi thật sự cần sửa. Khác con trỏ ở chỗ tham chiếu
// không thể là null và không cần kiểm tra trước khi dùng.
void nang_diem(PhatHien& p, int them) {
    p.diem += them;
}

int main() {
    PhatHien goc{"LAB-042", "quét cổng trong lab nội bộ", 50};

    std::cout << "  bản gốc      -> địa chỉ " << &goc << '\n';
    xem_ban_sao(goc);
    xem_tham_chieu(goc);

    nang_diem(goc, 25);  // sửa thật, không cần & ở chỗ gọi như con trỏ
    std::cout << "  sau nang_diem: điểm=" << goc.diem << '\n';

    // Trong vòng lặp thì khác biệt này nhân lên theo số phần tử: const& duyệt
    // mà không sao chép phần tử nào.
    const std::vector<PhatHien> ds{goc, {"LAB-043", "đăng nhập sai nhiều lần", 70}};
    int tong = 0;
    for (const PhatHien& p : ds) tong += p.diem;

    std::cout << "10 - Tham chiếu: tổng điểm " << tong << " từ " << ds.size()
              << " bản ghi, duyệt không sao chép bản nào\n";

    return 0;
}
