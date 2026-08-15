// Tuần 04 · Bài 09: Đo thời gian xử lý.
// Mục tiêu: đo thời gian bằng std::chrono::steady_clock và so sánh một việc chạy
//   tuần tự với chính nó chạy song song — đo thật, không đoán.
// Đầu vào: một khối lượng tính toán giả lập cố định.
// Đầu ra: thời gian mỗi cách tính, tỉ lệ tăng tốc, và kiểm tra hai kết quả khớp nhau.
// An toàn: mỗi luồng ghi vào ô kết quả riêng; không có dữ liệu chung cần khoá.
// Phụ thuộc: C++17 và thư viện luồng. Trên Linux thêm -pthread.

#include <chrono>    // std::chrono::steady_clock, duration_cast
#include <iostream>  // std::cout
#include <thread>    // std::thread
#include <vector>    // std::vector

constexpr int SO_PHAN = 4;
constexpr long long MOI_PHAN = 3000000;  // đủ lớn để phép đo có ý nghĩa

// Việc giả lập: cộng dồn một dãy. Trả về kết quả để trình biên dịch không được
// phép xoá bỏ vòng lặp vì cho rằng nó vô dụng.
long long lam_viec(long long tu, long long den) {
    long long tong = 0;
    for (long long i = tu; i < den; ++i) tong += i % 7;
    return tong;
}

int main() {
    // steady_clock chứ không phải system_clock: system_clock có thể bị NHẢY khi
    // hệ thống đồng bộ giờ mạng, và một phép đo khoảng thời gian dựa vào nó có
    // thể ra số âm. steady_clock chỉ tiến đều, đúng cho việc đo khoảng.
    const auto bat_dau_tuan_tu = std::chrono::steady_clock::now();

    long long tong_tuan_tu = 0;
    for (int p = 0; p < SO_PHAN; ++p) {
        tong_tuan_tu += lam_viec(p * MOI_PHAN, (p + 1) * MOI_PHAN);
    }

    const auto het_tuan_tu = std::chrono::steady_clock::now();

    // Song song: mỗi luồng ghi vào ô RIÊNG của nó trong vector kết quả. Không có
    // hai luồng nào chạm cùng một ô nên không cần khoá và không có tranh chấp.
    std::vector<long long> ket_qua(SO_PHAN, 0);
    const auto bat_dau_song_song = std::chrono::steady_clock::now();

    std::vector<std::thread> nhom;
    nhom.reserve(SO_PHAN);
    for (int p = 0; p < SO_PHAN; ++p) {
        nhom.emplace_back([&ket_qua, p] {
            ket_qua[static_cast<std::size_t>(p)] = lam_viec(p * MOI_PHAN, (p + 1) * MOI_PHAN);
        });
    }
    for (std::thread& t : nhom) t.join();  // phải join xong mới được đọc kết quả

    const auto het_song_song = std::chrono::steady_clock::now();

    long long tong_song_song = 0;
    for (const long long& r : ket_qua) tong_song_song += r;

    // duration_cast đổi sang đơn vị muốn đọc. Mili-giây là mức phù hợp ở đây.
    const auto ms_tuan_tu =
        std::chrono::duration_cast<std::chrono::milliseconds>(het_tuan_tu - bat_dau_tuan_tu).count();
    const auto ms_song_song =
        std::chrono::duration_cast<std::chrono::milliseconds>(het_song_song - bat_dau_song_song)
            .count();

    std::cout << "  tuần tự : " << ms_tuan_tu << " ms\n";
    std::cout << "  song song: " << ms_song_song << " ms (" << SO_PHAN << " luồng)\n";

    // Phép đo chỉ có nghĩa nếu hai cách ra cùng một kết quả. Nhanh hơn mà sai
    // thì không phải tối ưu, mà là hỏng.
    std::cout << "09 - Đo thời gian xử lý: kết quả khớp="
              << (tong_tuan_tu == tong_song_song ? "đúng" : "SAI")
              << ", nhanh hơn " << (ms_song_song > 0 ? (double)ms_tuan_tu / ms_song_song : 0.0)
              << " lần\n";

    return 0;
}
