// Tuần 04 · Bài 12: Kiểm tra độ dài input.
// Mục tiêu: kiểm tra độ dài TRƯỚC khi xử lý, và phân biệt hai cách xử lý dữ liệu
//   quá dài — cắt bớt hay từ chối — vì chọn nhầm là một lỗ hổng.
// Đầu vào: các chuỗi giả lập ngắn, vừa đúng, và dài quá mức cho phép.
// Đầu ra: quyết định chấp nhận hay từ chối cho từng đầu vào, kèm lý do.
// An toàn: không chuỗi nào được xử lý trước khi qua kiểm tra; không có ghi tràn.

#include <cstddef>   // std::size_t
#include <iostream>  // std::cout
#include <string>    // std::string
#include <vector>    // std::vector

constexpr std::size_t DAI_TOI_DA = 12;  // trần cho một nhãn cảnh báo

// Kết quả kiểm tra: nói rõ hợp lệ hay không VÀ vì sao. Một hàm chỉ trả bool sẽ
// buộc chỗ gọi tự đoán lý do, và thường là đoán sai khi viết thông báo lỗi.
struct KetQuaKiemTra {
    bool hop_le;
    std::string ly_do;
};

KetQuaKiemTra kiem_tra(const std::string& dau_vao) {
    // Rỗng cũng là dữ liệu không hợp lệ. Bỏ qua trường hợp này là cách một chuỗi
    // rỗng lọt xuống tầng dưới rồi gây lỗi ở nơi khó truy ngược.
    if (dau_vao.empty()) return {false, "rỗng"};

    // Kiểm tra độ dài TRƯỚC mọi thao tác khác. Đảo thứ tự — xử lý rồi mới kiểm
    // tra — là hình dạng kinh điển của lỗi tràn bộ đệm.
    if (dau_vao.size() > DAI_TOI_DA)
        return {false, "dài " + std::to_string(dau_vao.size()) + " > trần " +
                           std::to_string(DAI_TOI_DA)};

    // Danh sách cho phép (allowlist): chỉ chấp nhận ký tự đã liệt kê. An toàn hơn
    // danh sách cấm, vì thứ chưa nghĩ tới sẽ bị TỪ CHỐI thay vì được cho qua.
    for (const char& c : dau_vao) {
        const bool duoc_phep = (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-';
        if (!duoc_phep) return {false, std::string("ký tự không cho phép: '") + c + "'"};
    }

    return {true, "hợp lệ"};
}

int main() {
    const std::vector<std::string> dau_vao{
        "port-scan",              // vừa đúng
        "",                       // rỗng
        "dns-tunneling-detected", // quá dài
        "sql injection",          // có dấu cách, không nằm trong allowlist
        "brute-force",            // hợp lệ
    };

    int nhan = 0, tu_choi = 0;
    for (const std::string& s : dau_vao) {
        const KetQuaKiemTra kq = kiem_tra(s);
        if (kq.hop_le) ++nhan; else ++tu_choi;
        std::cout << "  \"" << s << "\" -> " << (kq.hop_le ? "NHẬN" : "TỪ CHỐI")
                  << " (" << kq.ly_do << ")\n";
    }

    // Với nhãn cảnh báo thì TỪ CHỐI là đúng: một nhãn bị cắt cụt vẫn trông hợp
    // lệ và sẽ âm thầm sai ở mọi chỗ dùng sau đó. Cắt bớt chỉ chấp nhận được khi
    // dữ liệu là văn bản hiển thị, nơi mất phần đuôi không đổi ý nghĩa.
    std::cout << "12 - Kiểm tra độ dài input: nhận " << nhan << ", từ chối " << tu_choi
              << "; chọn từ chối thay vì cắt bớt vì đây là định danh\n";

    return 0;
}
