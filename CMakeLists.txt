cmake_minimum_required(VERSION 3.14)
project(BMP_STUFF)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_EXE main.cpp)
set(SOURCE_LIB src/2D/bstree.cpp src/2D/circle.cpp src/2D/convex2D.cpp src/2D/dcel.cpp src/2D/ellipse.cpp src/2D/event.cpp
        src/2D/line2D.cpp src/2D/point2D.cpp src/2D/polygon.cpp src/2D/ray2D.cpp src/2D/rectangle.cpp src/2D/segment2D.cpp
        src/2D/shape2D.cpp src/2D/square.cpp src/2D/triangle.cpp src/2D/vector2D.cpp src/2D/voronoi2D.cpp src/splash_utils.cpp
        WinLib.cpp RGB.cpp)
add_library(voronoi STATIC ${SOURCE_LIB})
add_executable(BMP_STUFF main.cpp)
target_link_libraries(BMP_STUFF voronoi)
