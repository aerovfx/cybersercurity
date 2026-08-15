// Tuần 04 · Bài 01: Khởi tạo std::thread.
// Mục tiêu: tạo một luồng chạy song song với luồng chính, và thấy rõ luồng bắt
//   đầu chạy NGAY khi std::thread được khởi tạo, không phải khi gọi join().
// Đầu vào: không có; công việc là hàm giả lập viết sẵn.
// Đầu ra: nhật ký từ luồng chính và luồng phụ, kèm số luồng phần cứng gợi ý.
// An toàn: mỗi luồng chỉ ghi vào dữ liệu riêng; std::cout được bảo vệ bằng khoá
//   (xem ghi chú ở khoa_in); join trước khi thoát hàm.
// Phụ thuộc: C++17 và thư viện luồng của hệ thống. Trên Linux thêm -pthread:
//   c++ -std=c++17 -pthread 01_khoi_tao_std_thread.cpp -o /tmp/demo

#include <chrono>    // std::chrono: mốc thời gian cho sleep_for
#include <iostream>  // std::cout
#include <mutex>     // std::mutex, std::lock_guard: bảo vệ std::cout
#include <string>    // std::string
#include <thread>    // std::thread, std::this_thread

// std::cout dùng chung giữa các luồng và KHÔNG tự bảo vệ trạng thái định dạng
// bên trong của nó. Đo được bằng ThreadSanitizer trên bản đầu của bài này: hai
// luồng cùng `<<` gây data race trên ios_base::width. Chuẩn C++ chỉ hứa dữ liệu
// không bị hỏng, không hứa các luồng ghi xen kẽ mà an toàn — nên cần khoá riêng.
std::mutex khoa_in;

// Công việc của luồng phụ. Nhận tham số theo giá trị: luồng có thể sống lâu hơn
// biến ở chỗ gọi, nên truyền tham chiếu vào đây là một cái bẫy vòng đời.
void cong_viec(std::string ten, int so_buoc) {
    for (int i = 1; i <= so_buoc; ++i) {
        {
            // Khoá chỉ bao quanh phần in, không bao quanh cả vòng lặp: giữ khoá
            // lâu hơn mức cần thiết sẽ biến hai luồng thành một luồng nối tiếp.
            std::lock_guard<std::mutex> k(khoa_in);
            std::cout << "    [" << ten << "] bước " << i << "/" << so_buoc << '\n';
        }
        // Ngủ một chút để hai luồng thật sự xen kẽ nhau, nếu không luồng phụ
        // thường chạy xong trước khi luồng chính kịp in dòng nào.
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    // Gợi ý số luồng chạy song song thật sự mà máy này có. Chỉ là GỢI Ý: chuẩn
    // cho phép trả về 0 khi không xác định được, nên phải xử lý trường hợp đó.
    const unsigned so_luong_goi_y = std::thread::hardware_concurrency();
    std::cout << "  số luồng phần cứng gợi ý: "
              << (so_luong_goi_y != 0 ? std::to_string(so_luong_goi_y) : "không xác định")
              << '\n';

    // Luồng BẮT ĐẦU CHẠY ngay tại dòng này, không phải ở join(). Đây là hiểu lầm
    // phổ biến nhất về std::thread.
    std::thread luong_phu(cong_viec, "phụ", 3);

    // Luồng chính vẫn chạy tiếp, song song với luồng phụ ở trên.
    cong_viec("chính", 3);

    // join() chờ luồng phụ kết thúc. Bắt buộc phải gọi (hoặc detach) trước khi
    // đối tượng std::thread bị huỷ, nếu không std::terminate sẽ giết chương
    // trình — chi tiết ở bài 02.
    luong_phu.join();

    std::cout << "01 - Khởi tạo std thread: luồng phụ chạy từ lúc tạo, "
              << "kết thúc khi join() trả về\n";

    return 0;
}
