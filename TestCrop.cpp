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

pixel** cropImage(bmpinfor& bmifor, pixel**& pix, int startX, int startY, int endX, int endY)
{
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
}

int main() {
    const char* inputFilePath = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/pic_crop.bmp";
    const char* outputFilePath = "C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/crop_0.bmp";
    const int startX = 106;  // Vị trí x bắt đầu cắt
    const int startY = 0;  // Vị trí y bắt đầu cắt
    const int endX = 146;    // Vị trí x kết thúc cắt
    const int endY = 52;    // Vị trí y kết thúc cắt

    bmpinfor bmpInfo;
    unsigned char* data = nullptr;
    pixel** pix = nullptr;

    readbmp(inputFilePath, bmpInfo, data);
    pix = convert_data_pixel(bmpInfo, data);
	// int width = *(int*)bmpInfo.width;
	// int height = *(int*)bmpInfo.height;

    // Cắt ảnh
    pixel** croppedPix = cropImage(bmpInfo, pix, startX, startY, endX, endY);
    bmpinfor croppedBmpInfo = bmpInfo;
    *(int*)croppedBmpInfo.width = endX - startX;
    *(int*)croppedBmpInfo.height = endY - startY;

    // Lưu ảnh đã cắt
    unsigned char* croppedData = convert_pixel_data(croppedBmpInfo, croppedPix);
    savebmp(outputFilePath, croppedBmpInfo, croppedData);

    // Giải phóng bộ nhớ
    delete[] data;
    for (int i = 0; i < *(int*)bmpInfo.height; i++) {
        delete[] pix[i];
    }
    delete[] pix;
    for (int i = 0; i < *(int*)croppedBmpInfo.height; i++) {
        delete[] croppedPix[i];
    }
    delete[] croppedPix;
    delete[] croppedData;

    return 0;
}