// Tuần 03 · Bài 07: std::string an toàn.
// Mục tiêu: dùng std::string tự quản lý bộ nhớ thay cho mảng ký tự kiểu C, thứ
//   đứng sau phần lớn lỗi tràn bộ đệm trong phần mềm thật.
// Đầu vào: các chuỗi mẫu viết sẵn trong mã (nhãn cảnh báo giả lập).
// Đầu ra: chuỗi sau khi ghép, độ dài, kết quả tìm kiếm và cắt chuỗi.
// An toàn: không mảng char thô, không strcpy, không cần biết trước độ dài đệm.

#include <iostream>  // std::cout
#include <string>    // std::string

int main() {
    // std::string sở hữu vùng nhớ của chính nó và tự nới ra khi cần. Với mảng
    // char thô, lập trình viên phải tự đoán trước độ dài tối đa — đoán thiếu là
    // tràn bộ đệm, đoán thừa là lãng phí, và không có cách nào đoán đúng luôn.
    std::string canh_bao = "ALERT";

    // += nới chuỗi theo nhu cầu. Chỗ này với strcat() sẽ là một lỗi tràn nếu bộ
    // đệm đích không đủ chỗ, và strcat() không có cách nào biết điều đó.
    canh_bao += "-";
    canh_bao += "port-scan";

    // .size() luôn khớp với nội dung thật; không có biến độ dài riêng để lệch.
    std::cout << "  chuỗi=" << canh_bao << ", độ dài=" << canh_bao.size() << '\n';

    // .find() trả std::string::npos khi không thấy. So sánh với npos là nhánh
    // lỗi bắt buộc: nếu bỏ qua và đem giá trị trả về đi cắt chuỗi thì sẽ ném
    // std::out_of_range hoặc lấy nhầm đoạn.
    const std::size_t vi_tri = canh_bao.find("port");
    if (vi_tri == std::string::npos) {
        std::cout << "  không tìm thấy 'port' trong chuỗi\n";
    } else {
        // .substr() tạo chuỗi MỚI, không chỉnh sửa chuỗi gốc và không trả về
        // con trỏ vào vùng nhớ của nó — nên không có nguy cơ dùng sau khi hỏng.
        std::cout << "  tìm thấy 'port' ở vị trí " << vi_tri
                  << ", phần đuôi=" << canh_bao.substr(vi_tri) << '\n';
    }

    // So sánh bằng == đọc đúng nghĩa. Với char* thì == so sánh ĐỊA CHỈ chứ không
    // so sánh nội dung, một cái bẫy im lặng phải dùng strcmp mới tránh được.
    const bool dung_nhan = (canh_bao == "ALERT-port-scan");

    std::cout << "07 - std::string an toàn: nhãn khớp=" << (dung_nhan ? "đúng" : "sai")
              << ", không dùng mảng char thô lần nào\n";

    return 0;  // chuỗi tự giải phóng khi ra khỏi scope, không cần free()
}
