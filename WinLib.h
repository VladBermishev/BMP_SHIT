

#ifndef BMP_STUFF_WINLIB_H
#define BMP_STUFF_WINLIB_H

#include <string>
#include <windows.h>
#include <vector>
#include <algorithm>
#include "Constants.h"

char *CreateDir(const char *DirName);

void CreateBmp24(const char *fname, const std::vector<std::pair<int, int>> &points);

#endif //BMP_STUFF_WINLIB_H

