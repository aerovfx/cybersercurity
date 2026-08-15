// Tuần 03 · Bài 20: Mini memory safety lab.
// Mục tiêu: gộp các thói quen an toàn của cả tuần vào một luồng xử lý nhỏ —
//   container chuẩn, kiểm tra biên, kiểm tra nullptr, RAII và unique_ptr.
// Đầu vào: danh sách sự kiện bảo mật giả lập, cố tình có một bản ghi hỏng.
// Đầu ra: báo cáo phân loại, và nhật ký cho thấy tài nguyên được thu hồi đúng lúc.
// An toàn: toàn bộ dữ liệu là giả lập cục bộ; không new/delete thủ công, không
//   truy cập ngoài biên, không giải tham chiếu con trỏ chưa kiểm tra.

#include <iostream>   // std::cout
#include <memory>     // std::unique_ptr, std::make_unique
#include <stdexcept>  // std::out_of_range
#include <string>     // std::string
#include <vector>     // std::vector

struct SuKien {
    std::string ma;
    int diem;  // 0–100; giá trị ngoài thang là dữ liệu hỏng
};

// RAII (bài 15): phiên báo cáo tự đóng khi ra khỏi scope, kể cả khi có ngoại lệ.
class PhienBaoCao {
public:
    explicit PhienBaoCao(std::string ten) : ten_(std::move(ten)) {
        std::cout << "  [mở phiên " << ten_ << "]\n";
    }
    ~PhienBaoCao() { std::cout << "  [đóng phiên " << ten_ << "]\n"; }

    PhienBaoCao(const PhienBaoCao&) = delete;             // cấm sao chép:
    PhienBaoCao& operator=(const PhienBaoCao&) = delete;  // một phiên, một chủ

private:
    std::string ten_;
};

// Kiểm tra nullptr (bài 16): trả về observer, "không có" là kết quả hợp lệ.
const SuKien* nghiem_trong_nhat(const std::vector<SuKien>& ds) {
    const SuKien* cao_nhat = nullptr;
    for (const SuKien& s : ds) {
        if (s.diem < 0 || s.diem > 100) continue;  // bỏ qua bản ghi hỏng
        if (cao_nhat == nullptr || s.diem > cao_nhat->diem) cao_nhat = &s;
    }
    return cao_nhat;
}

int main() {
    // unique_ptr (bài 13): phiên nằm trên heap nhưng không ai phải nhớ delete.
    const std::unique_ptr<PhienBaoCao> phien =
        std::make_unique<PhienBaoCao>("mini-memory-safety-lab");

    // std::vector (bài 06, 18): kích thước đi cùng dữ liệu.
    const std::vector<SuKien> su_kien{
        {"LAB-001", 30}, {"LAB-002", 85}, {"LAB-003", 60}, {"LAB-004", -7},
    };

    int hop_le = 0, hong = 0;
    for (const SuKien& s : su_kien) {  // const&: duyệt không sao chép (bài 10)
        if (s.diem < 0 || s.diem > 100) {
            ++hong;
            std::cout << "  bỏ qua bản ghi hỏng: " << s.ma << " (điểm " << s.diem << ")\n";
            continue;  // nhánh lỗi: loại bỏ tường minh, không lặng lẽ tính vào
        }
        ++hop_le;
    }

    // Bound checking (bài 19): chỉ số ngoài biên bị bắt, không đọc trộm bộ nhớ.
    try {
        std::cout << "  thử đọc vị trí 10: " << su_kien.at(10).ma << '\n';
    } catch (const std::out_of_range&) {
        std::cout << "  vị trí 10 vượt biên, đã bị .at() chặn lại\n";
    }

    const SuKien* nang_nhat = nghiem_trong_nhat(su_kien);
    std::cout << "  nghiêm trọng nhất: "
              << (nang_nhat != nullptr ? nang_nhat->ma : std::string("(không có)")) << '\n';

    std::cout << "20 - Mini memory safety lab: " << hop_le << " bản ghi hợp lệ, "
              << hong << " bản ghi hỏng, 0 lần cấp phát thủ công\n";

    return 0;  // unique_ptr huỷ phiên; hàm huỷ của PhienBaoCao in dòng cuối cùng
}
