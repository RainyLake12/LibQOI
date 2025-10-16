#pragma once
#include <stdint.h>
#include <string>

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

    /*
    Takes an unprocessed image data and returns an image with real RGBA values
    */
    Image Decode(std::string data);

    /*
    Turns an image into QOI data
    */
    std::string Encode(Image img);

}

struct QOIHeader {
    char magic[4]; // magic bytes "qoif"
    uint32_t width; // image width in pixels (BE)
    uint32_t height; // image height in pixels (BE)
    uint8_t channels; // 3 = RGB, 4 = RGBA
    uint8_t colorspace; // 0 = sRGB with linear alpha
    // 1 = all channels linear
} __attribute__((packed));
