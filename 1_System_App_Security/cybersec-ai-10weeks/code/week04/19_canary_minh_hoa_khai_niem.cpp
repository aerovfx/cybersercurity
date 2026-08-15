// Tuần 04 · Bài 19: Canary — minh họa khái niệm.
// Mục tiêu: hiểu ý tưởng stack canary — đặt một giá trị chứng ngay sau vùng đệm,
//   kiểm tra trước khi trả về, và coi giá trị đó bị đổi là dấu hiệu ghi tràn.
// Đầu vào: các chuỗi giả lập, có chuỗi dài hơn vùng đệm.
// Đầu ra: kết quả kiểm tra canary sau mỗi lần ghi, và số lần ghi bị chặn.
// An toàn: bài này MÔ PHỎNG khái niệm bằng một cấu trúc dữ liệu bình thường.
//   Nó KHÔNG ghi tràn thật — ghi tràn là hành vi không xác định, không "minh
//   họa" được, và canary thật do trình biên dịch sinh ra (-fstack-protector),
//   không phải thứ viết bằng tay trong mã nguồn.

#include <algorithm>  // std::min, std::fill
#include <array>      // std::array
#include <cstddef>    // std::size_t
#include <cstdint>    // std::uint32_t
#include <iostream>   // std::cout
#include <string>     // std::string
#include <vector>     // std::vector

constexpr std::size_t CO_DEM = 8;
constexpr std::uint32_t GIA_TRI_CHUNG = 0xDEADBEEFu;  // giá trị chứng đã biết trước

// Khung mô phỏng: vùng đệm, rồi ngay sau nó là ô chứng. Trong chương trình thật,
// trình biên dịch chèn ô này vào khung stack; ở đây ta đặt tường minh để nhìn thấy.
struct KhungCoChung {
    std::array<char, CO_DEM> dem{};
    std::uint32_t chung = GIA_TRI_CHUNG;
};

// Ghi CÓ CHẶN BIÊN. Ghi tràn bị ngăn ngay từ đầu, nên ô chứng không bao giờ bị
// chạm tới — đó là điều ta muốn thấy ở một hàm viết đúng.
bool ghi_co_chan(KhungCoChung& k, const std::string& nguon) {
    std::fill(k.dem.begin(), k.dem.end(), '\0');

    if (nguon.size() >= k.dem.size()) {
        // Từ chối trước khi ghi. Đây mới là lớp phòng thủ chính; canary chỉ là
        // lưới an toàn cuối cùng cho những chỗ lớp này bị thiếu.
        return false;
    }
    for (std::size_t i = 0; i < nguon.size(); ++i) k.dem[i] = nguon[i];
    return true;
}

// Kiểm tra ô chứng — tương ứng với đoạn mã trình biên dịch chèn ngay trước lệnh
// return của hàm. Nếu giá trị đã đổi thì khung stack đã bị ghi đè, và chương
// trình thật sẽ DỪNG NGAY thay vì trả về một địa chỉ có thể đã bị kẻ tấn công
// thay đổi.
bool chung_con_nguyen(const KhungCoChung& k) { return k.chung == GIA_TRI_CHUNG; }

int main() {
    KhungCoChung khung;
    const std::vector<std::string> dau_vao{"tcp", "dns-req", "chuoi-qua-dai-cho-dem"};

    int da_ghi = 0, bi_chan = 0;
    for (const std::string& s : dau_vao) {
        const bool ok = ghi_co_chan(khung, s);
        if (ok) ++da_ghi; else ++bi_chan;

        std::cout << "  ghi \"" << s << "\" (" << s.size() << " byte) -> "
                  << (ok ? "OK" : "BỊ CHẶN")
                  << ", canary " << (chung_con_nguyen(khung) ? "còn nguyên" : "ĐÃ BỊ ĐỔI")
                  << '\n';
    }

    std::cout << "  giá trị chứng: 0x" << std::hex << khung.chung << std::dec << '\n';

    std::cout << "19 - Canary minh họa khái niệm: " << da_ghi << " lần ghi, " << bi_chan
              << " lần bị chặn, canary còn nguyên — bật thật bằng -fstack-protector-strong\n";

    return 0;
}
