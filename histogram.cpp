#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

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
    int width = 4;
    int height = 4;
    int max_val = 255;

    // Sample input data
    int pdata[16] = {
        100, 120, 150, 180,
        200, 220, 180, 160,
        140, 130, 120, 110,
        100, 90, 80, 70
    };

    // Apply histogram equalization
    equalizeHistogram(pdata, width, height, max_val);

    // Print the equalized data
    for (int i = 0; i < width * height; ++i) {
        std::cout << pdata[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
