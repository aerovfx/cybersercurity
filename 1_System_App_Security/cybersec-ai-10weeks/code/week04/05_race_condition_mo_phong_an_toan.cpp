// Tuần 04 · Bài 05: Race condition mô phỏng an toàn.
// Mục tiêu: thấy CƠ CHẾ của lỗi mất cập nhật (lost update) và vì sao nó làm sai
//   kết quả, mà không tạo ra một tranh chấp dữ liệu thật.
// Đầu vào: không có; kịch bản xen kẽ được viết cố định để kết quả tái lập được.
// Đầu ra: kết quả của phiên bản mô phỏng hỏng, so với phiên bản có khoá.
// An toàn: bài này KHÔNG chạy tranh chấp dữ liệu thật. Data race là hành vi
//   không xác định — kết quả của nó không đáng tin để làm ví dụ dạy học, và
//   trình tối ưu hoá có quyền biến chương trình thành bất cứ thứ gì.
// Phụ thuộc: C++17 và thư viện luồng. Trên Linux thêm -pthread.

#include <atomic>    // std::atomic: bản sửa đúng
#include <iostream>  // std::cout
#include <thread>    // std::thread
#include <vector>    // std::vector

constexpr int SO_LUONG = 4;
constexpr int LAN_TANG = 1000;

// ---- Phần 1: MÔ PHỎNG lỗi, chạy trên MỘT luồng duy nhất -------------------
//
// `++dem` trên một biến thường không phải một bước. Trình biên dịch sinh ra ba
// bước tách rời: ĐỌC vào thanh ghi, CỘNG, GHI ngược lại. Hàm dưới đây viết tách
// ba bước đó ra và cố tình cho hai "luồng" giả xen vào giữa, đúng như bộ lập
// lịch của hệ điều hành có thể làm.
long long mo_phong_mat_cap_nhat() {
    long long dem = 0;

    for (int i = 0; i < LAN_TANG; ++i) {
        // "Luồng A" đọc giá trị hiện tại vào bản sao của riêng nó.
        const long long ban_sao_A = dem;

        // "Luồng B" chen vào: đọc CÙNG giá trị đó, cộng, rồi ghi.
        const long long ban_sao_B = dem;
        dem = ban_sao_B + 1;

        // "Luồng A" tỉnh lại và ghi bằng giá trị nó đọc từ trước — đè mất phần
        // việc của B. Hai lần tăng, kết quả chỉ nhích lên một.
        dem = ban_sao_A + 1;
    }
    return dem;  // sẽ là LAN_TANG chứ không phải 2 × LAN_TANG
}

// ---- Phần 2: cách viết ĐÚNG, chạy nhiều luồng thật ------------------------
std::atomic<long long> dem_an_toan{0};

void tang_an_toan() {
    for (int i = 0; i < LAN_TANG; ++i) dem_an_toan.fetch_add(1);
}

int main() {
    const long long ket_qua_mo_phong = mo_phong_mat_cap_nhat();
    std::cout << "  mô phỏng xen kẽ: 2×" << LAN_TANG << " lần tăng nhưng chỉ đếm được "
              << ket_qua_mo_phong << " — mất " << (2 * LAN_TANG - ket_qua_mo_phong)
              << " lần\n";

    std::vector<std::thread> nhom;
    nhom.reserve(SO_LUONG);
    for (int i = 0; i < SO_LUONG; ++i) nhom.emplace_back(tang_an_toan);
    for (std::thread& t : nhom) t.join();

    const long long mong_doi = static_cast<long long>(SO_LUONG) * LAN_TANG;
    std::cout << "  bản có std::atomic: " << dem_an_toan.load() << "/" << mong_doi << '\n';

    std::cout << "05 - Race condition mô phỏng an toàn: đọc-sửa-ghi là ba bước, "
              << "atomic gộp lại thành một; bản đúng khớp="
              << (dem_an_toan.load() == mong_doi ? "đúng" : "SAI") << '\n';

    return 0;
}
