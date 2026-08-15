"""Tuần 05 · Bài 13: Xếp hạng exposure.

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
print("13 - Xếp hạng exposure:", result)
