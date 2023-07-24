#include <iostream>
#include <fstream>

#pragma pack(push, 1)

struct BitmapFileHeader {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
};

struct BitmapInfoHeader {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t sizeImage;
    int32_t xPelsPerMeter;
    int32_t yPelsPerMeter;
    uint32_t clrUsed;
    uint32_t clrImportant;
};

#pragma pack(pop)

void convertJpgToBmp(const char* jpgFile, const char* bmpFile, int width, int height) {
    // Mở tệp JPG để đọc dữ liệu pixel
    std::ifstream jpgInput(jpgFile, std::ios::binary);
    if (!jpgInput) {
        std::cerr << "Không thể mở tệp JPG" << std::endl;
        return;
    }

    // Tạo tệp BMP mới để ghi dữ liệu pixel đã chuyển đổi
    std::ofstream bmpOutput(bmpFile, std::ios::binary);
    if (!bmpOutput) {
        std::cerr << "Không thể tạo tệp BMP" << std::endl;
        return;
    }

    // Đọc dữ liệu pixel từ tệp JPG và ghi vào tệp BMP
    const int bytesPerPixel = 3;  // RGB
    const int paddingSize = 4;
    const int dataSize = width * bytesPerPixel + paddingSize;
    const int fileSize = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + dataSize * height;

    // Tạo thông tin header cho tệp BMP
    BitmapFileHeader bmpFileHeader;
    bmpFileHeader.type = 0x4D42;  // "BM" đối với BMP
    bmpFileHeader.size = fileSize;
    bmpFileHeader.reserved1 = 0;
    bmpFileHeader.reserved2 = 0;
    bmpFileHeader.offset = sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader);

    BitmapInfoHeader bmpInfoHeader;
    bmpInfoHeader.size = sizeof(BitmapInfoHeader);
    bmpInfoHeader.width = width;
    bmpInfoHeader.height = -height;  // Đảo chiều cao để đúng với BMP
    bmpInfoHeader.planes = 1;
    bmpInfoHeader.bitCount = bytesPerPixel * 8;
    bmpInfoHeader.compression = 0;
    bmpInfoHeader.sizeImage = 0;
    bmpInfoHeader.xPelsPerMeter = 0;
    bmpInfoHeader.yPelsPerMeter = 0;
    bmpInfoHeader.clrUsed = 0;
    bmpInfoHeader.clrImportant = 0;

    // Ghi thông tin header vào tệp BMP
    bmpOutput.write(reinterpret_cast<const char*>(&bmpFileHeader), sizeof(BitmapFileHeader));
    bmpOutput.write(reinterpret_cast<const char*>(&bmpInfoHeader), sizeof(BitmapInfoHeader));

    // Đọc dữ liệu pixel từ tệp JPG và ghi vào tệp BMP
    char pixel[bytesPerPixel];
    char padding[paddingSize] = { 0 };

    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            // Đọc pixel từ tệp JPG
            jpgInput.read(pixel, bytesPerPixel);

            // Ghi pixel vào tệp BMP
            bmpOutput.write(pixel, bytesPerPixel);
        }

        // Ghi các byte padding vào tệp BMP
        bmpOutput.write(padding, paddingSize);
    }

    // Đóng tệp
    jpgInput.close();
    bmpOutput.close();

    std::cout << "Chuyển đổi thành công từ JPG sang BMP." << std::endl;
}

int main() {
    const char* jpgFile = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/picture10.jpg";
    const char* bmpFile = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/convert.bmp";
    int width = 1280;
    int height = 720;

    convertJpgToBmp(jpgFile, bmpFile, width, height);

    return 0;
}
