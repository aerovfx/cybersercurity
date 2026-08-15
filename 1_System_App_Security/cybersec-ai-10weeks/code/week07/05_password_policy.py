"""Tuần 07 · Bài 05: Password policy.

Ví dụ phòng thủ, chạy offline với dữ liệu giả lập; không quét hay gọi dịch vụ ngoài.
"""
# Khởi tạo danh sách dữ liệu mẫu
records = ["allowed", "failed", "review"]
# Tạo danh sách kết quả (list comprehension)
result = [item for item in records if item != "allowed"]
# Kiểm tra điều kiện (assertion)
assert result is not None
# In kết quả ra màn hình
print("05 - Password policy:", result)
