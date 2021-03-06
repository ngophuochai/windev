# Thông tin cá nhân:
* **MSSV:** 1512138
* **Họ và tên:** Ngô Phước Hải
* **Email:** ngophuochai1512138@gmail.com

# Các chức năng làm được:
1. Đường thẳng (Line).
2. Hình chữ nhật (Rectangle). Giữ phím Shift sẽ vẽ hình vuông (Square).
3. Hình Ellipse. Giữ phím Shift sẽ vẽ hình tròn (Circle).
4. Cho phép chọn loại hình cần vẽ, màu vẽ, loại chổi tô.
6. Lưu và nạp hình. Lưu dưới dạng bitmap. Nạp dưới dạng png
7. Vẽ với GDI++.
8. Đưa một số hàm vào DLL.

# Các luồng sự kiện chính:
* Vẽ bằng cách nháy chọn một vị trí và kéo thả.
* Nháy vào nhóm Shapes để chọn hình, Size là kích thước chổi tô, Color để chọn màu.
* Khi vẽ với hình chữ nhật. Nếu giữ phím Shift sẽ vẽ hình vuông.
* Khi vẽ với hình Ellipse. Nếu giữ phím Shift sẽ vẽ hình tròn.
* Để nạp hình chọn File->Open. Để lưu hình chọn File->Save.

# Các luồng sự kiện phụ:
* Không vẽ được đường thẳng hay chéo khi giữ phím Shift.
* Lưu hình dưới dạng bitmap. Lưu cửa sổ đang vẽ với kích thước hiện hành.
* Khi nạp hình thì chỉ lưu kích thước hình đó.
* Chỉ lưu được hình với kích thước độ rộng (width) thỏa điều kiện: width % 4 == 0.
* Màu tô có giới hạn.

# Link repository:
https://github.com/ngophuochai/windev/tree/master/BTTuan07_Paint/1512138_Paint
# Link youtube:
https://youtu.be/3H3lUAF1s40