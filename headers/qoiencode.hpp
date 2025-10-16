#pragma once
#include <libqoi.hpp>
#include <string>

namespace LibQOI
{
    /*
    Turns an image into QOI data
    */
    std::string Encode(Image img);
}
