import cv2

# Đường dẫn đến tệp ảnh JPG
jpg_path = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/picture10.jpg"

# Đọc ảnh từ tệp JPG
image = cv2.imread(jpg_path)

if image is None:
    print("Không thể đọc ảnh!")
    exit()

# Đường dẫn đến tệp ảnh BMP đầu ra
bmp_path = 'picture_bmp.bmp'

# Chuyển đổi ảnh sang định dạng BMP và lưu vào tệp
cv2.imwrite(bmp_path, image)

print("Đã chuyển đổi ảnh thành công sang định dạng BMP!")
