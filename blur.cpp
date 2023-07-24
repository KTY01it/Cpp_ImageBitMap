#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

void insertionSort(int arr[], int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

int main()
{
    int window[9], row = 0, col = 0, numrows = 0, numcols = 0, MAX = 0;
    ifstream infile("ketqua.bmp", ios::binary);
    ofstream outfile("blur.bmp", ios::binary);

    // Đọc và ghi thông tin file BMP
    char bmpHeader[54];
    infile.read(bmpHeader, 54);
    outfile.write(bmpHeader, 54);

    // Đọc thông tin kích thước ảnh
    numcols = *(int*)&bmpHeader[18];
    numrows = *(int*)&bmpHeader[22];
    MAX = 255;

    // Print total number of rows, columns, and maximum intensity of the image
    cout << numcols << " columns and " << numrows << " rows" << endl << "Maximum Intensity " << MAX << endl;

    // Tạo mảng mới để lưu kết quả
    int** resultArray = new int*[numrows];
    for (int i = 0; i < numrows; ++i)
    {
        resultArray[i] = new int[numcols];
    }

    // Đọc dữ liệu ảnh từ file
    for (row = 0; row < numrows; ++row)
    {
        for (col = 0; col < numcols; ++col)
        {
            // Đọc dữ liệu pixel
            infile.read(reinterpret_cast<char*>(&resultArray[row][col]), sizeof(int));
        }
    }

    // Áp dụng mean filter cho các pixel trong vùng nội tâm của ảnh
    for (row = 1; row < numrows - 1; ++row)
    {
        for (col = 1; col < numcols - 1; ++col)
        {
            // Lấy giá trị pixel lân cận
            window[0] = resultArray[row - 1][col - 1];
            window[1] = resultArray[row - 1][col];
            window[2] = resultArray[row - 1][col + 1];
            window[3] = resultArray[row][col - 1];
            window[4] = resultArray[row][col];
            window[5] = resultArray[row][col + 1];
            window[6] = resultArray[row + 1][col - 1];
            window[7] = resultArray[row + 1][col];
            window[8] = resultArray[row + 1][col + 1];

            // Sắp xếp mảng window
            insertionSort(window, 9);

            // Tính giá trị trung bình
            int sum = 0;
            for (int i = 0; i < 9; ++i)
            {
                sum += window[i];
            }
            int mean = sum / 9;

            // Gán giá trị trung bình vào mảng kết quả
            resultArray[row][col] = mean;
        }
    }

    // Ghi mảng kết quả vào file đầu ra
    for (row = 0; row < numrows; ++row)
    {
        for (col = 0; col < numcols; ++col)
        {
            // Lưu giá trị pixel kết quả vào file
            outfile.write(reinterpret_cast<char*>(&resultArray[row][col]), sizeof(int));
        }
    }

    // Giải phóng bộ nhớ
    for (int i = 0; i < numrows; ++i)
    {
        delete[] resultArray[i];
    }
    delete[] resultArray;

    infile.close();
    outfile.close();

    return 0;
}
