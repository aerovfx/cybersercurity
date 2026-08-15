// Tuần 04 · Bài 08: Giới hạn số worker.
// Mục tiêu: hiểu vì sao số luồng nên bị giới hạn theo phần cứng thay vì tạo một
//   luồng cho mỗi việc, và cách chia việc cho một nhóm worker cố định.
// Đầu vào: danh sách việc giả lập nhiều hơn hẳn số worker.
// Đầu ra: số việc mỗi worker nhận, tổng việc, và số worker đã dùng.
// An toàn: chỉ số việc lấy qua std::atomic nên không có tranh chấp; số luồng bị
//   chặn trên nên không làm cạn tài nguyên hệ thống.
// Phụ thuộc: C++17 và thư viện luồng. Trên Linux thêm -pthread.

#include <algorithm>  // std::min
#include <atomic>     // std::atomic: con trỏ việc dùng chung
#include <iostream>   // std::cout
#include <thread>     // std::thread
#include <vector>     // std::vector

constexpr int SO_VIEC = 40;
constexpr unsigned TRAN_WORKER = 4;  // trần cứng, không phụ thuộc máy

int main() {
    // Mỗi việc một luồng là sai ở quy mô thật: 40 luồng cho 40 việc nghĩa là hệ
    // điều hành phải liên tục chuyển ngữ cảnh giữa chúng, và mỗi luồng còn tốn
    // vùng stack riêng. Quá số nhân vật lý, thêm luồng chỉ làm chậm đi.
    unsigned goi_y = std::thread::hardware_concurrency();
    if (goi_y == 0) goi_y = 2;  // nhánh lỗi: chuẩn cho phép trả 0 khi không biết

    const unsigned so_worker = std::min(goi_y, TRAN_WORKER);

    // Con trỏ việc dùng chung. fetch_add trả về chỉ số cũ và tăng lên trong một
    // thao tác, nên hai worker không bao giờ nhận trùng một việc.
    std::atomic<int> viec_tiep_theo{0};
    std::vector<int> dem(so_worker, 0);

    std::vector<std::thread> nhom;
    nhom.reserve(so_worker);

    for (unsigned w = 0; w < so_worker; ++w) {
        nhom.emplace_back([&viec_tiep_theo, &dem, w] {
            for (;;) {
                const int i = viec_tiep_theo.fetch_add(1);
                if (i >= SO_VIEC) return;  // hết việc: worker tự nghỉ
                ++dem[w];                  // ô riêng của worker này, không cần khoá
            }
        });
    }

    for (std::thread& t : nhom) t.join();

    int tong = 0;
    for (unsigned w = 0; w < so_worker; ++w) {
        std::cout << "  worker " << w << " nhận " << dem[w] << " việc\n";
        tong += dem[w];
    }

    std::cout << "08 - Giới hạn số worker: " << SO_VIEC << " việc chia cho " << so_worker
              << " worker (gợi ý phần cứng " << goi_y << ", trần " << TRAN_WORKER
              << "), tổng khớp=" << (tong == SO_VIEC ? "đúng" : "SAI") << '\n';

    return 0;
}
