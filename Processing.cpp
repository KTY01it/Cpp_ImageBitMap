#include <fstream>
#include <iostream>
#include <cstdio>

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

void grayscale(pixel**&pix,int height,int width )
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			// lấy giá trị xám trung bình
int val = (pix[i][j].B + pix[i][j].G + pix[i][j].R) / 3; 
			//tat ca cac gia tri RGB ve mot do sang
pix[i][j].B = pix[i][j].G = pix[i][j].R = val;   
		}
};

pixel ** zoom_in(bmpinfor &bmifor,pixel**&pix, int c)
{
	//try xuat gia tri can dung
	int width = *(int*)bmifor.width; //chiều ngang
	int height = *(int*)bmifor.height; //chiều dọc
	
	//tinh toan chieu dai chieu doc sau khi phóng
	int width_after = width*c;
	int height_after = height*c;
	
	//cap phat mang pixel moi co kich co gap C^2 mang ban dau
	pixel**pixafter = new pixel*[height_after];
	for (int i = 0; i < height_after; i++)
	{
		pixafter[i] = new pixel[width_after];
	};
	
	//tao nen trang
	for (int i = 0; i < height_after; i++)
		for (int j = 0; j < width_after; j++)
		{
			pixafter[i][j].B = pixafter[i][j].G = pixafter[i][j].R = 255; 
		};
	
	// tang kich co cua moi pixel
	for (int i = 0; i < height_after; i += c)
		 for (int j = 0; j < width_after; j += c)
		 {
			 for (int x = i; x < i + c; x++)     //moi pix xel sa khi zoom bang C^2 pixel ban dau
				 for (int y = j; y < j + c; y++)  
				 {
					 pixafter[x][y].B = pix[i/c][j/c].B;
					 pixafter[x][y].G = pix[i/c][j/c].G;
					 pixafter[x][y].R = pix[i/c][j/c].R;
				 }
		 }
	
	return pixafter;
}

unsigned char* convert_int_uc(int x)
{
	unsigned char *s = new unsigned char[4];
	s[0] = x % 256;
	s[1] = (x / 256) % 256;
	s[2] = (x / (256 * 256)) % 256;
	s[3] = (x / (256 * 256 * 256)) % 256;
	return s;
}

pixel** zoom_out(bmpinfor &bmifor, pixel **&pix, int c)
{
	//truy xuat gia tri can dung
	int width = *(int*)bmifor.width;
	int height = *(int*)bmifor.height;
	
	//kich co moi
	int width_after = width*c;
	int height_after = height*c;
	
	// cap phat mang pixel moi
	pixel**pixafter = new pixel*[height_after];
	for (int i = 0; i < height_after; i++)
	{
		pixafter[i] = new pixel[width_after];
	};
	
	//tao nen den
	for (int i = 0; i < height_after; i++)
		for (int j = 0; j < width_after; j++)
		{
			pixafter[i][j].B = pixafter[i][j].G = pixafter[i][j].R = 0;
		};
	
	//gán ảnh cũ về chính giữa của ảnh mới
	int w_offset = (width_after - width) / 2; //do lech chieu ngang giữa 2 anh
	int h_offset = (height_after - height) / 2; //do lenh chieu doc 
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			pixafter[i+h_offset][j+w_offset].B = pix[i][j].B;
			pixafter[i+h_offset][j+w_offset].G = pix[i][j].G;
			pixafter[i+h_offset][j+w_offset].R = pix[i][j].R;
		};
	
	// gan lai gia tri ngang doc moi vao bminfor
	unsigned char *w = convert_int_uc(width_after);
	unsigned char *h = convert_int_uc(height_after);
	for (int i = 0; i < 4; i++)
	{
	bmifor.width[i] = w[i];
	bmifor.height[i] = h[i];
	};
	return pixafter;
}

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

void dec_brightness(pixel**&pix, int height,int width, int c)
{
	if ((c<0) || (c>255)) // kiem tra gia tri C
	{
		printf(" ban da nhap sai chi nhap cac gia tri 0<c<255\n");
		return;
	};
	// giam do sang cua cac pixel them 1 gia tri c, 
       //gia tri sau khi giam phai lon hon 0
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			pix[i][j].B = (((pix[i][j].B - c) < 0) ? 0 : (pix[i][j].B - c));
			pix[i][j].G = (((pix[i][j].G - c) < 0) ? 0 : (pix[i][j].G - c));
			pix[i][j].R = (((pix[i][j].R - c) < 0) ? 0 : (pix[i][j].R - c));
		};
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

