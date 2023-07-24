from PIL import Image

# Đường dẫn tới file BMP
file_path = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/convert1.bmp"

# Mở file BMP
image = Image.open(file_path)

# Lấy thông tin về kích thước ảnh
width, height = image.size
print("Kích thước ảnh: {} x {}".format(width, height))

# Hiển thị ảnh
image.show()

# Đóng file
image.close()

# from PIL import Image

# file_path = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/rotated.bmp"

# try:
#     image = Image.open(file_path)
#     image.show()
# except Exception as e:
#     print("Lỗi khi đọc tệp hình ảnh:", e)

