// Tuần 04 · Bài 04: Mutex lock_guard.
// Mục tiêu: bảo vệ dữ liệu KHÔNG nguyên tử (vector, map) bằng std::mutex, và
//   dùng std::lock_guard để khoá luôn được nhả đúng lúc.
// Đầu vào: danh sách sự kiện giả lập do nhiều luồng cùng ghi vào một sổ chung.
// Đầu ra: số bản ghi thu được so với số bản ghi mong đợi.
// An toàn: mọi truy cập vào dữ liệu chung đều nằm trong vùng khoá; không tự
//   gọi lock()/unlock() bằng tay nên không có đường thoát nào bỏ quên unlock.
// Phụ thuộc: C++17 và thư viện luồng. Trên Linux thêm -pthread.

#include <iostream>  // std::cout
#include <mutex>     // std::mutex, std::lock_guard
#include <string>    // std::string
#include <thread>    // std::thread
#include <vector>    // std::vector: dữ liệu chung cần bảo vệ

constexpr int SO_LUONG = 4;
constexpr int MOI_LUONG_GHI = 100;

// Dữ liệu chung và cái khoá bảo vệ nó. Đặt cạnh nhau có chủ đích: người đọc phải
// thấy ngay biến nào được bảo vệ bởi khoá nào.
std::vector<std::string> so_su_kien;
std::mutex khoa_so;

void ghi_su_kien(int id_luong) {
    for (int i = 0; i < MOI_LUONG_GHI; ++i) {
        const std::string ban_ghi = "luồng-" + std::to_string(id_luong) +
                                    "/sự-kiện-" + std::to_string(i);

        // std::atomic không dùng được ở đây: push_back có thể cấp phát lại toàn
        // bộ vector, một chuỗi thao tác dài mà không phép nguyên tử nào bao nổi.
        //
        // lock_guard khoá khi được tạo và nhả trong hàm huỷ — kể cả khi thân
        // vòng lặp thoát sớm hay ném ngoại lệ. Đó là lý do không nên tự gọi
        // khoa_so.lock()/unlock(): chỉ cần một đường thoát bị bỏ sót là treo cả
        // chương trình, và deadlock thì không có thông báo lỗi nào cả.
        std::lock_guard<std::mutex> khoa(khoa_so);
        so_su_kien.push_back(ban_ghi);
    }  // <- khoá được nhả tại đây, mỗi vòng lặp
}

int main() {
    std::vector<std::thread> nhom;
    nhom.reserve(SO_LUONG);
    for (int i = 0; i < SO_LUONG; ++i) nhom.emplace_back(ghi_su_kien, i);
    for (std::thread& t : nhom) t.join();

    const std::size_t mong_doi = static_cast<std::size_t>(SO_LUONG) * MOI_LUONG_GHI;

    std::cout << "  " << SO_LUONG << " luồng, mỗi luồng ghi " << MOI_LUONG_GHI << " bản ghi\n";
    std::cout << "  mong đợi=" << mong_doi << ", thu được=" << so_su_kien.size() << '\n';
    std::cout << "  bản ghi đầu tiên: " << so_su_kien.front() << '\n';

    std::cout << "04 - Mutex lock_guard: khớp="
              << (so_su_kien.size() == mong_doi ? "đúng" : "SAI")
              << ", 0 lần gọi unlock() bằng tay\n";

    return 0;
}
