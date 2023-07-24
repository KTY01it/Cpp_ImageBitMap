#include <iostream>
#include <fstream>

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

void cropBitmap(const std::string& inputPath, const std::string& outputPath, int startX, int startY, int width, int height)
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

    // Kiểm tra xem vùng cắt có hợp lệ hay không
    if (startX < 0 || startX + width > infoHeader.width || startY < 0 || startY + height > infoHeader.height)
    {
        std::cout << "Vung cat khong hop le." << std::endl;
        inputFile.close();
        return;
    }

    // Tạo thông tin tiêu đề cho ảnh mới (vùng cắt)
    BitmapInfoHeader newInfoHeader = infoHeader;
    newInfoHeader.width = width;
    newInfoHeader.height = height;

    // Tính toán kích thước dữ liệu pixel của ảnh mới
    int imageSize = width * height * 3; // 3 bytes cho mỗi pixel (RGB)

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

    // Định vị tới vùng cắt trong tệp bitmap gốc
    int bytesPerPixel = infoHeader.bitDepth / 8;
    int rowSize = (infoHeader.width * bytesPerPixel + 3) & (~3); // Kích thước mỗi dòng (row) trong ảnh gốc (bao gồm padding)

    inputFile.seekg(fileHeader.dataOffset + (infoHeader.height - startY - height) * rowSize + startX * bytesPerPixel, std::ios::beg);

    // Đọc dữ liệu pixel của vùng cắt và ghi vào tệp bitmap mới
    char* pixelData = new char[imageSize];
    int newRowSize = (width * bytesPerPixel + 3) & (~3); // Kích thước mỗi dòng (row) trong vùng cắt (bao gồm padding)

    for (int y = 0; y < height; y++)
    {
        inputFile.read(pixelData + y * newRowSize, newRowSize);
        inputFile.seekg(rowSize - newRowSize, std::ios::cur); // Bỏ qua các byte padding trong ảnh gốc
    }

    outputFile.write(pixelData, imageSize);

    // Giải phóng bộ nhớ
    delete[] pixelData;

    // Đóng tệp
    inputFile.close();
    outputFile.close();

    std::cout << "Da cat va luu anh moi thanh cong." << std::endl;
}

int main()
{
    // Đường dẫn đến tệp bitmap gốc
    std::string inputPath = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/ketqua.bmp";

    // Đường dẫn đến tệp bitmap sau khi cắt
    std::string outputPath = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/cropped2.bmp";

    // Cắt ảnh từ tọa độ (50, 50) với kích thước 200x200
    cropBitmap(inputPath, outputPath, 237, 141, 250, 51);
    // Trục x là trục dọc, y là trục ngang

    return 0;
}
