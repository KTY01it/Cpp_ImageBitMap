#include <iostream>

struct pixel {
    int r;
    int g;
    int b;
};

void cropImage(pixel**& image, int width, int height, int startX, int startY, int endX, int endY, pixel**& croppedImage)
{
    int subWidth = endX - startX;
    int subHeight = endY - startY;

    croppedImage = new pixel*[subHeight];
    for (int i = 0; i < subHeight; i++)
    {
        croppedImage[i] = new pixel[subWidth];
    }

    for (int i = startY; i < endY; i++)
    {
        for (int j = startX; j < endX; j++)
        {
            croppedImage[i - startY][j - startX] = image[i][j];
        }
    }
}

int main() {
    const int width = 1000;
    const int height = 1000;

    // Tạo ma trận ảo kích thước 1000x1000
    pixel** image = new pixel*[height];
    for (int i = 0; i < height; i++)
    {
        image[i] = new pixel[width];
        for (int j = 0; j < width; j++)
        {
            // Gán giá trị mặc định cho pixel
            image[i][j].r = 255;
            image[i][j].g = 255;
            image[i][j].b = 255;
        }
    }

    // Cắt ảnh
    int startX = 200;
    int startY = 200;
    int endX = 800;
    int endY = 800;

    pixel** croppedImage = nullptr;
    cropImage(image, width, height, startX, startY, endX, endY, croppedImage);

    // In kết quả
    for (int i = 0; i < endY - startY; i++)
    {
        for (int j = 0; j < endX - startX; j++)
        {
            std::cout << "(" << croppedImage[i][j].r << ", " << croppedImage[i][j].g << ", " << croppedImage[i][j].b << ") ";
        }
        std::cout << std::endl;
    }

    // Giải phóng bộ nhớ
    for (int i = 0; i < height; i++)
    {
        delete[] image[i];
    }
    delete[] image;

    for (int i = 0; i < endY - startY; i++)
    {
        delete[] croppedImage[i];
    }
    delete[] croppedImage;

    return 0;
}
