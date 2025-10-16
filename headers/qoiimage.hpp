#pragma once
#include <stdint.h>

namespace LibQOI
{
    struct Color
    {
        uint8_t r, g, b, a;

        bool operator==(Color other)
        {
            return r == other.r && g == other.g && b == other.b && a == other.a;
        }
    } __attribute__((packed));

    struct Image
    {
        uint32_t width, height;
        Color *data; // HEAP ALLOCATED, REMEMBER TO DELETE AFTER USE
        uint8_t colorspace; // 0: sRGB, linear aplha, 1: linear
    } __attribute__((packed));

}
