# Thông tin cá nhân:
* **MSSV:** 1512138
* **Họ và tên:** Ngô Phước Hải
* **Email:** ngophuochai1512138@gmail.com


# Các chức năng làm được:
1. Sử dụng Shell để lấy các đối tượng thư mục và tập tin của các ổ đĩa.
2. Thay đổi kích thước treeview thì listview thay đổi theo.
3. Bổ sung StatusBar khi click vào một tập tin trong listview thì hiển thị kích thước tập tin tương ứng.
4. Lưu lại kích thước cửa sổ màn hình chính và nạp lại khi chương trình chạy lên.


# Các luồng sự kiện chính:
* Khi khởi động chương trình. Thấy TreeView bên trái và ListView bên phải. Màn hình ListView mặc định là Desktop, chỉ xử lý phần "This PC".
* Kéo thả lề bên phải TreeView sẽ thay đổi kích thước 2 cửa sổ.
* Khi click vào một tập tin bên ListView sẽ hiển thị kích thước tương ứng ở bên dưới (StatusBar).
* Khi thay đổi kích thước cửa sổ chính (hWnd) thì sẽ lưu lại kích thước dưới dạng file INI với tên "appname.ini".
* Double click chuột vào một đối tượng bên ListView sẽ mở thư mục hay ổ đĩa hoặc có thể chạy file tương ứng.


# Các luồng sự kiện phụ:
* Thay đổi kích thước cửa sổ chính không làm thay đổi kích thước TreeView hay StatusBar.
* Cần Debug x64.


# Link repository: 
https://github.com/ngophuochai/windev/tree/master/BTTuan04_Fileexplorer
# Link youtube:
https://www.youtube.com/watch?v=LCFQ6htuQoI&feature=youtu.be