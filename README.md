# CyberLearn

Trang học tập cho bộ giáo trình an toàn thông tin 10 tuần, gồm 5 lộ trình: Ethical Cybersecurity, Mật mã học ứng dụng, Reverse Engineering, Cybersecurity & AI và Cisco ASA Firewall.

## Truy cập trang học tập

**[Mở CyberLearn trên GitHub Pages →](https://aerovfx.github.io/cybersercurity/)**

### Công cụ lớp học

- [Cổng lớp học](https://aerovfx.github.io/cybersercurity/tools/khao-sat/portal.html)
- [Khảo sát học viên](https://aerovfx.github.io/cybersercurity/tools/khao-sat/index.html)
- [Đánh giá đồng đẳng](https://aerovfx.github.io/cybersercurity/tools/khao-sat/danh-gia.html)
- [Dashboard kết quả](https://aerovfx.github.io/cybersercurity/tools/khao-sat/ket-qua.html)
- [Chấm điểm giáo viên](https://aerovfx.github.io/cybersercurity/tools/khao-sat/admin.html)

Các công cụ lưu dữ liệu trong trình duyệt theo mặc định. Giáo viên có thể cấu hình Google Apps Script của riêng mình để đồng bộ tùy chọn với Google Sheets.

## Chạy local

Không cần cài dependency. Mở `index.html` trực tiếp hoặc chạy một static server:

```bash
python3 -m http.server 8080
```

Sau đó truy cập `http://localhost:8080`.

## Triển khai GitHub Pages

Trong repository, vào **Settings → Pages**, chọn **Deploy from a branch**, sau đó chọn nhánh cần phát hành và thư mục `/ (root)`.

## Nguyên tắc an toàn

Mọi bài thực hành chỉ được thực hiện trên hệ thống thuộc quyền sở hữu hoặc đã được ủy quyền rõ ràng. Ưu tiên môi trường lab offline, read-only hoặc dry-run.
