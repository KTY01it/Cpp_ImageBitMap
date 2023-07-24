import cv2

# Đường dẫn đến tệp BMP
file_path = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/pic_crop.bmp"

# Đọc tệp BMP
image = cv2.imread(file_path)

# Kiểm tra xem ảnh có được đọc thành công không
if image is not None:
    # Lấy kích thước ảnh
    height, width = image.shape[:2]
    print("Width:", width)
    print("Height:", height)
    print(image.shape)
    # Hiển thị ảnh
    cv2.imshow("BMP Image", image)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
else:
    print("Failed to read the BMP file.")
