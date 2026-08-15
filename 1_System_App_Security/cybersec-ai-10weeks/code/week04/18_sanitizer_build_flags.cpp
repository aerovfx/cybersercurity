// Tuần 04 · Bài 18: Sanitizer build flags.
// Mục tiêu: biết các cờ sanitizer cần bật khi kiểm thử, và tự phát hiện chương
//   trình đang chạy dưới sanitizer nào.
// Đầu vào: không có; chương trình tự kiểm tra macro do trình biên dịch đặt.
// Đầu ra: danh sách cờ khuyến nghị và trạng thái sanitizer đang bật.
// An toàn: file này KHÔNG cố tình gây lỗi bộ nhớ để "trình diễn" sanitizer —
//   một lỗi thật trong tài liệu dạy học là thứ người học sẽ sao chép lại.
// Cách kiểm chứng:
//   c++ -std=c++17 -g -fsanitize=address,undefined 18_sanitizer_build_flags.cpp -o /tmp/demo

#include <iostream>  // std::cout
#include <vector>    // std::vector

// Clang dùng __has_feature; GCC đặt __SANITIZE_ADDRESS__. Kiểm tra cả hai để
// bài chạy đúng trên cả hai trình biên dịch.
#if defined(__has_feature)
#  if __has_feature(address_sanitizer)
#    define CO_ASAN 1
#  endif
#  if __has_feature(thread_sanitizer)
#    define CO_TSAN 1
#  endif
#endif
#if defined(__SANITIZE_ADDRESS__)
#  define CO_ASAN 1
#endif
#if defined(__SANITIZE_THREAD__)
#  define CO_TSAN 1
#endif

#ifndef CO_ASAN
#  define CO_ASAN 0
#endif
#ifndef CO_TSAN
#  define CO_TSAN 0
#endif

int main() {
    std::cout << "  Cờ nên bật khi kiểm thử:\n";
    std::cout << "    -fsanitize=address     : đọc/ghi ngoài biên, dùng sau khi giải phóng,\n";
    std::cout << "                             giải phóng hai lần, rò rỉ (trên nền hỗ trợ)\n";
    std::cout << "    -fsanitize=undefined   : tràn số nguyên có dấu, dịch bit quá cỡ,\n";
    std::cout << "                             giải tham chiếu null, con trỏ lệch canh\n";
    std::cout << "    -fsanitize=thread      : tranh chấp dữ liệu giữa các luồng\n";
    std::cout << "    -g                     : giữ tên hàm và số dòng trong báo cáo\n";
    std::cout << "    -fno-omit-frame-pointer: giữ vết gọi hàm đọc được\n";

    // ASan và TSan KHÔNG dùng chung được trong một lần biên dịch — chúng đều
    // thay thế lớp cấp phát bộ nhớ. Phải build hai lần, mỗi lần một loại.
    std::cout << "\n  Lưu ý: address và thread phải build RIÊNG, không gộp chung một lệnh.\n";
    std::cout << "  Sanitizer làm chương trình chậm vài lần — dùng khi kiểm thử,\n";
    std::cout << "  không dùng cho bản phát hành.\n";

    std::cout << "\n  Lần chạy này: AddressSanitizer=" << (CO_ASAN ? "BẬT" : "tắt")
              << ", ThreadSanitizer=" << (CO_TSAN ? "BẬT" : "tắt") << '\n';

    // Một đoạn xử lý bình thường, hoàn toàn hợp lệ. Nếu chạy bản có sanitizer,
    // nó sẽ đi qua đây mà không báo gì — đó chính là kết quả mong đợi.
    std::vector<int> v{1, 2, 3};
    v.push_back(4);
    int tong = 0;
    for (const int& x : v) tong += x;

    std::cout << "18 - Sanitizer build flags: tổng=" << tong
              << ", chạy sạch (không có lỗi nào để sanitizer báo)\n";

    return 0;
}
