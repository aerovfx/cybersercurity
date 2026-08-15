// Tuần 04 · Bài 02: Join thread.
// Mục tiêu: hiểu vì sao MỌI std::thread đều phải được join() hoặc detach() trước
//   khi bị huỷ, và cách dùng RAII để không bao giờ quên việc đó.
// Đầu vào: không có; công việc là hàm giả lập viết sẵn.
// Đầu ra: trạng thái joinable() ở từng thời điểm và thứ tự kết thúc các luồng.
// An toàn: không detach luồng nào; mọi luồng đều được chờ trước khi hàm thoát;
//   std::cout được bảo vệ bằng khoá vì nhiều luồng cùng in.
// Phụ thuộc: C++17 và thư viện luồng. Trên Linux thêm -pthread.

#include <iostream>  // std::cout
#include <mutex>     // std::mutex, std::lock_guard: bảo vệ std::cout
#include <string>    // std::string: gom dòng in trước khi vào vùng khoá
#include <thread>    // std::thread
#include <utility>   // std::move
#include <vector>    // std::vector: giữ nhiều luồng cùng lúc

// Xem ghi chú ở bài 01: nhiều luồng cùng `<<` vào std::cout là data race thật,
// ThreadSanitizer bắt được.
//
// Bẫy đã đo được ở bản đầu của bài này: chỉ khoá phần in TRONG luồng con là
// chưa đủ. Luồng CHÍNH cũng in, và nó in trong lúc luồng con đang chạy — cũng
// là hai luồng cùng chạm std::cout. Mọi chỗ in có thể chồng lấn đều phải đi qua
// cùng một khoá, kể cả chỗ in của main.
std::mutex khoa_in;

void in_dong(const std::string& s) {
    std::lock_guard<std::mutex> k(khoa_in);
    std::cout << s << '\n';
}

void viec_ngan(int id) { in_dong("    luồng " + std::to_string(id) + " xong"); }

// RAII cho luồng (bài 15 tuần 03 áp dụng vào đây). C++20 có std::jthread làm sẵn
// việc này; ở C++17 ta tự bọc.
//
// Vì sao cần: nếu một std::thread còn joinable() mà bị huỷ, chuẩn quy định gọi
// std::terminate() — chương trình chết ngay, không phải rò rỉ âm thầm. Một
// `return` sớm hay một ngoại lệ giữa chừng là đủ để bỏ lỡ lệnh join() viết tay.
class ThreadGuard {
public:
    explicit ThreadGuard(std::thread t) : t_(std::move(t)) {}

    // Hàm huỷ chạy trên mọi đường thoát, nên join() không thể bị bỏ sót.
    ~ThreadGuard() {
        if (t_.joinable()) t_.join();
    }

    ThreadGuard(const ThreadGuard&) = delete;             // một luồng, một người chờ
    ThreadGuard& operator=(const ThreadGuard&) = delete;

private:
    std::thread t_;
};

int main() {
    // Cách viết tay: đúng, nhưng chỉ đúng khi luồng điều khiển đi tới được join().
    std::thread t(viec_ngan, 1);
    // Dòng này in TRONG LÚC luồng 1 có thể đang in — nên nó cũng phải qua khoá.
    in_dong(std::string("  trước join, joinable=") + (t.joinable() ? "có" : "không"));
    t.join();
    // Sau join() thì chỉ còn một luồng, in trực tiếp cũng an toàn — nhưng dùng
    // chung một lối in cho cả file thì không ai phải xét lại điều đó khi sửa sau.
    in_dong(std::string("  sau join,  joinable=") + (t.joinable() ? "có" : "không"));

    // Cách RAII: không có lệnh join nào trong thân hàm, mà vẫn bảo đảm join.
    {
        ThreadGuard g(std::thread(viec_ngan, 2));
        in_dong("  đã giao luồng 2 cho ThreadGuard");
    }  // <- join() xảy ra ở đây, kể cả khi khối này thoát vì ngoại lệ

    // Nhiều luồng: giữ trong vector rồi join lần lượt. Phải join TẤT CẢ, bỏ sót
    // một cái là std::terminate khi vector bị huỷ.
    std::vector<std::thread> nhom;
    for (int i = 3; i <= 5; ++i) nhom.emplace_back(viec_ngan, i);
    for (std::thread& mot_luong : nhom) {
        if (mot_luong.joinable()) mot_luong.join();
    }

    std::cout << "02 - Join thread: " << nhom.size() + 2
              << " luồng, tất cả đều được chờ, 0 lần detach\n";

    return 0;
}
