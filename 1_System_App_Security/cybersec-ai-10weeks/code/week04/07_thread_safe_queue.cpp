// Tuần 04 · Bài 07: Thread safe queue.
// Mục tiêu: gói hàng đợi, khoá và biến điều kiện vào MỘT lớp, để chỗ dùng không
//   còn cơ hội quên khoá — an toàn luồng trở thành thuộc tính của kiểu dữ liệu.
// Đầu vào: nhiều luồng cùng đẩy vào, nhiều luồng cùng lấy ra.
// Đầu ra: tổng số phần tử lấy ra so với số phần tử đã đẩy vào.
// An toàn: dữ liệu bên trong là private nên không thể truy cập vòng qua khoá;
//   có close() tường minh nên không luồng nào chờ vĩnh viễn.
// Phụ thuộc: C++17 và thư viện luồng. Trên Linux thêm -pthread.

#include <condition_variable>  // std::condition_variable
#include <iostream>            // std::cout
#include <mutex>               // std::mutex, std::unique_lock, std::lock_guard
#include <queue>               // std::queue
#include <string>              // std::string
#include <thread>              // std::thread
#include <vector>              // std::vector

// Bài 04 để dữ liệu và khoá thành hai biến toàn cục rời nhau — đúng, nhưng chỉ
// đúng chừng nào mọi chỗ dùng đều nhớ khoá. Đóng gói lại thì việc nhớ đó thành
// việc của lớp, một lần, thay vì của mọi lời gọi.
class HangDoiAnToan {
public:
    void day_vao(std::string gia_tri) {
        {
            std::lock_guard<std::mutex> lk(khoa_);
            if (da_dong_) return;  // nhánh lỗi: đẩy vào hàng đã đóng thì bỏ qua
            du_lieu_.push(std::move(gia_tri));
        }  // nhả khoá trước khi báo
        co_hang_.notify_one();
    }

    // Trả bool thay vì ném ngoại lệ khi hết hàng: "hàng đã đóng và rỗng" là kết
    // thúc bình thường của một worker, không phải sự cố.
    bool lay_ra(std::string& ra) {
        std::unique_lock<std::mutex> lk(khoa_);
        co_hang_.wait(lk, [this] { return !du_lieu_.empty() || da_dong_; });

        // Vét sạch hàng rồi mới chịu thoát, nếu không những phần tử đẩy vào ngay
        // trước close() sẽ bị bỏ rơi.
        if (du_lieu_.empty()) return false;

        ra = du_lieu_.front();
        du_lieu_.pop();
        return true;
    }

    // Đánh thức TẤT CẢ: mỗi worker đang chờ đều cần tự kiểm tra lại điều kiện.
    // notify_one ở đây sẽ để những worker còn lại ngủ mãi.
    void dong() {
        {
            std::lock_guard<std::mutex> lk(khoa_);
            da_dong_ = true;
        }
        co_hang_.notify_all();
    }

private:
    std::queue<std::string> du_lieu_;   // private: không ai với tới mà không qua khoá
    mutable std::mutex khoa_;
    std::condition_variable co_hang_;
    bool da_dong_ = false;
};

constexpr int SO_WORKER = 3;
constexpr int SO_VIEC = 12;

int main() {
    HangDoiAnToan hang;
    std::vector<int> dem_moi_worker(SO_WORKER, 0);  // mỗi worker một ô riêng: không cần khoá

    std::vector<std::thread> worker;
    worker.reserve(SO_WORKER);
    for (int i = 0; i < SO_WORKER; ++i) {
        worker.emplace_back([&hang, &dem_moi_worker, i] {
            std::string viec;
            while (hang.lay_ra(viec)) ++dem_moi_worker[static_cast<std::size_t>(i)];
        });
    }

    for (int i = 1; i <= SO_VIEC; ++i) hang.day_vao("việc-" + std::to_string(i));
    hang.dong();  // không đóng thì mọi worker chờ mãi và join() treo vĩnh viễn

    for (std::thread& t : worker) t.join();

    int tong = 0;
    for (int i = 0; i < SO_WORKER; ++i) {
        std::cout << "  worker " << i << " xử lý "
                  << dem_moi_worker[static_cast<std::size_t>(i)] << " việc\n";
        tong += dem_moi_worker[static_cast<std::size_t>(i)];
    }

    std::cout << "07 - Thread safe queue: đẩy vào " << SO_VIEC << ", lấy ra " << tong
              << ", khớp=" << (tong == SO_VIEC ? "đúng" : "SAI") << '\n';

    return 0;
}
