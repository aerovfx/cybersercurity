// Tuần 03 · Bài 18: Vector thay mảng C.
// Mục tiêu: thấy vì sao container chuẩn an toàn hơn mảng C thô — kích thước đi
//   cùng dữ liệu, thay vì nằm trong một biến rời có thể lệch bất cứ lúc nào.
// Đầu vào: cùng một tập dữ liệu, viết bằng hai cách để so sánh trực tiếp.
// Đầu ra: tổng tính được theo mỗi cách và số phần tử mà mỗi cách tự biết.
// An toàn: mảng C ở đây chỉ để đối chiếu, luôn duyệt trong biên; ưu tiên vector.

#include <cstddef>   // std::size_t
#include <iostream>  // std::cout
#include <numeric>   // std::accumulate: cộng dồn không cần viết vòng lặp bằng tay
#include <vector>    // std::vector

// CÁCH CŨ: mảng C thô suy biến thành con trỏ khi truyền đi, nên hàm buộc phải
// nhận thêm tham số `n`. Không có gì bắt `n` phải đúng — truyền nhầm số lớn hơn
// là đọc ra ngoài mảng, và chương trình vẫn chạy, vẫn in ra một con số trông
// hợp lý. Đây là hình dạng của rất nhiều lỗi bảo mật ngoài đời thật.
int tong_mang_c(const int* du_lieu, std::size_t n) {
    int tong = 0;
    for (std::size_t i = 0; i < n; ++i) tong += du_lieu[i];
    return tong;
}

// CÁCH MỚI: vector mang theo kích thước của chính nó. Không có tham số `n` để
// truyền sai, và hàm không thể bị lừa đọc quá phần dữ liệu thật.
int tong_vector(const std::vector<int>& du_lieu) {
    return std::accumulate(du_lieu.begin(), du_lieu.end(), 0);
}

int main() {
    // Mảng C thô: kích thước nằm ở nơi khác, trong đầu lập trình viên.
    const int mang_c[5] = {22, 53, 80, 443, 8080};
    const std::size_t so_phan_tu = 5;  // phải tự nhớ, và tự cập nhật khi sửa mảng

    // Vector: thêm bớt phần tử thì .size() tự đúng theo, không cần sửa chỗ nào khác.
    const std::vector<int> vec{22, 53, 80, 443, 8080};

    std::cout << "  mảng C : tổng=" << tong_mang_c(mang_c, so_phan_tu)
              << ", số phần tử phải truyền tay=" << so_phan_tu << '\n';
    std::cout << "  vector : tổng=" << tong_vector(vec)
              << ", số phần tử tự biết=" << vec.size() << '\n';

    // Vector còn làm được việc mà mảng C không làm được: đổi kích thước lúc chạy.
    std::vector<int> them = vec;
    them.push_back(9200);
    std::cout << "  sau push_back: size=" << them.size()
              << ", tổng mới=" << tong_vector(them) << '\n';

    std::cout << "18 - Vector thay mảng C: cùng dữ liệu, nhưng chỉ một cách "
              << "không thể truyền sai kích thước\n";

    return 0;
}
