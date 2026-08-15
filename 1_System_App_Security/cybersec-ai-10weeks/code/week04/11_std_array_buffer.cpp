// Tuần 04 · Bài 11: std::array buffer.
// Mục tiêu: dùng std::array làm bộ đệm kích thước cố định, với số phần tử luôn
//   đi kèm dữ liệu thay vì nằm trong một hằng số rời có thể lệch.
// Đầu vào: dữ liệu gói tin giả lập, có trường hợp ngắn hơn và dài hơn bộ đệm.
// Đầu ra: số byte thực sự chép được và trạng thái bộ đệm sau mỗi lần chép.
// An toàn: mọi lần ghi đều bị chặn bởi kích thước bộ đệm; không lần nào ghi tràn.

#include <algorithm>  // std::min, std::fill
#include <array>      // std::array
#include <cstddef>    // std::size_t
#include <iostream>   // std::cout
#include <string>     // std::string

constexpr std::size_t CO_BO_DEM = 8;

// Bộ đệm và số byte đang dùng đi thành một cặp. Trả về số byte đã chép để chỗ
// gọi biết dữ liệu có bị cắt hay không — thông tin mà memcpy() không hề cho.
std::size_t chep_an_toan(std::array<char, CO_BO_DEM>& bo_dem, const std::string& nguon) {
    // Dọn sạch trước: nếu không, phần đuôi còn sót dữ liệu của lần chép trước và
    // sẽ bị đọc nhầm thành nội dung hiện tại.
    std::fill(bo_dem.begin(), bo_dem.end(), '\0');

    // Đây là dòng quyết định. std::min chặn số byte chép ở kích thước bộ đệm,
    // nên nguồn dài bao nhiêu cũng không ghi ra ngoài. Trừ 1 để chừa chỗ cho ký
    // tự kết thúc chuỗi, thứ mà rất nhiều lỗi tràn quên mất.
    const std::size_t so_chep = std::min(nguon.size(), bo_dem.size() - 1);

    for (std::size_t i = 0; i < so_chep; ++i) bo_dem[i] = nguon[i];

    return so_chep;
}

void in_trang_thai(const std::array<char, CO_BO_DEM>& bo_dem, const std::string& nguon,
                   std::size_t da_chep) {
    std::cout << "  nguồn " << nguon.size() << " byte -> chép " << da_chep << " byte"
              << (da_chep < nguon.size() ? " (BỊ CẮT)" : "")
              << ", nội dung=\"" << bo_dem.data() << "\"\n";
}

int main() {
    std::array<char, CO_BO_DEM> bo_dem{};  // {} khởi tạo toàn bộ về 0

    // .size() lấy từ kiểu, luôn khớp với bộ đệm thật — không có hằng số rời để
    // quên cập nhật khi ai đó đổi CO_BO_DEM.
    std::cout << "  kích thước bộ đệm: " << bo_dem.size() << " byte\n";

    const std::string ngan = "tcp";
    in_trang_thai(bo_dem, ngan, chep_an_toan(bo_dem, ngan));

    // Nguồn dài hơn bộ đệm: đây chính là tình huống làm tràn bộ đệm nếu dùng
    // strcpy(). Ở đây nó chỉ bị cắt, và chỗ gọi được báo là đã cắt.
    const std::string dai = "dns-over-https-request";
    in_trang_thai(bo_dem, dai, chep_an_toan(bo_dem, dai));

    std::cout << "11 - std::array buffer: kích thước đi cùng dữ liệu, "
              << "mọi lần chép đều bị chặn ở " << bo_dem.size() << " byte\n";

    return 0;
}
