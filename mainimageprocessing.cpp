#include <stdio.h>
#include <conio.h>
#include <algorithm>
struct pixel
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
};	
struct bmpinfor //khai bao vung luu information của file bmp
{
	unsigned char type[2];  //chứa 2 kí tự "BM" de nhan dien file
	unsigned char file_size[4]; // lưu kich co file
	unsigned char reserved[4]; //phan du tru
	unsigned char data_offset[4]; //vi tri của phan data tinh tu dau file
	unsigned char struct_size[4]; // do lon cua struct infor
	unsigned char width[4];  // chieu ngang
	unsigned char height[4]; // chieu doc
	unsigned char planes[2]; // so luong planes (băng 1 vơi file bmp)
	unsigned char bitpercolor[2]; // so bit quy dinh 1 mau
	unsigned char m_compression_type[4]; // dang nen
	unsigned char size_image[4]; //kich co anh
	unsigned char xpremeter[4]; // do phan giai chieu x
	unsigned char ypremeter[4]; // do phan giai chieu y
	unsigned char m_color_used[4]; // so luong mau su dung
	unsigned char m_color_important[4]; // so luong mau important
};
unsigned char* convert_int_uc(int x)
{
	unsigned char *s = new unsigned char[4];
	s[0] = x % 256;
	s[1] = (x / 256) % 256;
	s[2] = (x / (256 * 256)) % 256;
	s[3] = (x / (256 * 256 * 256)) % 256;
	return s;
};
void readbmp(const char *file_path, bmpinfor &bmifor, unsigned char * &data)
{
	FILE *fp;
	fp = fopen(file_path, "rb"); //mo file bmp
	if ((fp) == nullptr)
	{
		printf("Can not read this file"); // thong bao bi loi khi mo
		
	}
	//ghi thong tin
	fread(&bmifor, sizeof(bmpinfor), 1, fp);// doc information
	
	//truy xuat cac gia tri can dung de doc
	int width =*(int*) bmifor.width;  //truy xuat chieu dai
	long height = *(int*) bmifor.height; //truy xuat chieu cao
	short bitcolor =*(int*) bmifor.bitpercolor; // truy xuat so bit xac dinh mot mau
	long dataoffset =*(int*) bmifor.data_offset; //truy xuat offset cua vung data
	
	//tinh toan pading byte va kich thuoc data
	int padbyte = width % 4;
	int fsize = height*(width*(bitcolor / 8) + padbyte);
	
	//doc data
	data = new unsigned char[fsize];// cap phat vung nho cho data
	fseek(fp, dataoffset, SEEK_SET);// chuyen con tro ve vung data
	fread(data, sizeof(char), fsize, fp);// ghi du lieu vao mang data fsize phan tu 1 byte

	fclose(fp);
};
void savebmp(const char *file_path, bmpinfor &bmifor, unsigned char *&data)
{
	//truy xuat cac gia tri can dung de ghi
	int width =*(int*) bmifor.width;
	int height = *(int*) bmifor.height;
	int bitcolor =*(int*) bmifor.bitpercolor; // truy xuat so bit xac dinh mot mau
	int dataoffset =*(int*) bmifor.data_offset; //truy xuat offset cua vung data

	//tinh toan pading byte va kich thuoc data
	int padbyte = width % 4;
	int fsize = height*(width*(bitcolor / 8) + padbyte);

	// khai bao con tro file
	FILE *fp;
	fp = fopen(file_path, "w+b");

	fwrite(&bmifor, sizeof(bmpinfor), 1, fp);// ghi information
	fwrite(data, fsize, 1, fp); // ghi data

	fclose(fp);
};
pixel** convert_data_pixel(bmpinfor &bmifor,unsigned char* &data)
{
	// truy xuat du lieu can thiet
	int height =*(int*) bmifor.height; //truy xuat chieu dai
	long width =*(int*) bmifor.width; //truy xuat chieu rong
	
	//tinh toan cac gia tri can thiet
	int size = height * width;//kich co vung data pixel
	int pading = width % 4; // tinh pading byte

	pixel ** pix = new pixel*[height];// cap phat mang 2 chieu de lưu pixel
	for (int i = 0; i < height; i++)
	{
		pix[i] = new pixel[width];
	};
	
	unsigned char *temp = data; // con tro tam de luu data
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			pix[i][j].B = *(temp++);// lay gia tri blue
			pix[i][j].G = *(temp++);// lay gia tri green
			pix[i][j].R = *(temp++);// lay gia tri red
		};
		temp = temp + pading;// bo qua pading file
	};
	return pix; // tra ve mang 2 chieu pix
};
unsigned char* convert_pixel_data(bmpinfor &bmifor, pixel **&pix)
{
	//truy xuat  gia tri
	int bitpercolor =*(int*) bmifor.bitpercolor; //so bit /mau
	int width = *(int*)bmifor.width;  //chieu dai
	int height = *(int*)bmifor.height; //chieu rong
	
	//tinh toan gia tri can thiet
	int pad = width % 4; //pading byte
	int size = height*(width*(bitpercolor / 8) + pad); //kich co vung data cua anh

	unsigned char *data = new unsigned char[size]; // cap phat vung nho
	unsigned char *temp = data; //con tro temp tro den du lieu

	//chuyen du lieu tu pixel vao data
	for (int i = 0; i < (height); i++)
	{
		for (int j = 0; j < (width); j++)
		{
			*(temp++) = pix[i][j].B; // ghi gia tri blue
			*(temp++) = pix[i][j].G; // ghi gia tri green
			*(temp++) = pix[i][j].R; // ghi gia tri red
		};
		for (int k = 0; k < pad; k++) //ghi padingbyte
			*(temp++) = 0;
	};
	return data;
};
pixel** cropImage(bmpinfor& bmifor, pixel**& pix, int startX, int startY, int endX, int endY){
    int width = *(int*)bmifor.width;
    int height = *(int*)bmifor.height;

    int subWidth = endX - startX;
    int subHeight = endY - startY;

    pixel** croppedPix = new pixel*[height];
    for (int i = 0; i < height; i++)
    {
        croppedPix[i] = new pixel[width];
    }

    for (int i = startY; i < endY; i++)
    {
        for (int j = startX; j < endX; j++)
        {
            croppedPix[i - startY][j - startX] = pix[i][j];
        }
    }

    return croppedPix;
};
pixel** inc_brightness(pixel**&pix,int height,int width, int c)
{
	if ((c < 0) || (c > 255)) // kiểm tra giá trị c
	{
		printf("Bạn đã nhập sai. Hãy nhập giá trị trong khoảng từ 0 đến 255.\n");
		return nullptr;
	}

	// Tạo một mảng mới và sao chép giá trị từ mảng đầu vào sang mảng mới
	pixel** newPix = new pixel*[height];
	for (int i = 0; i < height; i++)
	{
		newPix[i] = new pixel[width];
		for (int j = 0; j < width; j++)
		{
			newPix[i][j].B = pix[i][j].B;
			newPix[i][j].G = pix[i][j].G;
			newPix[i][j].R = pix[i][j].R;
		}
	}

	// Tăng độ sáng của mỗi pixel trong mảng mới
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			newPix[i][j].B = (((newPix[i][j].B + c) > 255) ? 255 : (newPix[i][j].B + c));
			newPix[i][j].G = (((newPix[i][j].G + c) > 255) ? 255 : (newPix[i][j].G + c));
			newPix[i][j].R = (((newPix[i][j].R + c) > 255) ? 255 : (newPix[i][j].R + c));
		}
	}

	return newPix;
};
pixel** inc_contrast(pixel**& pix, int height, int width, int c)
{
	if ((c < 0) || (c > 100))
	{
		printf("Giá trị bạn nhập không phù hợp. Hãy nhập giá trị trong khoảng từ 0 đến 100.\n");
		return nullptr;
	}

	double contrast = ((100.0 + c) / 100.0) * ((100.0 + c) / 100.0); // tính độ tương phản
	pixel** newPix = new pixel*[height];

	for (int i = 0; i < height; i++)
	{
		newPix[i] = new pixel[width];

		for (int j = 0; j < width; j++)
		{
			double R = pix[i][j].R - 177;
			R = R * contrast;
			R = R + 177;
			if (R > 255) R = 255;
			if (R < 0) R = 0;
			newPix[i][j].R = (unsigned char)R;

			double G = pix[i][j].G - 177;
			G = G * contrast;
			G = G + 177;
			if (G > 255) G = 255;
			if (G < 0) G = 0;
			newPix[i][j].G = (unsigned char)G;

			double B = pix[i][j].B - 177;
			B = B * contrast;
			B = B + 177;
			if (B > 255) B = 255;
			if (B < 0) B = 0;
			newPix[i][j].B = (unsigned char)B;
		}
	}

	return newPix;
};
pixel** histogramequalization(pixel** pix, int height, int width) {
    // Calculate the histogram
    int histogram[256] = {0};
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            unsigned char intensity = pix[i][j].R;
            histogram[intensity]++;
        }
    }

    // Calculate the cumulative distribution function (CDF)
    int cdf[256] = {0};
    cdf[0] = histogram[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + histogram[i];
    }

    // Perform histogram equalization
    pixel** equalizedPix = new pixel*[height];
    for (int i = 0; i < height; i++) {
        equalizedPix[i] = new pixel[width];
        for (int j = 0; j < width; j++) {
            unsigned char intensity = pix[i][j].R;
            unsigned char equalizedValue = static_cast<unsigned char>((cdf[intensity] * 255) / (height * width));
            equalizedPix[i][j].R = equalizedValue;
            equalizedPix[i][j].G = equalizedValue;
            equalizedPix[i][j].B = equalizedValue;
        }
    }
    delete[] pix;
    return equalizedPix;
};
pixel** gaussianfilter(pixel** pix, int height, int width) {
    // Create a new pixel array for the filtered image
    pixel** filteredPix = new pixel*[height];
    for (int i = 0; i < height; i++) {
        filteredPix[i] = new pixel[width];
    }

    // Define the Gaussian kernel
    float kernel[5][5] = {
        { 1,  4,  7,  4,  1 },
        { 4, 16, 26, 16,  4 },
        { 7, 26, 41, 26,  7 },
        { 4, 16, 26, 16,  4 },
        { 1,  4,  7,  4,  1 }
    };

    // Apply the Gaussian filter to each pixel in the image
    for (int i = 2; i < height - 2; i++) {
        for (int j = 2; j < width - 2; j++) {
            float sumR = 0, sumG = 0, sumB = 0;

            // Convolve the kernel with the neighboring pixels' intensities
            for (int k = -2; k <= 2; k++) {
                for (int l = -2; l <= 2; l++) {
                    int neighborR = pix[i + k][j + l].R;
                    int neighborG = pix[i + k][j + l].G;
                    int neighborB = pix[i + k][j + l].B;

                    sumR += kernel[k + 2][l + 2] * neighborR;
                    sumG += kernel[k + 2][l + 2] * neighborG;
                    sumB += kernel[k + 2][l + 2] * neighborB;
                }
            }

            // Normalize the sums
            sumR /= 273;
            sumG /= 273;
            sumB /= 273;

            // Set the pixel's intensity to the normalized sums
            filteredPix[i][j].R = static_cast<unsigned char>(sumR);
            filteredPix[i][j].G = static_cast<unsigned char>(sumG);
            filteredPix[i][j].B = static_cast<unsigned char>(sumB);
        }
    }

    // Copy the border pixels from the original image to the filtered image
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < 2; j++) {
            filteredPix[i][j] = pix[i][j];
            filteredPix[i][width - 1 - j] = pix[i][width - 1 - j];
        }
    }
    for (int j = 0; j < width; j++) {
        for (int i = 0; i < 2; i++) {
            filteredPix[i][j] = pix[i][j];
            filteredPix[height - 1 - i][j] = pix[height - 1 - i][j];
        }
    }
    delete[] pix;
    return filteredPix;
};
pixel** meanfilter(pixel** pix, int height, int width) {
    pixel** filteredPix = new pixel*[height];
    for (int i = 0; i < height; i++) {
        filteredPix[i] = new pixel[width];
        for (int j = 0; j < width; j++) {
            int sumR = 0, sumG = 0, sumB = 0;
            int count = 0;

            // Calculate the sum of pixel values in the neighborhood
            for (int k = i - 1; k <= i + 1; k++) {
                for (int l = j - 1; l <= j + 1; l++) {
                    if (k >= 0 && k < height && l >= 0 && l < width) {
                        sumR += pix[k][l].R;
                        sumG += pix[k][l].G;
                        sumB += pix[k][l].B;
                        count++;
                    }
                }
            }

            // Calculate the average values
            int avgR = sumR / count;
            int avgG = sumG / count;
            int avgB = sumB / count;

            // Set the pixel's intensity to the average values
            filteredPix[i][j].R = static_cast<unsigned char>(avgR);
            filteredPix[i][j].G = static_cast<unsigned char>(avgG);
            filteredPix[i][j].B = static_cast<unsigned char>(avgB);
        }
    }
    delete[] pix;
    return filteredPix;
};
pixel** medianfilter(pixel**& pix, int height, int width) {
    // Create a new pixel array for the filtered image
    pixel** filteredPix = new pixel*[height];
    for (int i = 0; i < height; i++) {
        filteredPix[i] = new pixel[width];
    }

    // Apply the median filter to each pixel in the image
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            // Create a temporary array to store the neighboring pixels' intensities
            unsigned char intensities[9];

            // Extract the intensities of the neighboring pixels
            intensities[0] = pix[i - 1][j - 1].R;
            intensities[1] = pix[i - 1][j].R;
            intensities[2] = pix[i - 1][j + 1].R;
            intensities[3] = pix[i][j - 1].R;
            intensities[4] = pix[i][j].R;
            intensities[5] = pix[i][j + 1].R;
            intensities[6] = pix[i + 1][j - 1].R;
            intensities[7] = pix[i + 1][j].R;
            intensities[8] = pix[i + 1][j + 1].R;

            // Sort the intensities in ascending order
            std::sort(intensities, intensities + 9);

            // Get the median intensity
            unsigned char median = intensities[4];

            // Set the pixel's intensity to the median value
            filteredPix[i][j].R = median;
            filteredPix[i][j].G = median;
            filteredPix[i][j].B = median;
        }
    }

    // Copy the border pixels from the original image to the filtered image
    for (int i = 0; i < height; i++) {
        filteredPix[i][0] = pix[i][0];
        filteredPix[i][width - 1] = pix[i][width - 1];
    }
    for (int j = 0; j < width; j++) {
        filteredPix[0][j] = pix[0][j];
        filteredPix[height - 1][j] = pix[height - 1][j];
    }
    delete[] pix;
    return filteredPix;
};
pixel** thresholding(pixel** pix, int height, int width, unsigned char threshold) {
    pixel** thresholdedPix = new pixel*[height];
    for (int i = 0; i < height; i++) {
        thresholdedPix[i] = new pixel[width];
        for (int j = 0; j < width; j++) {
            unsigned char intensity = pix[i][j].R;

            // Apply thresholding
            if (intensity >= threshold) {
                thresholdedPix[i][j].R = 0;
                thresholdedPix[i][j].G = 0;
                thresholdedPix[i][j].B = 0;
            } else {
                thresholdedPix[i][j].R = 255;
                thresholdedPix[i][j].G = 255;
                thresholdedPix[i][j].B = 255;
            }
        }
    }
    delete[] pix;
    return thresholdedPix;
};
pixel** erosion(pixel** pix, int height, int width) {
    pixel** erodedPix = new pixel*[height];
    for (int i = 0; i < height; i++) {
        erodedPix[i] = new pixel[width];
        for (int j = 0; j < width; j++) {
            // Check if the pixel is black
            if (pix[i][j].R == 0 && pix[i][j].G == 0 && pix[i][j].B == 0) {
                // Check if any neighboring pixels are white
                bool shouldErode = false;
                for (int k = -1; k <= 1; k++) {
                    for (int l = -1; l <= 1; l++) {
                        int ni = i + k;
                        int nj = j + l;

                        // Skip out-of-bounds pixels
                        if (ni < 0 || ni >= height || nj < 0 || nj >= width) {
                            continue;
                        }

                        // Check if the neighboring pixel is white
                        if (pix[ni][nj].R == 255 && pix[ni][nj].G == 255 && pix[ni][nj].B == 255) {
                            shouldErode = true;
                            break;
                        }
                    }
                    if (shouldErode) {
                        break;
                    }
                }

                // Erode the pixel if necessary
                if (shouldErode) {
                    erodedPix[i][j].R = 255;
                    erodedPix[i][j].G = 255;
                    erodedPix[i][j].B = 255;
                } else {
                    erodedPix[i][j].R = 0;
                    erodedPix[i][j].G = 0;
                    erodedPix[i][j].B = 0;
                }
            } else {
                erodedPix[i][j] = pix[i][j];
            }
        }
    }
    delete[] pix;
    return erodedPix;
};


