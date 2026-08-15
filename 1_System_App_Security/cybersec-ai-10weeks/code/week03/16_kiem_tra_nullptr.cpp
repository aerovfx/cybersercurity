// Tuần 03 · Bài 16: Kiểm tra nullptr.
// Mục tiêu: luôn kiểm tra con trỏ trước khi giải tham chiếu, và hiểu vì sao
//   "không tìm thấy" là một kết quả bình thường chứ không phải một tai nạn.
// Đầu vào: bảng tra cứu quy tắc giả lập, tra bằng một mã có thật và một mã không có.
// Đầu ra: kết quả của cả hai lần tra, không lần nào làm sập chương trình.
// An toàn: không giải tham chiếu con trỏ chưa kiểm tra; nullptr được xử lý tường minh.

#include <iostream>  // std::cout
#include <string>    // std::string
#include <vector>    // std::vector: nơi thật sự chứa dữ liệu

struct QuyTac {
    std::string ma;
    std::string mo_ta;
};

// Trả về con trỏ tới quy tắc, hoặc nullptr nếu không có. Kiểu trả về nói rõ
// rằng "không có" là khả năng thực tế — người gọi buộc phải nghĩ tới nó.
const QuyTac* tim(const std::vector<QuyTac>& bang, const std::string& ma) {
    for (const QuyTac& q : bang) {
        if (q.ma == ma) return &q;
    }
    return nullptr;  // không tìm thấy: một kết quả hợp lệ, không phải lỗi
}

// Mọi lần dùng đều đi qua đây, nên chỗ kiểm tra chỉ viết một lần và không sót.
void in_quy_tac(const QuyTac* q, const std::string& ma_da_tim) {
    // Nếu bỏ nhánh này và viết thẳng q->mo_ta, chương trình sẽ giải tham chiếu
    // nullptr — hành vi không xác định, thường là sập ngay tại chỗ, và trên
    // một số hệ thống thì tệ hơn: chạy tiếp với dữ liệu rác.
    if (q == nullptr) {
        std::cout << "  không có quy tắc nào mang mã " << ma_da_tim << '\n';
        return;
    }
    std::cout << "  " << q->ma << ": " << q->mo_ta << '\n';
}

int main() {
    const std::vector<QuyTac> bang{
        {"LAB-001", "phát hiện quét cổng tuần tự trong lab"},
        {"LAB-002", "phát hiện đăng nhập sai liên tiếp"},
    };

    // Trường hợp tìm thấy.
    in_quy_tac(tim(bang, "LAB-002"), "LAB-002");

    // Trường hợp KHÔNG tìm thấy — đường đi quan trọng nhất của bài này.
    in_quy_tac(tim(bang, "LAB-999"), "LAB-999");

    // Con trỏ khởi tạo tường minh bằng nullptr, không để nó mang giá trị rác.
    const QuyTac* chua_gan = nullptr;
    in_quy_tac(chua_gan, "(chưa gán)");

    std::cout << "16 - Kiểm tra nullptr: 3 lần tra, 2 lần không có kết quả, "
              << "0 lần sập\n";

    return 0;
}
