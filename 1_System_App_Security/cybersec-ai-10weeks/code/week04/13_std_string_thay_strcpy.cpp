// Tuần 04 · Bài 13: std::string thay strcpy.
// Mục tiêu: thấy vì sao strcpy() không thể an toàn được, và std::string giải
//   quyết vấn đề đó bằng cách bỏ hẳn khâu người dùng phải đoán kích thước.
// Đầu vào: các chuỗi giả lập, có chuỗi dài hơn bộ đệm cố định.
// Đầu ra: kết quả ghép chuỗi bằng std::string, và bản chép có chặn biên để đối chiếu.
// An toàn: KHÔNG dùng strcpy/strcat ở đâu; bản dùng mảng char luôn chặn theo
//   kích thước đích và luôn tự kết thúc chuỗi.

#include <algorithm>  // std::min
#include <array>      // std::array: bộ đệm cố định để đối chiếu
#include <cstddef>    // std::size_t
#include <iostream>   // std::cout
#include <string>     // std::string

constexpr std::size_t CO_DEM = 10;

// CÁCH CŨ, viết cho ĐÚNG — và vẫn đầy chỗ để sai.
//
// strcpy(dich, nguon) chép tới khi gặp '\0' trong NGUỒN. Nó không hề biết đích
// lớn bao nhiêu, nên nguồn dài hơn đích là ghi đè thẳng ra ngoài — cơ chế nằm
// sau vô số lỗ hổng thực thi mã tuỳ ý. Bản dưới đây tự chặn biên bằng tay:
// đúng, nhưng mỗi lần dùng lại phải nhớ làm đủ ba việc (chặn, chừa chỗ '\0',
// báo bị cắt), và chỉ cần quên một lần là hỏng.
std::size_t chep_co_chan(std::array<char, CO_DEM>& dich, const std::string& nguon) {
    const std::size_t n = std::min(nguon.size(), dich.size() - 1);  // chừa 1 cho '\0'
    for (std::size_t i = 0; i < n; ++i) dich[i] = nguon[i];
    dich[n] = '\0';  // bắt buộc: chuỗi C không tự kết thúc giúp
    return n;
}

int main() {
    // CÁCH NÊN DÙNG: std::string tự lo kích thước. Không có bộ đệm để tràn, vì
    // không có bộ đệm nào do ta tự quản lý.
    std::string nhan = "alert";
    nhan += "-";                 // += tự nới, không cần biết trước tổng độ dài
    nhan += "port-scan";
    nhan.append("-2026");

    std::cout << "  std::string: \"" << nhan << "\" (" << nhan.size() << " byte, "
              << "không cần khai báo kích thước lần nào)\n";

    // Bản mảng char, để thấy sự khác biệt về công sức và rủi ro.
    std::array<char, CO_DEM> dem{};
    const std::size_t da_chep = chep_co_chan(dem, nhan);

    std::cout << "  mảng char " << CO_DEM << " byte: \"" << dem.data() << "\" — chép "
              << da_chep << "/" << nhan.size() << " byte"
              << (da_chep < nhan.size() ? " (BỊ CẮT)" : "") << '\n';

    // std::string còn cho các thao tác mà chuỗi C phải tự viết vòng lặp: so
    // sánh nội dung bằng ==, tìm kiếm, cắt chuỗi — đều biết độ dài của chính nó.
    const bool la_alert = (nhan.find("alert") == 0);

    std::cout << "13 - std::string thay strcpy: bắt đầu bằng \"alert\"="
              << (la_alert ? "đúng" : "sai") << ", 0 lần gọi strcpy/strcat\n";

    return 0;
}
