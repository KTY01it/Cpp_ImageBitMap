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

void rotateBitmap(const std::string& inputPath, const std::string& outputPath)
{
    // Mở tệp bitmap gốc
    std::ifstream inputFile(inputPath, std::ios::binary);
    if (!inputFile)
    {
        std::cout << "Khong the mo tep bitmap." << std::endl;
        return;
    }

    // Đọc thông tin tiêu đề của ảnh gốc
    BitmapFileHeader fileHeader;
    BitmapInfoHeader infoHeader;
    inputFile.read(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    inputFile.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader));

    // Kiểm tra xem định dạng bitmap có đúng không
    if (fileHeader.signature[0] != 'B' || fileHeader.signature[1] != 'M')
    {
        std::cout << "Tep khong phai la bitmap." << std::endl;
        inputFile.close();
        return;
    }

    // Kiểm tra xem bitDepth có phải là 24 bit không
    if (infoHeader.bitDepth != 24)
    {
        std::cout << "Chi ho tro bitmap 24 bit." << std::endl;
        inputFile.close();
        return;
    }

    // Tạo thông tin tiêu đề cho ảnh mới (ảnh sau khi xoay)
    BitmapInfoHeader newInfoHeader = infoHeader;
    newInfoHeader.width = infoHeader.height; // Xoay góc 90 độ, nên chiều rộng mới sẽ là chiều cao của ảnh gốc
    newInfoHeader.height = infoHeader.width; // Xoay góc 90 độ, nên chiều cao mới sẽ là chiều rộng của ảnh gốc

    // Tính toán kích thước dữ liệu pixel của ảnh mới
    int imageSize = newInfoHeader.width * newInfoHeader.height * 3; // 3 bytes cho mỗi pixel (RGB)

    // Tạo tệp bitmap mới
    std::ofstream outputFile(outputPath, std::ios::binary);
    if (!outputFile)
    {
        std::cout << "Khong the tao tep bitmap moi." << std::endl;
        inputFile.close();
        return;
    }

    // Sao chép thông tin tiêu đề vào tệp bitmap mới
    outputFile.write(reinterpret_cast<char*>(&fileHeader), sizeof(fileHeader));
    outputFile.write(reinterpret_cast<char*>(&newInfoHeader), sizeof(newInfoHeader));

    // Định vị tới dữ liệu pixel trong tệp bitmap gốc
    inputFile.seekg(fileHeader.dataOffset, std::ios::beg);

    // Đọc dữ liệu pixel của ảnh gốc
    char* pixelData = new char[infoHeader.imageSize];
    inputFile.read(pixelData, infoHeader.imageSize);

    // Xác định các giá trị pixel trong ảnh gốc và ghi vào tệp bitmap mới sau khi xoay
    char* newPixelData = new char[imageSize];

    int oldIndex = 0;
    int newIndex = 0;

    for (int y = 0; y < infoHeader.height; y++)
    {
        for (int x = 0; x < infoHeader.width; x++)
        {
            // Tính toán chỉ số pixel trong ảnh gốc
            oldIndex = (y * infoHeader.width + x) * 3;

            // Tính toán chỉ số pixel trong ảnh mới sau khi xoay góc
            float radians = 3.0f * static_cast<float>(M_PI) / 180.0f; // Góc xoay 3 độ
            float cosine = std::cos(radians);
            float sine = std::sin(radians);
            float newX = static_cast<float>(x) * cosine - static_cast<float>(y) * sine;
            float newY = static_cast<float>(x) * sine + static_cast<float>(y) * cosine;
            newIndex = (static_cast<int>(newY) * newInfoHeader.width + static_cast<int>(newX)) * 3;

            // Gán giá trị pixel từ ảnh gốc vào ảnh mới sau khi xoay
            newPixelData[newIndex] = pixelData[oldIndex];         // Giá trị màu blue
            newPixelData[newIndex + 1] = pixelData[oldIndex + 1]; // Giá trị màu green
            newPixelData[newIndex + 2] = pixelData[oldIndex + 2]; // Giá trị màu red
        }
    }

    // Ghi dữ liệu pixel của ảnh mới sau khi xoay vào tệp bitmap mới
    outputFile.write(newPixelData, imageSize);

    // Giải phóng bộ nhớ
    delete[] pixelData;
    delete[] newPixelData;

    // Đóng tệp
    inputFile.close();
    outputFile.close();

    std::cout << "Da xoay va luu anh moi thanh cong." << std::endl;
}


int main()
{
    // Đường dẫn đến tệp bitmap gốc
    std::string inputPath = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/picture_bmp.bmp";

    // Đường dẫn đến tệp bitmap sau khi xoay
    std::string outputPath = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/rotated.bmp";

    // Xoay ảnh góc 3 độ
    rotateBitmap(inputPath, outputPath);

    return 0;
}