int main (){
    char anh[] ="C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/picture3.bmp";
    char kq[] = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/end.bmp";
    bmpinfor bmifor;
    unsigned char* data = NULL;
    unsigned char* data1 = NULL;
    pixel** pix;
    readbmp(anh,bmifor,data);
    pix = convert_data_pixel(bmifor, data);
    // int width = *(int*)bmifor.width;
    // int height = *(int*)bmifor.height;
    // int photo_size = width * height;
    const int startX = 237;  // Vị trí x bắt đầu cắt
    const int startY = 410;  // Vị trí y bắt đầu cắt
    const int endX = 487;    // Vị trí x kết thúc cắt
    const int endY = 457;    // Vị trí y kết thúc cắt
    // Cắt ảnh
    pixel** cropedPix;
    cropedPix = cropImage(bmifor, pix, startX, startY, endX, endY);
    int width = *(int*)bmifor.width = endX - startX;
    int height = *(int*)bmifor.height = endY - startY;
    int photo_size = width * height;
    
    cropedPix = inc_brightness(cropedPix, height, width,10);
    cropedPix = inc_contrast(cropedPix,height, width,3);
    cropedPix = histogramequalization(cropedPix, height, width);
    cropedPix = meanfilter(cropedPix, height, width);
    cropedPix = thresholding(cropedPix, height, width, 82);
    cropedPix = erosion(cropedPix, height, width);
    
    for (int i = 0; i < 5; i++) {
		char outputtitle[20]; // Đặt kích thước phù hợp với tên file tối đa
		sprintf(outputtitle, "crop%d.bmp", i);
		
		if (i == 0) {
            pixel** cropedPix0;
            int width = *(int*)bmifor.width = 40;
            int height = *(int*)bmifor.height = 45;
            cropedPix0 = cropImage(bmifor, cropedPix, 0, 0, 40, 45);
            data1 = convert_pixel_data(bmifor, cropedPix0);
    	    savebmp(outputtitle,bmifor,data1);
		}
    	else if (i == 1) {
            pixel** cropedPix1;
            int width = *(int*)bmifor.width = 40;
            int height = *(int*)bmifor.height = 45;
        	cropedPix1 = cropImage(bmifor, cropedPix, 53, 0, 93, 45);
            data1 = convert_pixel_data(bmifor, cropedPix1);
    	    savebmp(outputtitle,bmifor,data1);
		}
    	else if (i == 2){
            pixel** cropedPix2;
            int width = *(int*)bmifor.width = 40;
            int height = *(int*)bmifor.height = 45;
        	cropedPix2 = cropImage(bmifor, cropedPix, 106, 0, 146, 45);
            data1 = convert_pixel_data(bmifor, cropedPix2);
    	    savebmp(outputtitle,bmifor,data1);
		}
     	else if (i == 3){
            pixel** cropedPix3;
            int width = *(int*)bmifor.width = 40;
            int height = *(int*)bmifor.height = 45;
        	cropedPix3 = cropImage(bmifor, cropedPix, 159, 0, 199, 45);
            data1 = convert_pixel_data(bmifor, cropedPix3);
    	    savebmp(outputtitle,bmifor,data1);
		}
     	else if (i == 4){
            pixel** cropedPix4;
            int width = *(int*)bmifor.width = 40;
            int height = *(int*)bmifor.height = 45;
            cropedPix4 = cropImage(bmifor, cropedPix, 212, 0, 252, 45);
            data1 = convert_pixel_data(bmifor, cropedPix4);
    	    savebmp(outputtitle,bmifor,data1);
		}
	}

    data1 = convert_pixel_data(bmifor, cropedPix);
    savebmp(kq,bmifor,data1);
}