"""Tuần 01 · Bài 13: Phân biệt TCP và UDP.

Ví dụ phòng thủ, chạy offline với dữ liệu giả lập; không quét hay gọi dịch vụ ngoài.
"""
# Import hàm/class từ thư viện dataclasses
from dataclasses import asdict, dataclass
# Khai báo dataclass
@dataclass(frozen=True)
# Định nghĩa class
class Finding:
    rule: str
    severity: str
# Gọi hàm để tính toán kết quả
result = asdict(Finding("LAB-RULE", "medium"))
# Kiểm tra điều kiện (assertion)
assert result is not None
# In kết quả ra màn hình
print("13 - Phân biệt TCP và UDP:", result)
