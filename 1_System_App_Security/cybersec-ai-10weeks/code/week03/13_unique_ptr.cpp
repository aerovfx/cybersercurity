// Tuần 03 · Bài 13: unique_ptr.
// Mục tiêu: nắm quyền sở hữu DUY NHẤT — một lúc chỉ một chủ — và việc giải phóng
//   tự động khi chủ sở hữu ra khỏi scope.
// Đầu vào: không có; chương trình tự tạo đối tượng mẫu.
// Đầu ra: nhật ký cấp phát, chuyển quyền sở hữu và thu hồi.
// An toàn: không new/delete thủ công; không thể sao chép nên không có double free.

#include <iostream>  // std::cout
#include <memory>    // std::unique_ptr, std::make_unique, std::move
#include <string>    // std::string
#include <utility>   // std::move

class KetNoiLab {
public:
    explicit KetNoiLab(std::string ten) : ten_(std::move(ten)) {
        std::cout << "    + mở " << ten_ << '\n';
    }
    ~KetNoiLab() { std::cout << "    - đóng " << ten_ << '\n'; }

    const std::string& ten() const { return ten_; }

private:
    std::string ten_;
};

// Nhận unique_ptr theo GIÁ TRỊ nghĩa là hàm này TIẾP QUẢN quyền sở hữu. Chữ ký
// hàm nói thẳng điều đó, nên người đọc không phải đoán ai chịu trách nhiệm dọn.
void tiep_quan(std::unique_ptr<KetNoiLab> ket_noi) {
    if (!ket_noi) {  // unique_ptr rỗng chuyển thành false
        std::cout << "    (không nhận được kết nối nào)\n";
        return;
    }
    std::cout << "    tiếp quản " << ket_noi->ten() << '\n';
}  // <- ra khỏi scope: đối tượng bị huỷ ngay tại đây

int main() {
    // make_unique cấp phát và bọc luôn trong một bước. Ưu tiên nó hơn
    // unique_ptr<T>(new T(...)) vì không để lộ ra một con trỏ thô nào ở giữa.
    std::unique_ptr<KetNoiLab> chinh = std::make_unique<KetNoiLab>("kết-nối-chính");

    std::cout << "  chinh đang giữ: " << chinh->ten() << '\n';

    // unique_ptr KHÔNG sao chép được — dòng `auto ban_sao = chinh;` sẽ không
    // biên dịch. Đó chính là điều ngăn hai chỗ cùng tưởng mình sở hữu rồi cùng
    // giải phóng một đối tượng.
    //
    // std::move nói rõ: chuyển quyền, không nhân bản.
    tiep_quan(std::move(chinh));

    // Sau khi move, `chinh` rỗng. Kiểm tra được, và đây là trạng thái hợp lệ —
    // không phải con trỏ treo.
    std::cout << "  sau move, chinh còn giữ gì không: "
              << (chinh ? "có" : "không") << '\n';

    // Gọi với một unique_ptr rỗng để chạy qua nhánh kiểm tra trong hàm.
    tiep_quan(nullptr);

    std::cout << "13 - unique_ptr: một chủ sở hữu tại một thời điểm, "
              << "giải phóng tự động, không viết delete lần nào\n";

    return 0;
}
