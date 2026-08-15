// Tuần 03 · Bài 08: Địa chỉ biến.
// Mục tiêu: hiểu địa chỉ bộ nhớ là gì, lấy nó bằng toán tử & và đọc lại giá trị
//   bằng toán tử *; quan trọng hơn là biết khi nào KHÔNG cần địa chỉ.
// Đầu vào: hai biến cục bộ giả lập.
// Đầu ra: giá trị, địa chỉ của từng biến, và bằng chứng truyền tham trị tạo bản sao.
// An toàn: chỉ đọc địa chỉ của biến cục bộ còn sống; không có số học con trỏ.

#include <iostream>  // std::cout

// Tham TRỊ: hàm nhận một BẢN SAO. Sửa bên trong không ảnh hưởng bản gốc, nên ở
// đây không cần địa chỉ của gì cả — đó là mặc định nên chọn khi chỉ cần đọc.
void tang_ban_sao(int diem) {
    diem += 100;  // chỉ đổi bản sao cục bộ, biến của người gọi giữ nguyên
    // In ngay tại đây để thấy bản sao ĐÃ đổi thật — và địa chỉ của nó khác hẳn
    // địa chỉ bản gốc, đó là bằng chứng nó là một ô nhớ riêng.
    std::cout << "    (trong hàm: bản sao=" << diem << " tại địa chỉ " << &diem << ")\n";
}

// Tham CHIẾU qua con trỏ: hàm nhận ĐỊA CHỈ nên sửa được biến gốc. Chỉ dùng khi
// thật sự cần sửa, vì nó cho hàm quyền tác động ra ngoài phạm vi của nó.
void tang_ban_goc(int* diem) {
    // Nhánh lỗi bắt buộc: con trỏ có thể là nullptr, và giải tham chiếu nullptr
    // là hành vi không xác định — thường là sập chương trình.
    if (diem == nullptr) return;
    *diem += 100;  // * đọc/ghi vào ô nhớ mà con trỏ đang chỉ tới
}

int main() {
    int diem_goc = 42;
    const double ti_le = 0.5;

    // & lấy địa chỉ. In ra để thấy đó là một con số định vị ô nhớ, và hai biến
    // khác nhau nằm ở hai địa chỉ khác nhau.
    std::cout << "  diem_goc=" << diem_goc << " tại địa chỉ " << &diem_goc << '\n';
    std::cout << "  ti_le=" << ti_le << " tại địa chỉ " << &ti_le << '\n';

    tang_ban_sao(diem_goc);
    std::cout << "  sau tang_ban_sao (tham trị): diem_goc=" << diem_goc << '\n';

    tang_ban_goc(&diem_goc);  // truyền địa chỉ vì lần này muốn sửa thật
    std::cout << "  sau tang_ban_goc (qua địa chỉ): diem_goc=" << diem_goc << '\n';

    // Gọi với nullptr để chứng minh nhánh kiểm tra ở trên có tác dụng thật.
    tang_ban_goc(nullptr);

    std::cout << "08 - Địa chỉ biến: tham trị không đổi bản gốc, qua địa chỉ thì có; "
              << "giá trị cuối=" << diem_goc << '\n';

    return 0;
}
