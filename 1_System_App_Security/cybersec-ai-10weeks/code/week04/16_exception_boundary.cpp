// Tuần 04 · Bài 16: Exception boundary.
// Mục tiêu: dựng ranh giới bắt ngoại lệ ở đúng chỗ, và mang được ngoại lệ từ
//   luồng con về luồng chính thay vì để nó giết cả chương trình.
// Đầu vào: các công việc giả lập, trong đó có việc cố tình ném ngoại lệ.
// Đầu ra: kết quả từng việc, và ngoại lệ được báo cáo thay vì làm sập chương trình.
// An toàn: mọi luồng đều có ranh giới bắt; không ngoại lệ nào thoát khỏi thân
//   hàm luồng — nếu thoát, chuẩn quy định std::terminate và không cứu được.
// Phụ thuộc: C++17 và thư viện luồng. Trên Linux thêm -pthread.

#include <exception>  // std::exception_ptr, std::current_exception, std::rethrow_exception
#include <iostream>   // std::cout
#include <stdexcept>  // std::runtime_error
#include <string>     // std::string
#include <thread>     // std::thread
#include <vector>     // std::vector

// Công việc có thể hỏng. Nó KHÔNG tự bắt lỗi: việc quyết định làm gì khi hỏng
// thuộc về tầng gọi, không thuộc về tầng tính toán.
int xu_ly(int id) {
    if (id == 2) throw std::runtime_error("bản ghi " + std::to_string(id) + " hỏng");
    return id * 10;
}

// Thân hàm của luồng LÀ ranh giới. Một ngoại lệ thoát khỏi đây không lan sang
// luồng chính như trong mã một luồng — nó gọi thẳng std::terminate. Vì vậy mọi
// hàm luồng đều phải có catch(...) bao ngoài, không có ngoại lệ nào cho quy tắc này.
void than_luong(int id, int* ket_qua, std::exception_ptr* loi) {
    try {
        *ket_qua = xu_ly(id);
    } catch (...) {
        // Đóng gói ngoại lệ hiện tại để luồng chính ném lại sau. Đây là cách
        // duy nhất mang trọn thông tin lỗi qua ranh giới luồng — chép lấy chuỗi
        // what() sẽ mất kiểu ngoại lệ và mọi dữ liệu đi kèm.
        *loi = std::current_exception();
    }
}

int main() {
    constexpr int SO_VIEC = 4;
    std::vector<int> ket_qua(SO_VIEC, 0);
    std::vector<std::exception_ptr> loi(SO_VIEC);  // mặc định là con trỏ rỗng

    std::vector<std::thread> nhom;
    nhom.reserve(SO_VIEC);
    for (int i = 0; i < SO_VIEC; ++i) {
        nhom.emplace_back(than_luong, i, &ket_qua[static_cast<std::size_t>(i)],
                          &loi[static_cast<std::size_t>(i)]);
    }
    for (std::thread& t : nhom) t.join();  // join TRƯỚC khi đọc kết quả hay lỗi

    int thanh_cong = 0, that_bai = 0;
    for (int i = 0; i < SO_VIEC; ++i) {
        const std::size_t k = static_cast<std::size_t>(i);
        if (loi[k]) {
            // Ném lại rồi bắt ngay để lấy đúng kiểu và thông điệp gốc.
            try {
                std::rethrow_exception(loi[k]);
            } catch (const std::exception& e) {
                ++that_bai;
                std::cout << "  việc " << i << " HỎNG: " << e.what() << '\n';
            }
        } else {
            ++thanh_cong;
            std::cout << "  việc " << i << " xong, kết quả=" << ket_qua[k] << '\n';
        }
    }

    // Một việc hỏng không kéo theo ba việc còn lại. Đó là điều ranh giới ngoại
    // lệ mua được: hỏng cục bộ thay vì sập toàn cục.
    std::cout << "16 - Exception boundary: " << thanh_cong << " thành công, " << that_bai
              << " thất bại, chương trình vẫn kết thúc bình thường\n";

    return 0;
}
