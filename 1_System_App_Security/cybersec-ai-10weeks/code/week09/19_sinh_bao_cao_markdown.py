"""Tuần 09 · Bài 19: Sinh báo cáo Markdown.

Ví dụ phòng thủ, chạy offline với dữ liệu giả lập; không quét hay gọi dịch vụ ngoài.
"""
# Khởi tạo từ điển
records = {"source": "classroom-fixture", "verified": False, "count": 3}
# Tạo từ điển kết quả
result = {key: records[key] for key in sorted(records)}
# Kiểm tra điều kiện (assertion)
assert result is not None
# In kết quả ra màn hình
print("19 - Sinh báo cáo Markdown:", result)
