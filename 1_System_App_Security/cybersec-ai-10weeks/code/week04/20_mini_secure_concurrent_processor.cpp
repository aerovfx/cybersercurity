// Tuần 04 · Bài 20: Mini secure concurrent processor.
// Mục tiêu: ghép các bài trong tuần thành một bộ xử lý sự kiện nhỏ — nhóm worker
//   có trần, hàng đợi an toàn luồng, kiểm tra đầu vào, và ranh giới ngoại lệ.
// Đầu vào: danh sách sự kiện giả lập, cố tình có bản ghi hỏng và bản ghi quá dài.
// Đầu ra: thống kê hợp lệ / bị từ chối / lỗi, và kiểm tra tổng số khớp đầu vào.
// An toàn: dữ liệu chung chỉ chạm qua khoá hoặc atomic; đầu vào được kiểm tra
//   trước khi xử lý; mọi luồng có catch bao ngoài; mọi luồng đều được join.
// Phụ thuộc: C++17 và thư viện luồng. Trên Linux thêm -pthread.

#include <algorithm>           // std::min
#include <atomic>              // std::atomic
#include <condition_variable>  // std::condition_variable
#include <exception>           // std::exception_ptr
#include <iostream>            // std::cout
#include <mutex>               // std::mutex, std::lock_guard, std::unique_lock
#include <queue>               // std::queue
#include <stdexcept>           // std::runtime_error
#include <string>              // std::string
#include <thread>              // std::thread
#include <vector>              // std::vector

constexpr std::size_t DAI_TOI_DA = 16;  // bài 12: trần độ dài nhãn
constexpr unsigned TRAN_WORKER = 4;     // bài 08: trần số luồng

// Bài 07: hàng đợi tự lo khoá, chỗ dùng không có cơ hội quên.
class HangViec {
public:
    void day(std::string v) {
        {
            std::lock_guard<std::mutex> lk(khoa_);
            q_.push(std::move(v));
        }
        co_.notify_one();
    }
    bool lay(std::string& ra) {
        std::unique_lock<std::mutex> lk(khoa_);
        co_.wait(lk, [this] { return !q_.empty() || dong_; });
        if (q_.empty()) return false;  // vét sạch rồi mới thoát
        ra = q_.front();
        q_.pop();
        return true;
    }
    void dong() {
        {
            std::lock_guard<std::mutex> lk(khoa_);
            dong_ = true;
        }
        co_.notify_all();  // đánh thức TẤT CẢ worker, không phải một
    }

private:
    std::queue<std::string> q_;
    std::mutex khoa_;
    std::condition_variable co_;
    bool dong_ = false;
};

// Bài 12: kiểm tra trước khi xử lý; rỗng và quá dài đều bị từ chối.
bool hop_le(const std::string& s) { return !s.empty() && s.size() <= DAI_TOI_DA; }

// Bài 16: công việc có thể ném; nó không tự bắt, tầng trên mới quyết định.
int cham_diem(const std::string& s) {
    if (s == "loi-mo-phong") throw std::runtime_error("bản ghi gây lỗi: " + s);
    return static_cast<int>(s.size()) * 3;
}

int main() {
    unsigned goi_y = std::thread::hardware_concurrency();
    if (goi_y == 0) goi_y = 2;
    const unsigned so_worker = std::min(goi_y, TRAN_WORKER);

    HangViec hang;

    // Bài 03: đếm bằng atomic thay vì khoá — mỗi bộ đếm chỉ là một số.
    std::atomic<int> hop_le_dem{0}, tu_choi_dem{0}, loi_dem{0}, tong_diem{0};

    // Bài 16: mỗi worker một ô lỗi riêng, không tranh chấp.
    std::vector<std::exception_ptr> loi(so_worker);

    std::vector<std::thread> worker;
    worker.reserve(so_worker);
    for (unsigned w = 0; w < so_worker; ++w) {
        worker.emplace_back([&hang, &hop_le_dem, &tu_choi_dem, &loi_dem, &tong_diem, &loi, w] {
            std::string viec;
            while (hang.lay(viec)) {
                if (!hop_le(viec)) {  // nhánh từ chối: chưa xử lý gì cả
                    tu_choi_dem.fetch_add(1);
                    continue;
                }
                // Ranh giới ngoại lệ nằm TRONG vòng lặp: một bản ghi hỏng không
                // được phép kết liễu cả worker và bỏ lại phần việc còn lại.
                try {
                    tong_diem.fetch_add(cham_diem(viec));
                    hop_le_dem.fetch_add(1);
                } catch (...) {
                    loi_dem.fetch_add(1);
                    if (!loi[w]) loi[w] = std::current_exception();  // giữ lỗi đầu tiên
                }
            }
        });
    }

    const std::vector<std::string> dau_vao{
        "port-scan", "brute-force", "", "loi-mo-phong",
        "chuoi-nhan-qua-dai-khong-hop-le", "dns-tunnel", "sql-probe",
    };
    for (const std::string& s : dau_vao) hang.day(s);
    hang.dong();  // không đóng thì worker chờ mãi và join() treo

    for (std::thread& t : worker) t.join();  // join hết trước khi đọc kết quả

    // Báo cáo lỗi đầu tiên gặp được, có ngữ cảnh thật thay vì chỉ một con số.
    for (unsigned w = 0; w < so_worker; ++w) {
        if (!loi[w]) continue;
        try {
            std::rethrow_exception(loi[w]);
        } catch (const std::exception& e) {
            std::cout << "  worker " << w << " báo lỗi: " << e.what() << '\n';
        }
    }

    const int tong_da_xu_ly = hop_le_dem.load() + tu_choi_dem.load() + loi_dem.load();

    std::cout << "  hợp lệ=" << hop_le_dem.load() << ", từ chối=" << tu_choi_dem.load()
              << ", lỗi=" << loi_dem.load() << ", tổng điểm=" << tong_diem.load() << '\n';

    std::cout << "20 - Mini secure concurrent processor: " << so_worker << " worker, "
              << tong_da_xu_ly << "/" << dau_vao.size() << " bản ghi được xử lý, khớp="
              << (tong_da_xu_ly == static_cast<int>(dau_vao.size()) ? "đúng" : "SAI") << '\n';

    return 0;
}
