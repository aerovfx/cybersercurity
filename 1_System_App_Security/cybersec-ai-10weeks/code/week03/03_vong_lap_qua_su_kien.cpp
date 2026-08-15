// Tuần 03 · Bài 03: Vòng lặp qua sự kiện.
// Mục tiêu: duyệt tuần tự một danh sách sự kiện sao cho mỗi phần tử được xử lý
//   đúng một lần, và chứng minh điều đó bằng bộ đếm chứ không bằng niềm tin.
// Đầu vào: danh sách sự kiện mạng giả lập viết sẵn trong mã.
// Đầu ra: số lần xuất hiện của từng giao thức và tổng số lượt xử lý.
// An toàn: dữ liệu lab cục bộ; không bắt gói tin thật, không chạm tới card mạng.

#include <cstddef>   // std::size_t
#include <iostream>  // std::cout
#include <map>       // std::map: đếm theo khoá, tự sắp xếp theo thứ tự khoá
#include <string>    // std::string
#include <vector>    // std::vector: danh sách sự kiện, số phần tử biết lúc chạy

int main() {
    // Sự kiện giả lập. Cố ý lặp "tcp" để phần đếm có việc để làm.
    const std::vector<std::string> su_kien{"tcp", "dns", "tcp", "icmp", "tcp", "dns"};

    std::map<std::string, int> dem_theo_giao_thuc;
    std::size_t luot_xu_ly = 0;  // bằng chứng: phải bằng đúng su_kien.size() ở cuối

    // Range-for duyệt từ đầu tới cuối, mỗi phần tử đúng một lượt. Không có biến
    // chỉ số nên không thể lỡ tay ++i hai lần hay bỏ sót phần tử cuối — hai lỗi
    // kinh điển của vòng lặp viết bằng chỉ số thủ công.
    for (const std::string& giao_thuc : su_kien) {
        ++dem_theo_giao_thuc[giao_thuc];  // khoá chưa có thì map tạo mới với giá trị 0
        ++luot_xu_ly;
    }

    // Duyệt map để in. const auto& tránh sao chép từng cặp khoá-giá trị.
    for (const auto& cap : dem_theo_giao_thuc) {
        std::cout << "  " << cap.first << " xuất hiện " << cap.second << " lần\n";
    }

    // Bất biến của bài: số lượt xử lý phải khớp số phần tử. Lệch nghĩa là vòng
    // lặp đã bỏ sót hoặc đếm trùng, và ta muốn biết ngay tại đây.
    const bool dung_mot_lan = (luot_xu_ly == su_kien.size());

    std::cout << "03 - Vòng lặp qua sự kiện: xử lý " << luot_xu_ly << "/"
              << su_kien.size() << " sự kiện, mỗi phần tử đúng một lần="
              << (dung_mot_lan ? "đúng" : "SAI") << '\n';

    return 0;
}
