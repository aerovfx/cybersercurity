// Tuần 04 · Bài 06: Producer consumer.
// Mục tiêu: hai luồng phối hợp qua một hàng đợi có giới hạn, dùng
//   std::condition_variable để chờ mà không đốt CPU.
// Đầu vào: số sự kiện cần sản xuất, đặt sẵn trong mã.
// Đầu ra: nhật ký sản xuất/tiêu thụ và tổng số sự kiện đã qua hàng đợi.
// An toàn: hàng đợi có TRẦN nên bên sản xuất nhanh không làm cạn bộ nhớ; có tín
//   hiệu kết thúc tường minh nên bên tiêu thụ không chờ mãi.
// Phụ thuộc: C++17 và thư viện luồng. Trên Linux thêm -pthread.

#include <condition_variable>  // std::condition_variable
#include <iostream>            // std::cout
#include <mutex>               // std::mutex, std::unique_lock, std::lock_guard
#include <queue>               // std::queue: hàng đợi FIFO
#include <string>              // std::string
#include <thread>              // std::thread

constexpr int SO_SU_KIEN = 8;
constexpr std::size_t TRAN_HANG_DOI = 3;  // trần: chống bên sản xuất chạy quá xa

std::queue<std::string> hang_doi;
std::mutex khoa;
std::condition_variable co_cho_trong;  // báo: hàng đợi vừa vơi
std::condition_variable co_hang_moi;   // báo: hàng đợi vừa có thêm
bool da_xong = false;                  // tín hiệu kết thúc, luôn sửa trong vùng khoá

void ben_san_xuat() {
    for (int i = 1; i <= SO_SU_KIEN; ++i) {
        std::unique_lock<std::mutex> lk(khoa);

        // wait() nhả khoá rồi ngủ, và chỉ tỉnh lại khi điều kiện đúng. Dạng có
        // vị từ (lambda) này xử lý luôn "spurious wakeup" — hệ điều hành được
        // phép đánh thức nhầm, và một vòng `if` đơn giản sẽ chạy tiếp với hàng
        // đợi vẫn đầy.
        co_cho_trong.wait(lk, [] { return hang_doi.size() < TRAN_HANG_DOI; });

        hang_doi.push("sự-kiện-" + std::to_string(i));
        std::cout << "    + sản xuất sự-kiện-" << i << " (hàng đợi=" << hang_doi.size() << ")\n";

        lk.unlock();          // nhả khoá TRƯỚC khi báo, để bên kia tỉnh dậy là vào được ngay
        co_hang_moi.notify_one();
    }

    // Báo kết thúc. Vẫn phải sửa cờ trong vùng khoá, nếu không bên tiêu thụ có
    // thể kiểm tra cờ ngay giữa lúc nó đang đổi.
    {
        std::lock_guard<std::mutex> lk(khoa);
        da_xong = true;
    }
    co_hang_moi.notify_all();  // đánh thức mọi người đang chờ để họ thấy cờ
}

void ben_tieu_thu(int* dem_ra) {
    for (;;) {
        std::unique_lock<std::mutex> lk(khoa);

        // Chờ tới khi CÓ hàng, hoặc bên sản xuất báo đã xong.
        co_hang_moi.wait(lk, [] { return !hang_doi.empty() || da_xong; });

        // Nhánh kết thúc: hết hàng VÀ đã có tín hiệu xong thì mới được thoát.
        // Kiểm tra da_xong trước khi vét sạch hàng đợi sẽ làm mất sự kiện cuối.
        if (hang_doi.empty() && da_xong) return;

        const std::string sk = hang_doi.front();
        hang_doi.pop();
        ++(*dem_ra);
        std::cout << "    - tiêu thụ " << sk << " (hàng đợi=" << hang_doi.size() << ")\n";

        lk.unlock();
        co_cho_trong.notify_one();  // báo cho bên sản xuất là đã vơi một chỗ
    }
}

int main() {
    int da_tieu_thu = 0;

    std::thread sx(ben_san_xuat);
    std::thread tt(ben_tieu_thu, &da_tieu_thu);

    sx.join();
    tt.join();

    std::cout << "06 - Producer consumer: sản xuất " << SO_SU_KIEN << ", tiêu thụ "
              << da_tieu_thu << ", trần hàng đợi " << TRAN_HANG_DOI << '\n';

    return 0;
}
