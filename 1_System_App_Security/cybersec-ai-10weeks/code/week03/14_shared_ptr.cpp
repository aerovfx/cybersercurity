// Tuần 03 · Bài 14: shared_ptr.
// Mục tiêu: hiểu đồng sở hữu và bộ đếm tham chiếu — tài nguyên sống tới khi chủ
//   sở hữu CUỐI CÙNG buông tay, không sớm hơn và không muộn hơn.
// Đầu vào: không có; chương trình tự tạo đối tượng mẫu.
// Đầu ra: giá trị use_count() ở từng thời điểm và lúc đối tượng bị thu hồi.
// An toàn: không new/delete thủ công; đối tượng chỉ bị huỷ đúng một lần.

#include <iostream>  // std::cout
#include <memory>    // std::shared_ptr, std::make_shared
#include <string>    // std::string
#include <vector>    // std::vector: một chủ sở hữu thứ hai để đếm tăng lên

class BangQuyTac {
public:
    explicit BangQuyTac(std::string ten) : ten_(std::move(ten)) {
        std::cout << "    + nạp " << ten_ << '\n';
    }
    ~BangQuyTac() { std::cout << "    - giải phóng " << ten_ << '\n'; }

    const std::string& ten() const { return ten_; }

private:
    std::string ten_;
};

// Nhận const shared_ptr& để ĐỌC mà không làm bộ đếm nhúc nhích. Nếu nhận theo
// giá trị, mỗi lần gọi sẽ tăng rồi giảm bộ đếm — đúng nhưng tốn công vô ích.
void doc_ten(const std::shared_ptr<BangQuyTac>& bang) {
    if (!bang) {  // nhánh lỗi: shared_ptr rỗng vẫn là trường hợp hợp lệ
        std::cout << "    (chưa có bảng quy tắc)\n";
        return;
    }
    std::cout << "    đang đọc " << bang->ten()
              << ", số chủ sở hữu=" << bang.use_count() << '\n';
}

int main() {
    // make_shared cấp phát đối tượng và khối đếm trong một lần xin bộ nhớ.
    std::shared_ptr<BangQuyTac> chu_1 = std::make_shared<BangQuyTac>("quy-tắc-SOC");
    std::cout << "  sau khi tạo: use_count=" << chu_1.use_count() << '\n';

    {
        // Sao chép shared_ptr là hợp lệ và làm tăng bộ đếm — khác hẳn unique_ptr.
        std::shared_ptr<BangQuyTac> chu_2 = chu_1;
        std::cout << "  thêm chủ thứ hai: use_count=" << chu_1.use_count() << '\n';
        doc_ten(chu_2);

        // Chủ sở hữu thứ ba nằm trong một container.
        std::vector<std::shared_ptr<BangQuyTac>> kho{chu_1};
        std::cout << "  thêm chủ trong vector: use_count=" << chu_1.use_count() << '\n';
    }  // <- chu_2 và vector ra khỏi scope: bộ đếm giảm, đối tượng CHƯA bị huỷ

    std::cout << "  ra khỏi scope: use_count=" << chu_1.use_count()
              << " (đối tượng vẫn sống vì chu_1 còn giữ)\n";

    doc_ten(nullptr);

    std::cout << "14 - shared_ptr: đối tượng chỉ bị giải phóng khi chủ cuối cùng "
              << "buông, use_count hiện tại=" << chu_1.use_count() << '\n';

    return 0;  // chu_1 ra khỏi scope, bộ đếm về 0, huỷ đúng một lần
}
