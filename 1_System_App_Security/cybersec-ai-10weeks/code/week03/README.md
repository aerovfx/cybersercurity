---
layout: course
title: "Readme"
permalink: /1_System_App_Security/cybersec-ai-10weeks/code/week03/README.html
---

# Tuần 03 — 20 code minh họa

Mỗi khung bên dưới chứa mã nguồn C++17 hoàn chỉnh. Nhấn **COPY CODE** ở góc phải để sao chép toàn bộ đoạn code.

## 01_bien_va_kieu_du_lieu_c.cpp

**Chức năng:** Minh họa biến, kiểu số nguyên, chuỗi và container kích thước cố định trong C++.

```cpp
{% include_relative 01_bien_va_kieu_du_lieu_c.cpp %}
```

## 02_dieu_kien_phan_loai_rui_ro.cpp

**Chức năng:** Tổng hợp điểm làm dữ liệu đầu vào cho quy trình phân loại mức độ rủi ro.

```cpp
{% include_relative 02_dieu_kien_phan_loai_rui_ro.cpp %}
```

## 03_vong_lap_qua_su_kien.cpp

**Chức năng:** Duyệt tuần tự danh sách sự kiện và xử lý từng phần tử đúng một lần.

```cpp
{% include_relative 03_vong_lap_qua_su_kien.cpp %}
```

## 04_ham_tinh_diem.cpp

**Chức năng:** Minh họa lõi tính tổng điểm có thể tách thành hàm tái sử dụng.

```cpp
{% include_relative 04_ham_tinh_diem.cpp %}
```

## 05_std_array_co_dinh.cpp

**Chức năng:** Sử dụng `std::array` an toàn khi số phần tử đã biết trước.

```cpp
{% include_relative 05_std_array_co_dinh.cpp %}
```

## 06_std_vector_dong.cpp

**Chức năng:** Giới thiệu lựa chọn container động `std::vector` và đối chiếu với mảng cố định.

```cpp
{% include_relative 06_std_vector_dong.cpp %}
```

## 07_std_string_an_toan.cpp

**Chức năng:** Dùng `std::string` tự quản lý bộ nhớ thay cho mảng ký tự C dễ tràn bộ đệm.

```cpp
{% include_relative 07_std_string_an_toan.cpp %}
```

## 08_dia_chi_bien.cpp

**Chức năng:** Giải thích địa chỉ bộ nhớ và thời điểm cần hoặc không cần lấy địa chỉ của biến.

```cpp
{% include_relative 08_dia_chi_bien.cpp %}
```

## 09_con_tro_observer.cpp

**Chức năng:** Giới thiệu con trỏ observer không sở hữu và quy tắc không tự giải phóng đối tượng.

```cpp
{% include_relative 09_con_tro_observer.cpp %}
```

## 10_tham_chieu.cpp

**Chức năng:** Giới thiệu tham chiếu để truy cập dữ liệu mà không tạo bản sao không cần thiết.

```cpp
{% include_relative 10_tham_chieu.cpp %}
```

## 11_stack_allocation.cpp

**Chức năng:** Minh họa biến cục bộ trên stack và vòng đời tự động theo phạm vi hàm.

```cpp
{% include_relative 11_stack_allocation.cpp %}
```

## 12_heap_allocation.cpp

**Chức năng:** Đối chiếu heap với đối tượng cục bộ và giải thích rủi ro của `new`/`delete` thủ công.

```cpp
{% include_relative 12_heap_allocation.cpp %}
```

## 13_unique_ptr.cpp

**Chức năng:** Giới thiệu quyền sở hữu duy nhất và tự động giải phóng bằng `std::unique_ptr`.

```cpp
{% include_relative 13_unique_ptr.cpp %}
```

## 14_shared_ptr.cpp

**Chức năng:** Giới thiệu đồng sở hữu tài nguyên và bộ đếm tham chiếu của `std::shared_ptr`.

```cpp
{% include_relative 14_shared_ptr.cpp %}
```

## 15_raii_resource.cpp

**Chức năng:** Minh họa nguyên tắc RAII để tài nguyên được thu hồi tự động khi ra khỏi scope.

```cpp
{% include_relative 15_raii_resource.cpp %}
```

## 16_kiem_tra_nullptr.cpp

**Chức năng:** Nhắc quy tắc kiểm tra `nullptr` trước khi giải tham chiếu con trỏ.

```cpp
{% include_relative 16_kiem_tra_nullptr.cpp %}
```

## 17_tranh_dangling_pointer.cpp

**Chức năng:** Giải thích cách tránh giữ con trỏ tới đối tượng đã kết thúc vòng đời.

```cpp
{% include_relative 17_tranh_dangling_pointer.cpp %}
```

## 18_vector_thay_mang_c.cpp

**Chức năng:** Khuyến nghị container chuẩn thay cho mảng C thô để quản lý kích thước an toàn hơn.

```cpp
{% include_relative 18_vector_thay_mang_c.cpp %}
```

## 19_bound_checking_voi_at.cpp

**Chức năng:** Truy cập phần tử bằng `.at()` để phát hiện chỉ số vượt biên khi chạy.

```cpp
{% include_relative 19_bound_checking_voi_at.cpp %}
```

## 20_mini_memory_safety_lab.cpp

**Chức năng:** Tổng hợp các thói quen C++ an toàn về container, vòng đời và kiểm tra biên.

```cpp
{% include_relative 20_mini_memory_safety_lab.cpp %}
```
