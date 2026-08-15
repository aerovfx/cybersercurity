"""Tuần 10 · Bài 02: Chuẩn hóa event.

Ví dụ phòng thủ, chạy offline với dữ liệu giả lập; không quét hay gọi dịch vụ ngoài.
"""
# Import hàm/class từ thư viện collections
from collections import Counter
# Khởi tạo danh sách dữ liệu mẫu
records = ["tcp", "dns", "tcp", "icmp"]
# Gọi hàm để tính toán kết quả
result = dict(Counter(records))
# Kiểm tra điều kiện (assertion)
assert result is not None
# In kết quả ra màn hình
print("02 - Chuẩn hóa event:", result)
