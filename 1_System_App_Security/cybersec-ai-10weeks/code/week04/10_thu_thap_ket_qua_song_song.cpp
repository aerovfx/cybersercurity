// Tuần 04 · Bài 10: Thu thập kết quả song song.
// Mục tiêu: gom kết quả từ nhiều luồng bằng cách cho mỗi luồng một ô riêng rồi
//   cộng lại sau — nhanh hơn hẳn việc tranh nhau một biến chung có khoá.
// Đầu vào: danh sách sự kiện giả lập chia đều cho các luồng.
// Đầu ra: kết quả gom theo hai cách, và so sánh cho thấy chúng bằng nhau.
// An toàn: giai đoạn song song không có ô nhớ nào bị hai luồng cùng ghi; giai
//   đoạn gộp chạy sau khi đã join nên cũng không cần khoá.
// Phụ thuộc: C++17 và thư viện luồng. Trên Linux thêm -pthread.

#include <iostream>  // std::cout
#include <mutex>     // std::mutex, std::lock_guard: cho cách chậm để đối chiếu
#include <thread>    // std::thread
#include <vector>    // std::vector

constexpr int SO_LUONG = 4;
constexpr int MOI_LUONG = 20000;

// CÁCH CHẬM: mọi luồng cùng khoá một mutex để cộng vào một biến. Đúng, nhưng
// các luồng dành phần lớn thời gian xếp hàng chờ nhau — càng nhiều luồng càng
// chậm, đúng ngược với mục đích chạy song song.
long long tong_chung = 0;
std::mutex khoa_tong;

void gom_kieu_tranh_chap(int id) {
    for (int i = 0; i < MOI_LUONG; ++i) {
        std::lock_guard<std::mutex> lk(khoa_tong);
        tong_chung += (id + i) % 5;
    }
}

int main() {
    // --- Cách chậm, để có số mà so ---
    {
        std::vector<std::thread> nhom;
        nhom.reserve(SO_LUONG);
        for (int i = 0; i < SO_LUONG; ++i) nhom.emplace_back(gom_kieu_tranh_chap, i);
        for (std::thread& t : nhom) t.join();
    }

    // --- Cách nên dùng: mỗi luồng một ô riêng ---
    //
    // Vector được cấp phát đủ chỗ TRƯỚC khi luồng nào chạy. Nếu vừa chạy vừa
    // push_back thì vector có thể cấp phát lại và mọi luồng đang ghi sẽ ghi vào
    // vùng nhớ đã bị bỏ đi.
    std::vector<long long> ket_qua_rieng(SO_LUONG, 0);

    std::vector<std::thread> nhom;
    nhom.reserve(SO_LUONG);
    for (int i = 0; i < SO_LUONG; ++i) {
        nhom.emplace_back([&ket_qua_rieng, i] {
            long long cuc_bo = 0;  // cộng dồn trên biến cục bộ: nhanh nhất
            for (int k = 0; k < MOI_LUONG; ++k) cuc_bo += (i + k) % 5;
            ket_qua_rieng[static_cast<std::size_t>(i)] = cuc_bo;  // ghi đúng một lần
        });
    }
    for (std::thread& t : nhom) t.join();

    // Giai đoạn gộp chạy SAU khi mọi luồng đã kết thúc, nên nó là mã một luồng
    // bình thường và không cần bất kỳ cơ chế đồng bộ nào.
    long long tong_gom = 0;
    for (int i = 0; i < SO_LUONG; ++i) {
        std::cout << "  luồng " << i << " góp " << ket_qua_rieng[static_cast<std::size_t>(i)]
                  << '\n';
        tong_gom += ket_qua_rieng[static_cast<std::size_t>(i)];
    }

    std::cout << "10 - Thu thập kết quả song song: có khoá=" << tong_chung
              << ", ô riêng=" << tong_gom
              << ", khớp=" << (tong_chung == tong_gom ? "đúng" : "SAI") << '\n';

    return 0;
}
