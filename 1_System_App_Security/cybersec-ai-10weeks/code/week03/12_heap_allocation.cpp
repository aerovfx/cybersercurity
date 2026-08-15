// Tuần 03 · Bài 12: Heap allocation.
// Mục tiêu: đối chiếu đối tượng trên heap với đối tượng cục bộ, và hiểu vì sao
//   cặp new/delete thủ công là nguồn rò rỉ bộ nhớ kinh điển.
// Đầu vào: không có; chương trình tự tạo đối tượng mẫu.
// Đầu ra: nhật ký vòng đời của bản trên stack và bản trên heap.
// An toàn: mọi new trong bài đều có delete tương ứng, và bản khuyến nghị dùng
//   std::unique_ptr để không còn phải nhớ điều đó.

#include <iostream>  // std::cout
#include <memory>    // std::unique_ptr, std::make_unique — cách làm được khuyến nghị
#include <string>    // std::string

class BoDem {
public:
    explicit BoDem(std::string ten) : ten_(std::move(ten)) {
        std::cout << "    + cấp phát " << ten_ << '\n';
    }
    ~BoDem() { std::cout << "    - thu hồi " << ten_ << '\n'; }

private:
    std::string ten_;
};

// Cách THỦ CÔNG, viết đúng — và vẫn mong manh.
//
// Mong manh vì delete chỉ chạy nếu luồng điều khiển đi tới được nó. Thêm một
// lệnh return sớm ở giữa, hoặc một hàm ném ngoại lệ, là đối tượng rò rỉ ngay —
// và trình biên dịch không hề cảnh báo. Đúng ở thời điểm viết không có nghĩa là
// còn đúng sau lần sửa thứ ba.
void cach_thu_cong() {
    BoDem* p = new BoDem("heap-thủ-công");  // xin vùng nhớ trên heap

    // ... phần xử lý ...  Mọi `return` chèn vào đây đều thành một chỗ rò rỉ.

    delete p;   // bắt buộc, và phải đúng một lần
    p = nullptr;  // đặt lại để không ai lỡ tay dùng con trỏ đã chết (bài 17)
}

// Cách ĐƯỢC KHUYẾN NGHỊ: vẫn nằm trên heap, nhưng quyền sở hữu do unique_ptr
// giữ, và nó gọi delete giúp — kể cả khi hàm thoát sớm hoặc có ngoại lệ.
void cach_tu_dong() {
    std::unique_ptr<BoDem> p = std::make_unique<BoDem>("heap-unique_ptr");
    // Không có delete ở đây. Không quên được, vì không có gì để quên.
}

int main() {
    // Trên stack: vòng đời gắn với scope, không liên quan gì tới heap.
    {
        BoDem cuc_bo("stack-cục-bộ");
    }

    cach_thu_cong();
    cach_tu_dong();

    std::cout << "12 - Heap allocation: heap cho phép sống lâu hơn scope, "
              << "cái giá là phải trả lại vùng nhớ — hãy để unique_ptr trả hộ\n";

    return 0;
}
