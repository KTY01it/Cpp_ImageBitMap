#include <iostream>

struct camera_fb_t {
    unsigned short width;
    unsigned short height;
    unsigned int len;
    unsigned char buf[10];
};

void crop_image(camera_fb_t *fb, unsigned short cropLeft, unsigned short cropRight, unsigned short cropTop, unsigned short cropBottom)
{
    unsigned int maxTopIndex = cropTop * fb->width * 2;
    unsigned int minBottomIndex = ((fb->width * fb->height) - (cropBottom * fb->width)) * 2;
    unsigned short maxX = fb->width - cropRight; // In pixels
    unsigned short newWidth = fb->width - cropLeft - cropRight;
    unsigned short newHeight = fb->height - cropTop - cropBottom;
    size_t newJpgSize = newWidth * newHeight * 2;

    unsigned int writeIndex = 0;
    // Loop over all bytes
    for (int i = 0; i < fb->len; i += 2) {
        // Calculate current X, Y pixel position
        int x = (i / 2) % fb->width;

        // Crop from the top
        if (i < maxTopIndex) {
            continue;
        }

        // Crop from the bottom
        if (i > minBottomIndex) {
            continue;
        }

        // Crop from the left
        if (x <= cropLeft) {
            continue;
        }

        // Crop from the right
        if (x > maxX) {
            continue;
        }

        // If we get here, keep the pixels
        fb->buf[writeIndex++] = fb->buf[i];
        fb->buf[writeIndex++] = fb->buf[i + 1];
    }

    // Set the new dimensions of the framebuffer for further use.
    fb->width = newWidth;
    fb->height = newHeight;
    fb->len = newJpgSize;
}

int main() {
    // Create a camera_fb_t object with a 5x5 matrix
    camera_fb_t fb;
    fb.width = 5;
    fb.height = 5;
    fb.len = 10;  // Assuming 2 bytes per pixel (RGB565 format)
    // Fill the buffer with random pixel values (for testing)
    for (int i = 0; i < fb.len; i++) {
        fb.buf[i] = rand() % 256;
    }

    // Call the crop_image function
    crop_image(&fb, 1, 1, 1, 1);

    // Print the cropped image
    for (int i = 0; i < fb.len; i += 2) {
        std::cout << static_cast<int>(fb.buf[i]) << " ";
        std::cout << static_cast<int>(fb.buf[i + 1]) << "\n";
    }

    return 0;
}
