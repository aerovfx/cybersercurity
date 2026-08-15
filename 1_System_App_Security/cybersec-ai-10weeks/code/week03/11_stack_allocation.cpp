// Tuần 03 · Bài 11: Stack allocation.
// Mục tiêu: thấy biến cục bộ sống trên stack có vòng đời tự động theo scope —
//   sinh ra khi vào, huỷ khi ra, không cần một dòng lệnh dọn dẹp nào.
// Đầu vào: không có; chương trình tự tạo đối tượng mẫu.
// Đầu ra: nhật ký thứ tự khởi tạo và huỷ, cho thấy huỷ theo chiều ngược lại.
// An toàn: không cấp phát heap, không rò rỉ; mọi thứ do trình biên dịch dọn.

#include <iostream>  // std::cout
#include <string>    // std::string

// Lớp chỉ để quan sát vòng đời: in ra khi sinh và khi huỷ.
class PhienLab {
public:
    // Hàm khởi tạo chạy khi đối tượng ra đời, tại đúng dòng khai báo nó.
    explicit PhienLab(std::string ten) : ten_(std::move(ten)) {
        std::cout << "    + mở phiên " << ten_ << '\n';
    }

    // Hàm huỷ chạy TỰ ĐỘNG khi đối tượng ra khỏi scope. Không ai gọi nó bằng
    // tay, và nó vẫn chạy kể cả khi hàm thoát sớm vì return hay vì ngoại lệ.
    ~PhienLab() { std::cout << "    - đóng phiên " << ten_ << '\n'; }

    const std::string& ten() const { return ten_; }

private:
    std::string ten_;
};

void mot_pham_vi_long_nhau() {
    PhienLab ngoai("ngoài");  // sinh ra ở đây
    {
        // Scope lồng bên trong: đối tượng này chết ở dấu } ngay dưới, sớm hơn
        // hẳn `ngoai`, dù cả hai cùng nằm trong một hàm.
        PhienLab trong("trong");
        std::cout << "    đang ở trong scope lồng, cả hai phiên cùng sống\n";
    }  // <- `trong` bị huỷ tại đây
    std::cout << "    ra khỏi scope lồng, chỉ còn phiên " << ngoai.ten() << '\n';
}  // <- `ngoai` bị huỷ tại đây

int main() {
    std::cout << "  bắt đầu main\n";
    mot_pham_vi_long_nhau();

    // Thứ tự huỷ là NGƯỢC với thứ tự sinh: cái tạo sau chết trước. Đó là điều
    // làm cho RAII (bài 15) hoạt động đúng khi các tài nguyên phụ thuộc nhau.
    PhienLab a("A");
    PhienLab b("B");

    std::cout << "11 - Stack allocation: không có new, không có delete, "
              << "vòng đời do scope quyết định\n";

    return 0;  // b huỷ trước, rồi tới a
}
