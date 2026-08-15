"""Tuần 09 · Bài 06: Phát hiện weak hash.

Ví dụ phòng thủ, chạy offline với dữ liệu giả lập; không quét hay gọi dịch vụ ngoài.
"""
# Khởi tạo danh sách dữ liệu mẫu
records = [{"id": "evt-01", "score": 20}, {"id": "evt-02", "score": 80}]
# Tạo danh sách kết quả (list comprehension)
result = [item for item in records if item["score"] >= 50]
# Kiểm tra điều kiện (assertion)
assert result is not None
# In kết quả ra màn hình
print("06 - Phát hiện weak hash:", result)
