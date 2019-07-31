//@TODO Voronov diagram
//@TODO Input main arguments
//@TODO Colors
//@TODO WinBash Encoding (str)
#include <iostream>
#include <random>
#include <fstream>
#include <ctime>
#include "WinLib.h"
#include "src/2D/voronoi2D.hpp"





void log(const std::vector<std::pair<int, int>> &vec, int index) {
    std::ofstream file_out("log.txt", std::ios_base::app);
    file_out << "------------" << index << "------------\n";
    for (size_t i = 0; i < vec.size(); ++i) {
        file_out << i + 1 << ". x:" << vec[i].first << ", y:" << vec[i].second << '\n';
    }
    file_out.close();
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    std::string exec_dir(CreateDir("PHOTO"));
    std::mt19937 rng(time(nullptr));
    std::uniform_int_distribution<int> x_coord(0, constants::X_SIZE - 1);
    std::uniform_int_distribution<int> y_coord(0, constants::Y_SIZE - 1);
    for (int i = 1; i <= constants::PHOTOS_AMOUNT; ++i) {
        std::string filename = exec_dir + '\\' + std::to_string(i) + ".bmp";
        std::vector<std::pair<int, int>> points(constants::POINTS_AMOUNT, {-1, -1});
        for (int j = 0; j < constants::POINTS_AMOUNT; ++j) {
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
        std::cout << (float) (i) / constants::PHOTOS_AMOUNT * 100 << '\n';
    }
    return 0;
}
