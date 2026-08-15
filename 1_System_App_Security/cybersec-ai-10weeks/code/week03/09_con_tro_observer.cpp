// Tuần 03 · Bài 09: Con trỏ observer.
// Mục tiêu: dùng con trỏ thô như một "người quan sát" không sở hữu đối tượng, và
//   nắm quy tắc đi kèm: ai không sở hữu thì tuyệt đối không được giải phóng.
// Đầu vào: danh sách phát hiện giả lập do main sở hữu.
// Đầu ra: mô tả phần tử được quan sát, và trường hợp không tìm thấy.
// An toàn: observer chỉ đọc; không delete, không giữ con trỏ quá vòng đời chủ sở hữu.

#include <iostream>  // std::cout
#include <string>    // std::string
#include <vector>    // std::vector: chủ sở hữu thật sự của các phần tử

struct PhatHien {
    std::string ma;
    int diem;
};

// Nhận const PhatHien*: chỉ QUAN SÁT. Hàm này không cấp phát, không giải phóng,
// không cất con trỏ lại chỗ nào. Nó chỉ nhìn trong lúc được gọi.
//
// Quy tắc của observer: vòng đời do chủ sở hữu quyết định. Nếu hàm này gọi
// delete thì nó đang phá huỷ thứ nó chỉ được cho mượn, và vector ở main sẽ giải
// phóng lần thứ hai — lỗi double free.
void mo_ta(const PhatHien* quan_sat) {
    // Nhánh lỗi: observer hoàn toàn có thể là nullptr (không tìm thấy). Kiểm tra
    // trước khi giải tham chiếu là bắt buộc, không phải tuỳ chọn.
    if (quan_sat == nullptr) {
        std::cout << "  không có phát hiện nào để quan sát\n";
        return;
    }
    std::cout << "  quan sát " << quan_sat->ma << " (điểm " << quan_sat->diem << ")\n";
}

// Trả về observer tới phần tử trong vector, hoặc nullptr nếu không có.
// Con trỏ trả về CHỈ hợp lệ chừng nào `ds` còn sống và không bị thêm phần tử.
const PhatHien* tim_diem_cao_nhat(const std::vector<PhatHien>& ds) {
    if (ds.empty()) return nullptr;

    const PhatHien* cao_nhat = &ds.front();  // & lấy địa chỉ phần tử, không sao chép
    for (const PhatHien& p : ds) {
        if (p.diem > cao_nhat->diem) cao_nhat = &p;
    }
    return cao_nhat;
}

int main() {
    // main sở hữu dữ liệu. Vector cấp phát và sẽ tự giải phóng khi ra khỏi scope.
    const std::vector<PhatHien> ds{{"LAB-001", 30}, {"LAB-002", 85}, {"LAB-003", 60}};

    const PhatHien* cao_nhat = tim_diem_cao_nhat(ds);
    mo_ta(cao_nhat);

    // Trường hợp rỗng: hàm trả nullptr và observer xử lý đúng, không sập.
    const std::vector<PhatHien> rong{};
    mo_ta(tim_diem_cao_nhat(rong));

    std::cout << "09 - Con trỏ observer: đã quan sát "
              << (cao_nhat != nullptr ? cao_nhat->ma : std::string("(không có)"))
              << ", không lần nào gọi delete\n";

    return 0;  // vector giải phóng phần tử của nó; observer chỉ việc biến mất
}
