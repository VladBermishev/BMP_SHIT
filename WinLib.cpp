#include "WinLib.h"

char *CreateDir(const char *DirName) {
    HMODULE hModule = GetModuleHandleW(nullptr);
    TCHAR path[MAX_PATH];
    GetModuleFileNameA(hModule, path, MAX_PATH);
    size_t i = 0;
    size_t n = 0;
    while (path[i] != '\0') {
        if (path[i] == '\\') {
            n = i;
        }
        i++;
    }
    path[n] = '\0';
    char *exec_dir = strcat(strcat(path, "\\"), DirName);
    CreateDirectoryA(exec_dir, nullptr);
    return exec_dir;
}


void CreateBmp24(const char *fname, const std::vector<std::pair<int, int>> &points) {
    HANDLE hFile;
    DWORD RW;
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    BYTE Palette[1024];
    int Width = constants::X_SIZE;
    int Height = constants::Y_SIZE;
    memset(Palette, 0, 1024);
    memset(&bfh, 0, sizeof(bfh));
    bfh.bfType = 0x4D42;
    bfh.bfOffBits = sizeof(bfh) + sizeof(bih) + 1024;
    bfh.bfSize = bfh.bfOffBits +
                 sizeof(constants::colors[0]) * Width * Height +
                 Height * (Width % 4);

    memset(&bih, 0, sizeof(bih));
    bih.biSize = sizeof(bih);
    bih.biBitCount = 24;
    bih.biCompression = BI_RGB;
    bih.biHeight = Height;
    bih.biWidth = Width;
    bih.biPlanes = 1;
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
                for (int k = 0; k < constants::POINTS_AMOUNT; ++k) {
                    int len = (i - points[k].first) * (i - points[k].first) +
                              (j - points[k].second) * (j - points[k].second);
                    if (len < minn) {
                        minn = len;
                        if (k < 16) {
                            color = constants::colors[k];
                        }
                    }
                }
            }
            WriteFile(hFile, &color, sizeof(color), &RW, NULL);
        }
        WriteFile(hFile, Palette, Width % 4, &RW, NULL);
    }
}