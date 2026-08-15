// Tuần 03 · Bài 06: std::vector động.
// Mục tiêu: dùng std::vector khi số phần tử chỉ biết lúc chạy, và đối chiếu với
//   std::array để thấy khi nào nên chọn cái nào.
// Đầu vào: danh sách port quét được, thêm dần từng phần tử trong lúc chạy.
// Đầu ra: số phần tử và sức chứa sau mỗi lần thêm, cùng kết quả lọc.
// An toàn: bộ nhớ do vector tự cấp phát và tự thu hồi; không new/delete thủ công.

#include <array>     // std::array: để so sánh với vector
#include <iostream>  // std::cout
#include <vector>    // std::vector: container tăng giảm được lúc chạy

int main() {
    // std::array: số phần tử CỐ ĐỊNH, quyết định lúc viết code. Không thêm bớt
    // được, đổi lại không bao giờ phải cấp phát lại vùng nhớ.
    const std::array<int, 3> port_biet_truoc{22, 80, 443};

    // std::vector: bắt đầu rỗng, dài ra theo dữ liệu thực tế. Đây là lựa chọn
    // đúng khi số phần tử phụ thuộc đầu vào — điều mà array không làm được.
    std::vector<int> port_quet_duoc;

    // reserve() báo trước sức chứa cần dùng. Không bắt buộc, nhưng nếu biết
    // trước quy mô thì nó tránh được vài lần cấp phát lại và sao chép dữ liệu.
    port_quet_duoc.reserve(4);

    // Thêm dần. size() là số phần tử ĐANG có; capacity() là chỗ đã xin sẵn.
    // Hai con số này khác nhau, và nhầm lẫn giữa chúng là hiểu sai về vector.
    for (const int p : {8080, 53, 3306, 5432}) {
        port_quet_duoc.push_back(p);
        std::cout << "  thêm " << p << " -> size=" << port_quet_duoc.size()
                  << ", capacity=" << port_quet_duoc.capacity() << '\n';
    }

    // Lọc ra port cao (>= 1024). Vector đích cũng tự lo bộ nhớ cho chính nó.
    std::vector<int> port_cao;
    for (const int& p : port_quet_duoc) {
        if (p >= 1024) port_cao.push_back(p);
    }

    std::cout << "06 - std::vector động: cố định " << port_biet_truoc.size()
              << " phần tử, động " << port_quet_duoc.size() << " phần tử, trong đó "
              << port_cao.size() << " port >= 1024\n";

    // Không có delete ở đây, và đó là điểm chính: vector giải phóng vùng nhớ của
    // nó khi ra khỏi scope, kể cả khi hàm thoát sớm vì một ngoại lệ.
    return 0;
}
