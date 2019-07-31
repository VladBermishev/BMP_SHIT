//@TODO Voronov diagram
//@TODO Reformat Code
//@TODO Input main arguments
//@TODO Colors
//@TODO WinBash Encoding (str)
#include "windows.h"
#include <vector>
#include <iostream>
#include <random>
#include <fstream>
#include <algorithm>
#include <ctime>
#include "src/2D/voronoi2D.hpp"
const int PHOTOS_AMOUNT = 100;
const int POINTS_AMOUNT = 10;
const int X_SIZE = 50;
const int Y_SIZE = 100;

struct rgb {
    BYTE rgbtRed;
    BYTE rgbtGreen;
    BYTE rgbtBlue;

    rgb() {}

    rgb(BYTE RED, BYTE GREEN, BYTE BLUE) {
        this->rgbtRed = RED;
        this->rgbtGreen = GREEN;
        this->rgbtBlue = BLUE;
    }
};

void CreateBmp24(const char *fname, const std::vector<std::pair<int, int>> &points);


const rgb colors[] =
        {rgb(255, 0, 0), rgb(255, 20, 147), rgb(0, 255, 0),
         rgb(255, 165, 0), rgb(255, 215, 0), rgb(0, 255, 127),
         rgb(0, 100, 0), rgb(32, 178, 170), rgb(0, 255, 255),
         rgb(25, 25, 112), rgb(192, 192, 192), rgb(255, 0, 255),
         rgb(255, 255, 0), rgb(0, 0, 0), rgb(128, 0, 0),
         rgb(210, 133, 63)};


void log(const std::vector<std::pair<int, int>> &vec, int index) {
    std::ofstream file_out("log.txt", std::ios_base::app);
    file_out << "------------" << index << "------------\n";
    for (size_t i = 0; i < vec.size(); ++i) {
        file_out << i + 1 << ". x:" << vec[i].first << ", y:" << vec[i].second << '\n';
    }
    file_out.close();
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    HMODULE hModule = GetModuleHandleW(NULL);
    TCHAR path[MAX_PATH];
    GetModuleFileNameA(hModule, path, MAX_PATH);
    {
        size_t i = 0;
        size_t n;
        while (path[i] != '\0') {
            if (path[i] == '\\') {
                n = i;
            }
            i++;
        }
        path[n] = '\0';
    }
    std::string exec_dir = strcat(path, "\\PHOTO");
    CreateDirectoryA(exec_dir.c_str(), NULL);
    std::mt19937 rng(time(0));
    std::uniform_int_distribution<int> x_coord(0, X_SIZE - 1);
    std::uniform_int_distribution<int> y_coord(0, Y_SIZE - 1);
    for (int i = 1; i <= PHOTOS_AMOUNT; ++i) {
        std::string filename = exec_dir + '\\' + std::to_string(i) + ".bmp";
        std::vector<std::pair<int, int>> points(POINTS_AMOUNT, {-1, -1});
        for (int j = 0; j < POINTS_AMOUNT; ++j) {
            int x = x_coord(rng);
            int y = y_coord(rng);
            while (std::find(points.begin(), points.end(), std::make_pair(x, y)) != points.end()) {
                x = x_coord(rng);
                y = y_coord(rng);
            }
            points[j] = {x, y};
        }
        log(points, i);
        CreateBmp24(filename.c_str(), points);
        std::cout << (float) (i) / PHOTOS_AMOUNT * 100 << '\n';
    }
    return 0;
}

void CreateBmp24(const char *fname, const std::vector<std::pair<int, int>> &points) {
    HANDLE hFile;
    DWORD RW;
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    BYTE Palette[1024];                                    // Ïàëèòðà
    int Width = X_SIZE;
    int Height = Y_SIZE;
    memset(Palette, 0, 1024);                                // Â ïàëèòðå ó íàñ íóëè
    memset(&bfh, 0, sizeof(bfh));
    bfh.bfType = 0x4D42;                                    // Îáîçíà÷èì, ÷òî ýòî bmp 'BM'
    bfh.bfOffBits = sizeof(bfh) + sizeof(bih) + 1024;        // Ïàëèòðà çàíèìàåò 1Kb, íî ìû åãî èñïîüçîâàòü íå áóäåì
    bfh.bfSize = bfh.bfOffBits +
                 sizeof(colors[0]) * Width * Height +
                 Height * (Width % 4);                        // Ïîñ÷èòàåì ðàçìåð êîíå÷íîãî ôàéëà

    memset(&bih, 0, sizeof(bih));
    bih.biSize = sizeof(bih);                                // Òàê ïîëîæåíî
    bih.biBitCount = 24;                                    // 16 áèò íà ïèêñåëü
    bih.biCompression = BI_RGB;                                // Áåç ñæàòèÿ
    bih.biHeight = Height;
    bih.biWidth = Width;
    bih.biPlanes = 1;                                        // Äîëæíî áûòü 1
    hFile = CreateFile(fname, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
        return;
    WriteFile(hFile, &bfh, sizeof(bfh), &RW, NULL);
    WriteFile(hFile, &bih, sizeof(bih), &RW, NULL);
    WriteFile(hFile, Palette, 1024, &RW, NULL);

    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            rgb color;
            if (std::find(points.begin(), points.end(), std::make_pair(i, j)) != points.end()) {
                color = rgb(0, 0, 0);
            } else {
                int minn = INT_MAX;
                for (int k = 0; k < POINTS_AMOUNT; ++k) {
                    int len = (i - points[k].first) * (i - points[k].first) +
                              (j - points[k].second) * (j - points[k].second);
                    if (len < minn) {
                        minn = len;
                        color = colors[k];
                    }
                }
            }
            WriteFile(hFile, &color, sizeof(color), &RW, NULL);
        }
        WriteFile(hFile, Palette, Width % 4, &RW, NULL);
    }
}
