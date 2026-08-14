// Tuần 03 · Bài 19: Bound checking với at.
// Mục tiêu: Dùng .at(i) để kiểm tra biên và ném std::out_of_range nếu chỉ số không hợp lệ.
// Lưu ý an toàn: chương trình chỉ minh họa trên dữ liệu cục bộ, không đọc đầu vào
// chưa kiểm chứng và không thực hiện cấp phát/giải phóng bộ nhớ thủ công.

// <array> cung cấp std::array: container kích thước cố định, biết rõ số phần tử.
// <iostream> cung cấp std::cout để ghi kết quả ra thiết bị đầu ra chuẩn.
// <string> cung cấp std::string, tự quản lý bộ nhớ chứa chuỗi ký tự.
#include <array>
#include <iostream>
#include <string>

int main() {
    // const khóa dữ liệu đầu vào sau khi khởi tạo, ngăn sửa nhầm trong lúc tính.
    // Tham số mẫu 3 là kích thước cố định; trình biên dịch kiểm tra kiểu của
    // cả ba phần tử đều là int.
    const std::array<int, 3> scores{19, 29, 39};

    // std::string sở hữu vùng nhớ của chính nó; không cần mảng char hoặc hàm
    // sao chép chuỗi kiểu C vốn dễ gây lỗi vượt quá kích thước bộ đệm.
    const std::string lesson = "Bound checking với at";

    // Biến tích lũy bắt đầu từ phần tử trung hòa của phép cộng là 0.
    int total = 0;

    // std::size_t là kiểu chỉ số phù hợp với giá trị do scores.size() trả về.
    // Điều kiện i < scores.size() bảo đảm vòng lặp dừng trước cuối container.
    // .at(i) kiểm tra biên khi chạy; nếu i sai, chương trình báo lỗi rõ ràng
    // thay vì âm thầm truy cập vùng nhớ ngoài phạm vi như toán tử [] có thể làm.
    for (std::size_t i = 0; i < scores.size(); ++i) {
        total += scores.at(i);  // Cộng điểm hiện tại vào tổng đã tính trước đó.
    }

    // Ghép mã bài, tên bài và tổng điểm; ký tự xuống dòng không buộc flush
    // bộ đệm như std::endl, phù hợp với đầu ra đơn giản này.
    std::cout << "19 - " << lesson << ": " << total << '\n';

    // Trả về 0 cho hệ điều hành để xác nhận chương trình kết thúc thành công.
    return 0;
}
