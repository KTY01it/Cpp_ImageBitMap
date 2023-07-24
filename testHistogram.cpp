#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

// Structure to store the header information of a bitmap file
struct BitmapHeader {
    char signature[2];
    int file_size;
    int reserved;
    int data_offset;
    int header_size;
    int width;
    int height;
    short planes;
    short bits_per_pixel;
    int compression;
    int image_size;
    int x_pixels_per_meter;
    int y_pixels_per_meter;
    int colors_used;
    int important_colors;
};

void equalizeHistogram(int* pdata, int width, int height, int max_val = 255)
{
    int total = width * height;
    int n_bins = max_val + 1;

    // Compute histogram
    std::vector<int> hist(n_bins, 0);
    for (int i = 0; i < total; ++i) {
        hist[pdata[i]]++;
    }

    // Build LUT from cumulative histogram

    // Find first non-zero bin
    int i = 0;
    while (!hist[i]) ++i;

    if (hist[i] == total) {
        for (int j = 0; j < total; ++j) {
            pdata[j] = i;
        }
        return;
    }

    // Compute scale
    float scale = (n_bins - 1.f) / (total - hist[i]);

    // Initialize lut
    std::vector<int> lut(n_bins, 0);
    i++;

    int sum = 0;
    for (; i < hist.size(); ++i) {
        sum += hist[i];
        // the value is saturated in range [0, max_val]
        lut[i] = std::max(0, std::min(static_cast<int>(std::round(sum * scale)), max_val));
    }

    // Apply equalization
    for (int i = 0; i < total; ++i) {
        pdata[i] = lut[pdata[i]];
    }
}

int main()
{
    std::string inputImagePath = "picture3.bmp";

    // Read the bitmap file
    std::ifstream file(inputImagePath, std::ios::binary);
    if (!file) {
        std::cout << "Failed to open the bitmap file!" << std::endl;
        return -1;
    }

    // Read the bitmap header
    BitmapHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));

    // Verify the bitmap signature
    if (header.signature[0] != 'B' || header.signature[1] != 'M') {
        std::cout << "Invalid bitmap file!" << std::endl;
        return -1;
    }

    // Check if the bitmap is 24-bit (supports only grayscale)
    if (header.bits_per_pixel != 24) {
        std::cout << "Unsupported bitmap format! Only 24-bit bitmaps are supported." << std::endl;
        return -1;
    }

    // Calculate padding bytes
    int padding = (4 - (header.width * 3) % 4) % 4;

    // Allocate memory for pixel data
    int* pdata = new int[header.width * header.height];

    // Read pixel data
    for (int y = header.height - 1; y >= 0; --y) {
        for (int x = 0; x < header.width; ++x) {
            unsigned char pixel[3];
            file.read(reinterpret_cast<char*>(pixel), sizeof(pixel));

            // Convert RGB to grayscale using the formula: Y = 0.299*R + 0.587*G + 0.114*B
            int gray_value = static_cast<int>(0.299 * pixel[2] + 0.587 * pixel[1] + 0.114 * pixel[0]);
            pdata[y * header.width + x] = gray_value;
        }

        // Skip padding bytes
        file.seekg(padding, std::ios::cur);
    }

    file.close();

    // Apply equalization
    equalizeHistogram(pdata, header.width, header.height);

    // Create a new bitmap file to save the equalized image
    std::ofstream outputFile("hist1.bmp", std::ios::binary);
    if (!outputFile) {
        std::cout << "Failed to create the output bitmap file!" << std::endl;
        delete[] pdata;
        return -1;
    }

    // Write the bitmap header to the output file
    outputFile.write(reinterpret_cast<char*>(&header), sizeof(header));

    // Write the equalized pixel data to the output file
    for (int y = header.height - 1; y >= 0; --y) {
        for (int x = 0; x < header.width; ++x) {
            unsigned char pixel[3] = { static_cast<unsigned char>(pdata[y * header.width + x]),
                                       static_cast<unsigned char>(pdata[y * header.width + x]),
                                       static_cast<unsigned char>(pdata[y * header.width + x]) };
            outputFile.write(reinterpret_cast<char*>(pixel), sizeof(pixel));
        }

        // Write padding bytes
        unsigned char padding_bytes[4] = { 0 };
        outputFile.write(reinterpret_cast<char*>(padding_bytes), padding);
    }

    outputFile.close();

    // Clean up
    delete[] pdata;

    return 0;
}
