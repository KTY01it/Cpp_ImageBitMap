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

void grayscale(pixel**&pix,int height,int width ){
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			// lấy giá trị xám trung bình
            int val = (pix[i][j].B + pix[i][j].G + pix[i][j].R) / 3; 
			//tat ca cac gia tri RGB ve mot do sang
            pix[i][j].B = pix[i][j].G = pix[i][j].R = val;   
		}
};

void inc_brightness(pixel**&pix,int height,int width, int c)
{
	if ((c<0)||(c>255)) // kiem tra gia tri C
	{
		printf(" ban da nhap sai chi nhap cac gia tri 0<c<255/n");
		return;
	};
	// tang do sang cua cac pixel them 1 gia tri c gia tri sau khi tang phai nho hon 255
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
		pix[i][j].B = (((pix[i][j].B + c) > 255) ? 255 : (pix[i][j].B + c));
		pix[i][j].G = (((pix[i][j].G + c) > 255) ? 255 : (pix[i][j].G + c));
		pix[i][j].R = (((pix[i][j].R + c) > 255) ? 255 : (pix[i][j].R + c));
		}
};

void inc_contrast(pixel**&pix, int height,int width, int c)
{
	if ((c < 0) || (c > 100))
	{
		printf("\n gia tri ban nhap khong phu hop 0 < c < 100 ");
		return;
	};
	double contrast = ((100.0 + c) / 100.0)*((100.0+c)/100.0); // tinh do tuong phan
	double R, G, B;
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			//xu ly mau red
			R = pix[i][j].R - 177;  
			R = R*contrast;  
			R = R + 177;
			if (R > 255) R = 255;
			if (R < 0) R = 0;
			pix[i][j].R = (unsigned char)R;

			//xu ly mau green
			G = pix[i][j].G - 177;
			G = G*contrast;
			G = G + 177;
			if (G > 255) G = 255;
			if (G < 0) G = 0;
			pix[i][j].G = (unsigned char)G;

			//xu ly mau blue
			B = pix[i][j].B - 177;
			B = B*contrast;
			B = B + 177;
			if (B > 255) B = 255;
			if (B < 0) B = 0;
			pix[i][j].B = (unsigned char)B;
		};
};

void meanfilter(pixel**& pix, int height, int width) {
    // Create a temporary pixel array to store the filtered image
    pixel** tempPix = new pixel*[height];
    for (int i = 0; i < height; i++) {
        tempPix[i] = new pixel[width];
    }

    // Apply the mean filter to each pixel in the image
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            int sumR = 0, sumG = 0, sumB = 0;

            // Compute the average of the neighboring pixels' intensities
            for (int k = -1; k <= 1; k++) {
                for (int l = -1; l <= 1; l++) {
                    sumR += pix[i + k][j + l].R;
                    sumG += pix[i + k][j + l].G;
                    sumB += pix[i + k][j + l].B;
                }
            }

            // Compute the average intensities
            int avgR = sumR / 9;
            int avgG = sumG / 9;
            int avgB = sumB / 9;

            // Set the pixel's intensity to the average intensities
            tempPix[i][j].R = static_cast<unsigned char>(avgR);
            tempPix[i][j].G = static_cast<unsigned char>(avgG);
            tempPix[i][j].B = static_cast<unsigned char>(avgB);
        }
    }

    // Copy the filtered image from the temporary pixel array back to the original pixel array
    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            pix[i][j] = tempPix[i][j];
        }
    }

    // Deallocate the memory for the temporary pixel array
    for (int i = 0; i < height; i++) {
        delete[] tempPix[i];
    }
    delete[] tempPix;
}

void histogramequalization(pixel**& pix, int height, int width) {
    // Calculate the histogram of the grayscale image
    int histogram[256] = { 0 };
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int intensity = (pix[i][j].R + pix[i][j].G + pix[i][j].B) / 3;
            histogram[intensity]++;
        }
    }

    // Calculate the cumulative distribution function (CDF) of the histogram
    int cdf[256] = { 0 };
    cdf[0] = histogram[0];
    for (int i = 1; i < 256; i++) {
        cdf[i] = cdf[i - 1] + histogram[i];
    }

    // Compute the equalized intensity values for each pixel
    int totalPixels = height * width;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int intensity = (pix[i][j].R + pix[i][j].G + pix[i][j].B) / 3;
            int equalizedIntensity = (cdf[intensity] * 255) / totalPixels;

            // Assign the equalized intensity to each channel (R, G, B) of the pixel
            pix[i][j].R = static_cast<unsigned char>(equalizedIntensity);
            pix[i][j].G = static_cast<unsigned char>(equalizedIntensity);
            pix[i][j].B = static_cast<unsigned char>(equalizedIntensity);
        }
    }
}

void threshold(pixel **& pix, int height,int width, int c)
{
	if ((c < 0) || (c > 100))
	{
		printf("\n gia tri ban nhap khong phu hop 0 < c < 100 ");
		return;
	};
	double x = c*255.0 / 100.0; //quy dôi c về gia tri nguong theo gia tri xam
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			//lay gia tri xam trung binh cua tung diem anh
			unsigned char val = (pix[i][j].B + pix[i][j].G + pix[i][j].R) / 3; 
			val = val < x ? 255 : 0; // nếu nhỏ hơn x thì là trắng, ngược lại là đen
			pix[i][j].B = pix[i][j].G = pix[i][j].R = val;
		}
}

int main (){
    char anh[] ="C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/blur2.bmp";
    char kq[] = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/hist.bmp";
    bmpinfor bmifor;
    unsigned char* data = NULL;
    unsigned char* data1 = NULL;
    pixel** pix;
    readbmp(anh,bmifor,data);
    pix = convert_data_pixel(bmifor, data);
    int width = *(int*)bmifor.width;
    int height = *(int*)bmifor.height;
    int photo_size = width * height;
    int n;

	// grayscale(pix, height, width);
	// inc_brightness(pix, height, width, n);
	// inc_contrast(pix, height, width, n);
	// meanfilter(pix,height,width);
	// histogramequalization(pix,height,width);
	// threshold(pix,height,width,n);

    data1 = convert_pixel_data(bmifor, pix);
    savebmp(kq,bmifor,data1);
    delete[] data; delete[] data1; delete[] pix;
	fflush(stdin);
}