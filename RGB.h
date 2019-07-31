//
// Created by Vladi on 7/31/2019.
//

#ifndef BMP_STUFF_RGB_H
#define BMP_STUFF_RGB_H

#include <windows.h>

struct rgb {
    BYTE rgbtRed;
    BYTE rgbtGreen;
    BYTE rgbtBlue;

    rgb();

    rgb(BYTE RED, BYTE GREEN, BYTE BLUE);
};

#endif //BMP_STUFF_RGB_H
