import cv2

# Đường dẫn đến tệp BMP
bmp_file_path = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/cropped1.bmp"

# Đọc tệp BMP
image = cv2.imread(bmp_file_path)

# Kiểm tra xem ảnh có được đọc thành công không
if image is not None:
    # Đường dẫn và tên tệp JPG đầu ra
    jpg_file_path = "path_to_output_jpg_file.jpg"

    # Chuyển đổi ảnh thành JPG và lưu tệp
    cv2.imwrite(jpg_file_path, image, [cv2.IMWRITE_JPEG_QUALITY, 90])
    print("Đã chuyển đổi thành công từ BMP sang JPG.")
else:
    print("Không thể đọc tệp BMP.")

