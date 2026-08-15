// Tuần 03 · Bài 02: Điều kiện phân loại rủi ro.
// Mục tiêu: dùng if / else if / else để ánh xạ một điểm số thành mức rủi ro, và
//   thấy vì sao thứ tự các nhánh quyết định kết quả.
// Đầu vào: danh sách điểm giả lập 0–100 viết sẵn trong mã.
// Đầu ra: mỗi điểm kèm mức rủi ro tương ứng, và số cảnh báo mức cao.
// An toàn: thuần tính toán trên dữ liệu cục bộ; không đọc đầu vào chưa kiểm chứng.

#include <array>     // std::array: danh sách điểm mẫu, số phần tử cố định
#include <iostream>  // std::cout
#include <string>    // std::string: nhãn mức rủi ro trả về

// Hàm phân loại. Trả std::string thay vì int để chỗ gọi đọc ra nghĩa ngay.
//
// Các nhánh xếp từ CAO xuống THẤP có chủ đích: nếu đảo ngược thứ tự thì điều
// kiện `diem >= 40` sẽ khớp trước và mọi điểm từ 40 trở lên đều bị gán "trung
// bình" — một lỗi phân loại im lặng, chương trình vẫn chạy và vẫn in ra kết quả.
std::string muc_rui_ro(int diem) {
    // Nhánh lỗi đứng TRƯỚC mọi nhánh phân loại: điểm âm không nằm trong thang
    // 0–100 nên là dữ liệu hỏng, không phải rủi ro thấp. Nếu để nó xuống cuối,
    // điều kiện `diem < 40` sẽ nuốt mất và -3 lặng lẽ thành "thấp".
    if (diem < 0 || diem > 100) {
        return "không hợp lệ";
    } else if (diem >= 80) {
        return "cao";
    } else if (diem >= 40) {
        return "trung bình";
    } else {
        // else cuối cùng bắt trọn phần còn lại (0–39), nên hàm luôn trả về một
        // giá trị trên mọi đường đi — không có nhánh nào rơi ra ngoài.
        return "thấp";
    }
}

int main() {
    // Dữ liệu giả lập, cố tình có một giá trị âm để chạy qua nhánh lỗi.
    const std::array<int, 5> diem_su_kien{12, 45, 80, 99, -3};

    int so_canh_bao_cao = 0;  // đếm riêng để in tổng kết ở cuối

    // Range-for: không cần chỉ số nên không có cơ hội viết sai chỉ số. const&
    // tránh sao chép và ngăn sửa nhầm phần tử đang duyệt.
    for (const int& diem : diem_su_kien) {
        const std::string muc = muc_rui_ro(diem);
        if (muc == "cao") ++so_canh_bao_cao;
        std::cout << "  điểm " << diem << " -> " << muc << '\n';
    }

    std::cout << "02 - Điều kiện phân loại rủi ro: "
              << so_canh_bao_cao << " cảnh báo mức cao trên "
              << diem_su_kien.size() << " sự kiện\n";

    return 0;
}
