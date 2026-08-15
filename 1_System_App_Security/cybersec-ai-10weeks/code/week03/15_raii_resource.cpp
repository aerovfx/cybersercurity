// Tuần 03 · Bài 15: RAII resource.
// Mục tiêu: buộc vòng đời tài nguyên vào vòng đời một đối tượng, để tài nguyên
//   được thu hồi kể cả khi hàm thoát sớm hoặc ném ngoại lệ.
// Đầu vào: không có; tài nguyên ở đây là một "khoá lab" giả lập, không phải file thật.
// Đầu ra: nhật ký chiếm và nhả tài nguyên trên cả ba đường thoát khỏi hàm.
// An toàn: không mở file/socket thật; không cần dọn dẹp thủ công ở chỗ gọi.

#include <iostream>   // std::cout
#include <stdexcept>  // std::runtime_error: minh hoạ đường thoát bằng ngoại lệ
#include <string>     // std::string

// RAII: Resource Acquisition Is Initialization — chiếm tài nguyên trong hàm khởi
// tạo, nhả trong hàm huỷ. Nhờ vậy "nhả" không còn là việc người dùng phải nhớ.
class KhoaLab {
public:
    explicit KhoaLab(std::string ten) : ten_(std::move(ten)) {
        std::cout << "    + chiếm khoá " << ten_ << '\n';
    }

    // Hàm huỷ chạy trên MỌI đường ra khỏi scope: return bình thường, return sớm,
    // hay ngoại lệ đang lan ngược lên. Đó là điều mà một lệnh nha_khoa() đặt ở
    // cuối hàm không bao giờ bảo đảm được.
    ~KhoaLab() { std::cout << "    - nhả khoá " << ten_ << '\n'; }

    // Cấm sao chép: hai đối tượng cùng tưởng mình giữ một khoá sẽ nhả hai lần.
    KhoaLab(const KhoaLab&) = delete;
    KhoaLab& operator=(const KhoaLab&) = delete;

private:
    std::string ten_;
};

// Đường ra 1: kết thúc bình thường.
void ket_thuc_binh_thuong() {
    KhoaLab k("bình-thường");
    std::cout << "    đang làm việc\n";
}

// Đường ra 2: return sớm giữa hàm. Không có dòng dọn dẹp nào ở đây.
void thoat_som(bool bo_qua) {
    KhoaLab k("thoát-sớm");
    if (bo_qua) {
        std::cout << "    bỏ qua, thoát sớm\n";
        return;  // khoá vẫn được nhả
    }
    std::cout << "    xử lý đầy đủ\n";
}

// Đường ra 3: ngoại lệ. Đây là đường mà cách dọn dẹp thủ công hay bỏ sót nhất.
void nem_ngoai_le() {
    KhoaLab k("có-ngoại-lệ");
    throw std::runtime_error("lỗi giả lập trong lab");
}

int main() {
    ket_thuc_binh_thuong();
    thoat_som(true);

    // try/catch để chương trình kết thúc gọn gàng; khoá đã được nhả TRƯỚC khi
    // luồng điều khiển tới được khối catch này.
    try {
        nem_ngoai_le();
    } catch (const std::runtime_error& e) {
        std::cout << "    bắt được ngoại lệ: " << e.what() << '\n';
    }

    std::cout << "15 - RAII resource: khoá được nhả trên cả ba đường thoát, "
              << "không viết lệnh dọn dẹp nào ở chỗ gọi\n";

    return 0;
}
