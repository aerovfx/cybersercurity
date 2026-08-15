// Tuần 03 · Bài 17: Tránh dangling pointer.
// Mục tiêu: nhận ra ba tình huống làm con trỏ trỏ vào đối tượng đã chết, và biết
//   cách viết lại để tình huống đó không xảy ra được.
// Đầu vào: dữ liệu giả lập cục bộ.
// Đầu ra: kết quả của các cách viết AN TOÀN đã thay thế cho từng tình huống.
// An toàn: bài này KHÔNG giải tham chiếu con trỏ treo lần nào — hành vi không
//   xác định không thể "minh hoạ" được, vì kết quả của nó không đáng tin.

#include <iostream>  // std::cout
#include <string>    // std::string
#include <vector>    // std::vector

// TÌNH HUỐNG 1 — trả về địa chỉ của biến cục bộ.
//
//   const int* sai() { int x = 42; return &x; }   // x chết khi hàm return
//
// Cách đúng: trả về GIÁ TRỊ. Bản sao thuộc về người gọi nên luôn còn sống.
int cach_dung_1() {
    const int diem_cuc_bo = 42;
    return diem_cuc_bo;
}

// TÌNH HUỐNG 2 — giữ con trỏ vào phần tử vector rồi thêm phần tử.
//
// push_back có thể cấp phát vùng nhớ mới và chuyển toàn bộ phần tử sang đó; mọi
// con trỏ và iterator lấy trước đó lập tức thành treo. Nguy hiểm ở chỗ nó không
// phải lúc nào cũng xảy ra — chỉ khi vector cần nới — nên lỗi này chạy đúng
// hàng trăm lần rồi hỏng đúng lần thứ một nghìn.
//
// Cách đúng: giữ CHỈ SỐ, thứ vẫn còn nghĩa sau khi vector nới.
int cach_dung_2() {
    std::vector<int> diem{10, 20, 30};

    const std::size_t vi_tri = 1;  // giữ chỉ số, không giữ &diem[1]
    diem.push_back(40);            // sau dòng này, con trỏ cũ đã có thể vô hiệu

    return diem.at(vi_tri);  // lấy lại từ chỉ số: luôn đúng
}

// TÌNH HUỐNG 3 — dùng con trỏ sau khi delete.
//
// Cách đúng: đừng tự quản lý vòng đời. std::string ở đây sở hữu dữ liệu của nó
// và không có gì để delete, nên cũng không có gì để dùng nhầm sau khi delete.
std::string cach_dung_3() {
    std::string nhan = "LAB-";
    nhan += "017";
    return nhan;  // trả về giá trị; người gọi nhận một đối tượng còn sống
}

int main() {
    std::cout << "  1) trả về giá trị thay vì địa chỉ biến cục bộ: "
              << cach_dung_1() << '\n';
    std::cout << "  2) giữ chỉ số thay vì con trỏ vào phần tử vector: "
              << cach_dung_2() << '\n';
    std::cout << "  3) để đối tượng tự sở hữu dữ liệu, không delete thủ công: "
              << cach_dung_3() << '\n';

    // Thói quen bổ trợ: sau khi một con trỏ hết nhiệm vụ, gán nullptr. Nó không
    // sửa được lỗi thiết kế, nhưng biến một lỗi im lặng thành một lỗi mà nhánh
    // kiểm tra ở bài 16 bắt được.
    const int* quan_sat = nullptr;
    std::cout << "  con trỏ đã dọn: " << (quan_sat == nullptr ? "nullptr" : "còn trỏ") << '\n';

    std::cout << "17 - Tránh dangling pointer: 3 tình huống, 3 cách viết lại, "
              << "0 lần chạm vào vùng nhớ đã chết\n";

    return 0;
}
