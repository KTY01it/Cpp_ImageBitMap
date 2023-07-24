#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

#pragma pack(push, 1)
struct BitmapHeader {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t offset;
    uint32_t header_size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;
    int32_t x_pixels_per_meter;
    int32_t y_pixels_per_meter;
    uint32_t total_colors;
    uint32_t important_colors;
};
#pragma pack(pop)

void convertJPGtoBMP(const std::string& inputFilePath, const std::string& outputFilePath, int width, int height) {
    std::ifstream inputFile(inputFilePath, std::ios::binary);
    if (!inputFile) {
        std::cout << "Error opening input file: " << inputFilePath << std::endl;
        return;
    }
    
    std::vector<char> jpgData((std::istreambuf_iterator<char>(inputFile)), std::istreambuf_iterator<char>());
    inputFile.close();

    BitmapHeader bmpHeader;
    bmpHeader.type = 0x4D42;  // "BM"
    bmpHeader.size = sizeof(BitmapHeader) + width * height * 3;
    bmpHeader.reserved1 = 0;
    bmpHeader.reserved2 = 0;
    bmpHeader.offset = sizeof(BitmapHeader);
    bmpHeader.header_size = 40;
    bmpHeader.width = width;
    bmpHeader.height = height;
    bmpHeader.planes = 1;
    bmpHeader.bits_per_pixel = 24;
    bmpHeader.compression = 0;
    bmpHeader.image_size = width * height * 3;
    bmpHeader.x_pixels_per_meter = 0;
    bmpHeader.y_pixels_per_meter = 0;
    bmpHeader.total_colors = 0;
    bmpHeader.important_colors = 0;

    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile) {
        std::cout << "Error opening output file: " << outputFilePath << std::endl;
        return;
    }
    
    outputFile.write(reinterpret_cast<const char*>(&bmpHeader), sizeof(BitmapHeader));

    int jpgIndex = 0;
    std::vector<char> bmpData(width * height * 3, 0);
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int r = static_cast<int>(static_cast<uint8_t>(jpgData[jpgIndex++]));
            int g = static_cast<int>(static_cast<uint8_t>(jpgData[jpgIndex++]));
            int b = static_cast<int>(static_cast<uint8_t>(jpgData[jpgIndex++]));

            int bmpIndex = (y * width + x) * 3;
            bmpData[bmpIndex] = static_cast<char>(b);
            bmpData[bmpIndex + 1] = static_cast<char>(g);
            bmpData[bmpIndex + 2] = static_cast<char>(r);
        }
    }

    outputFile.write(bmpData.data(), bmpData.size());
    outputFile.close();

    std::cout << "JPEG to BMP conversion completed." << std::endl;
}

int main() {
    std::string inputFilePath = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/picture10.jpg";
    std::string outputFilePath = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/convert1.bmp";
    int width = 1280;
    int height = 720;

    convertJPGtoBMP(inputFilePath, outputFilePath, width, height);

    return 0;
}
