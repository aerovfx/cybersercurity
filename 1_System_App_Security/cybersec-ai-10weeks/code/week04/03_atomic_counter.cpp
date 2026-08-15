// Tuần 04 · Bài 03: Atomic counter.
// Mục tiêu: dùng std::atomic để nhiều luồng cùng tăng một bộ đếm mà kết quả vẫn
//   chính xác, không cần khoá.
// Đầu vào: số luồng và số lần tăng mỗi luồng, đặt sẵn trong mã.
// Đầu ra: giá trị đếm được so với giá trị đúng theo lý thuyết.
// An toàn: mọi truy cập chung đều qua std::atomic; không có tranh chấp dữ liệu.
// Phụ thuộc: C++17 và thư viện luồng. Trên Linux thêm -pthread.

#include <atomic>    // std::atomic: đọc-sửa-ghi không thể bị chen ngang
#include <iostream>  // std::cout
#include <thread>    // std::thread
#include <vector>    // std::vector

// Cấu hình của bài. Đặt thành hằng để kết quả đúng có thể tính trước và so sánh.
constexpr int SO_LUONG = 4;
constexpr int LAN_TANG = 50000;

// std::atomic<long long>: phép ++ trên nó là MỘT thao tác không chia cắt được.
// Với một biến long long thường, `++x` thật ra là ba bước (đọc, cộng, ghi) và
// hai luồng có thể cùng đọc một giá trị rồi cùng ghi đè lên nhau — mất cập nhật.
std::atomic<long long> dem_atomic{0};

void tang_atomic() {
    for (int i = 0; i < LAN_TANG; ++i) {
        // fetch_add tăng và trả về giá trị cũ, tất cả trong một thao tác nguyên
        // tử. Toán tử ++ cũng làm đúng như vậy; viết rõ ra cho dễ đọc.
        dem_atomic.fetch_add(1);
    }
}

int main() {
    std::vector<std::thread> nhom;
    nhom.reserve(SO_LUONG);

    // Tạo các luồng; tất cả cùng đập vào một biến đếm.
    for (int i = 0; i < SO_LUONG; ++i) nhom.emplace_back(tang_atomic);

    // Chờ tất cả xong TRƯỚC khi đọc kết quả. Đọc sớm thì chỉ đọc được số dở dang.
    for (std::thread& t : nhom) t.join();

    const long long dung_ra_phai_la = static_cast<long long>(SO_LUONG) * LAN_TANG;
    const long long thuc_te = dem_atomic.load();

    std::cout << "  " << SO_LUONG << " luồng × " << LAN_TANG << " lần tăng\n";
    std::cout << "  đúng ra phải là: " << dung_ra_phai_la << '\n';
    std::cout << "  đếm được:        " << thuc_te << '\n';

    std::cout << "03 - Atomic counter: khớp=" << (thuc_te == dung_ra_phai_la ? "đúng" : "SAI")
              << ", không dùng mutex lần nào\n";

    return 0;
}
