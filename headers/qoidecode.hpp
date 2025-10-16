#pragma once
#include <libqoi.hpp>
#include <string>

namespace LibQOI
{
    /*
    Takes an unprocessed image data and returns an image with real RGBA values
    */
    Image Decode(std::string data);
}
