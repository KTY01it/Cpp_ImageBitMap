#include <iostream>
#include <fstream>
#include <cmath>

#pragma pack(push, 1)
struct BitmapFileHeader
{
    char signature[2];
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
};

struct BitmapInfoHeader
{
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitDepth;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsUsed;
    uint32_t colorsImportant;
};
#pragma pack(pop)

// Hàm xoay ảnh bitmap
void rotateImage(float angle, char* sourcePixels, int sourceWidth, int sourceHeight, char* destinationPixels)
{
    // Chuyển đổi góc xoay thành radian
    float radians = angle * 3.14159f / 180.0f; // Thay đổi góc xoay bằng cách thay đổi giá trị của biến Angle

    // Tính toán tâm của ảnh xoay
    float centerX = sourceWidth / 2.0f;
    float centerY = sourceHeight / 2.0f;

    for (int y = 0; y < sourceHeight; ++y)
    {
        for (int x = 0; x < sourceWidth; ++x)
        {
            // Tính toán vị trí pixel xoay mới
            float newX = cos(radians) * (x - centerX) + sin(radians) * (y - centerY) + centerX;
            float newY = -sin(radians) * (x - centerX) + cos(radians) * (y - centerY) + centerY;

            // Lấy giá trị RGB từ vị trí pixel xoay mới
            int sourceIndex = (static_cast<int>(newY) * sourceWidth + static_cast<int>(newX)) * 3;
            int destinationIndex = (y * sourceWidth + x) * 3;
            destinationPixels[destinationIndex] = sourcePixels[sourceIndex]; // Sao chép giá trị R
            destinationPixels[destinationIndex + 1] = sourcePixels[sourceIndex + 1]; // Sao chép giá trị G
            destinationPixels[destinationIndex + 2] = sourcePixels[sourceIndex + 2]; // Sao chép giá trị B
        }
    }
}

int main()
{
    std::string imagePath = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/picture_bmp.bmp";

    std::ifstream file(imagePath, std::ios::binary);
    if (!file)
    {
        std::cout << "Khong the mo tep bitmap." << std::endl;
        return -1;
    }

    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;

    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    if (fileHeader.signature[0] != 'B' || fileHeader.signature[1] != 'M')
    {
        std::cout << "Tep khong phai la bitmap." << std::endl;
        return -1;
    }

    if (infoHeader.bitDepth != 24)
    {
        std::cout << "Chi ho tro bitmap 24 bit." << std::endl;
        return -1;
    }

    int imageSize = infoHeader.width * infoHeader.height * 3;
    char* pixelData = new char[imageSize];
    file.seekg(fileHeader.dataOffset, std::ios::beg);
    file.read(pixelData, imageSize);

    std::cout << "Width: " << infoHeader.width << std::endl;
    std::cout << "Height: " << infoHeader.height << std::endl;

    // Tạo mảng pixel mới để lưu trữ ảnh sau khi xoay
    char* rotatedPixels = new char[imageSize];

    // Xoay ảnh 3 độ
    float angle = 3.0f; // Góc xoay (đơn vị: độ)
    rotateImage(angle, pixelData, infoHeader.width, infoHeader.height, rotatedPixels);

    // Ghi dữ liệu pixel mới vào tệp bitmap xoay
    std::string newImagePath = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/rotated.bmp";
    std::ofstream rotatedFile(newImagePath, std::ios::binary);
    rotatedFile.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    rotatedFile.write(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));
    rotatedFile.write(rotatedPixels, imageSize);
    rotatedFile.close();


    delete[] pixelData;
    delete[] rotatedPixels;

    file.close();

    return 0;
}