void dec_contrast(pixel**&pix, int height,int width, int c)
{
	if ((c < 0) || (c > 100))
	{
		printf("\n gia tri ban nhap khong phu hop 0 < c < 100 ");
		return;
	};
	double contrast = ((100.0 - c) / 100.0)*((100.0 - c) / 100.0); // tinh do tuong phan
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
		}
};

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
			val = val > x ? 255 : 0; //neu lon hom c thì la trang nguoc lai la den
			pix[i][j].B = pix[i][j].G = pix[i][j].R = val;
		}
}

int main()
{
	//khu vuc huong dan su dung
	printf("======================================================\n");
	printf("=================== Danh sach cach lenh===============\n");
	printf("ma so              lenh                               \n");
	printf(" 1         phong to anh _ zoom out                    \n");
	printf(" 2         thu nho anh _ zoom in                      \n");
	printf(" 3         tang do sang increase _ brightness         \n");
	printf(" 4         giam do sang decrease _ brightness         \n");
	printf(" 5         tang do tuong phan _ increase contrast     \n");
	printf(" 6         giam do tuong phan _ decrease contrast     \n");
	printf(" 7         tao anh den trang _ make gray photo        \n");
	printf(" 8         phan nguong _ threshold                    \n");
	printf("======================================================\n");
	char s[20];
	printf(" dua file can xu ly vao file editphoto/editphoto \n");
	printf("nhap ten file can xu ly:  "); scanf("%s", &s); fflush(stdin);
	
	//nhap xuat du lieu de xu ly
	bmpinfor bmifor;
	unsigned char* data = NULL;
	pixel** pix;
	readbmp(s, bmifor, data);
	pix = convert_data_pixel(bmifor, data);
	int width = *(int*)bmifor.width;
	int height = *(int*)bmifor.height;
	int photo_size = width * height;
	
	
	// khu vưc goi cac ham de xu ly
	int chose;
	printf(" nhap ham can thuc hien bang ma so: "); scanf("%d", &chose);
	switch (chose)
	{
		case 1 : //chon lenh zoom out
		{
			int n;
		printf("nhap tham so phong to n lan 1 < n < 10: "); scanf("%d", &n);
			pix = zoom_in(bmifor, pix, n);
			break;
		};
		
		case 2 : //chon lenh zoom in
		{
			int n;
		printf("nhap tham so thu nho n lan 1 < n < 10: "); scanf("%d", &n);
			pix = zoom_out(bmifor, pix, n);
			break;
		}
		case 3 : //chon lenh tang do sang
		{
			int n;
			printf("nhap tham so tang do sang 0<n<255: "); scanf("%d", &n);
			inc_brightness(pix, height, width, n);
			break;
		}
		case 4: //chon lenh giam do sang
		{
			int n;
			printf("nhap tham so giam do sang 0<n<255: "); scanf("%d", &n);
			dec_brightness(pix, height, width, n);
			break;
		}
		case 5: //chon lenh tang do tuong phan
		{
			int n;
		printf("nhap tham so tang do tuong phan 0<n<100: "); scanf("%d", &n);
			inc_contrast(pix, height, width, n);
			break;
		}
		case 6: //chon lenh giam do tuong phan
		{
			int n;
		printf("nhap tham so giam do tuong phan 0<n<100: "); scanf("%d", &n);
			dec_contrast(pix, height, width, n);
			break;
		}
		case 7: //chon lenh chuyen anh den trang
		{
			grayscale(pix, height, width);
			break;
		}
		case 8: //chon lenh phân nguong
		{
			int n;
			printf("nhap tham so phan nguong 0<n<100: "); scanf("%d", &n);
			threshold(pix,height,width,n);
			break;
		}
	}
	printf(" xem file ket qua tai file ketqua.bmp trong cung thu muc\n");
	printf(" nhan phim bat ki de thoat\n");
	// lưu tru cac gia tri de save ảnh
	unsigned char* data1 = NULL;
	
	data1 = convert_pixel_data(bmifor, pix);
	savebmp("ketqua.bmp", bmifor, data1);
	delete[] data; delete[] data1; delete[] pix;
	fflush(stdin);
	printf("Press any key to continue...");
	int key = getchar();
	printf("You pressed the key: %d\n", key);
	
}


