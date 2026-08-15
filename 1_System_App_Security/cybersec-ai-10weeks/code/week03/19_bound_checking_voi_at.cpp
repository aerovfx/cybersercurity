// Tuần 03 · Bài 19: Bound checking với .at().
// Mục tiêu: dùng .at() để chỉ số vượt biên bị phát hiện NGAY khi chạy, thay vì
//   âm thầm đọc vùng nhớ bên cạnh như toán tử [].
// Đầu vào: danh sách điểm giả lập và một chỉ số cố tình nằm ngoài biên.
// Đầu ra: giá trị đọc hợp lệ, và thông báo bắt được ngoại lệ out_of_range.
// An toàn: chỉ số sai được xử lý bằng ngoại lệ; không lần nào dùng [] ngoài biên.

#include <iostream>   // std::cout
#include <stdexcept>  // std::out_of_range: ngoại lệ mà .at() ném ra
#include <vector>     // std::vector

// Đọc có kiểm soát: bọc .at() trong try/catch để lỗi chỉ số thành một giá trị
// mặc định có kiểm soát, thay vì làm sập cả chương trình.
int doc_an_toan(const std::vector<int>& diem, std::size_t vi_tri, int mac_dinh) {
    try {
        // .at() so chỉ số với size() trước khi truy cập. Chi phí là một phép so
        // sánh; đổi lại, một lỗi chỉ số không còn là lỗ hổng đọc ngoài vùng nhớ.
        return diem.at(vi_tri);
    } catch (const std::out_of_range& e) {
        // Nhánh lỗi: nói rõ chỉ số nào sai. Với [] thì không có nhánh này để
        // viết — không có gì báo cho ta biết là đã đọc sai chỗ.
        std::cout << "    chỉ số " << vi_tri << " vượt biên (" << e.what()
                  << "), dùng giá trị mặc định\n";
        return mac_dinh;
    }
}

int main() {
    const std::vector<int> diem{10, 20, 30};

    // Trong biên: trả về đúng phần tử.
    std::cout << "  đọc vị trí 1: " << doc_an_toan(diem, 1, -1) << '\n';

    // Ngoài biên: .at() ném, ta bắt và xử lý. Chương trình đi tiếp bình thường.
    std::cout << "  đọc vị trí 9: " << doc_an_toan(diem, 9, -1) << '\n';

    // Cách phòng ngừa còn rẻ hơn: kiểm tra trước khi đọc. Dùng khi chỉ số vượt
    // biên là chuyện thường gặp, vì ngoại lệ nên dành cho trường hợp bất thường.
    const std::size_t muon_doc = 5;
    if (muon_doc < diem.size()) {
        std::cout << "  kiểm tra trước: đọc được " << diem[muon_doc] << '\n';
    } else {
        std::cout << "  kiểm tra trước: " << muon_doc << " >= size()="
                  << diem.size() << " nên không đọc\n";
    }

    std::cout << "19 - Bound checking với .at(): mọi truy cập đều được kiểm tra, "
              << "chỉ số sai bị bắt chứ không bị bỏ qua\n";

    return 0;
}
