#include <libqoi.hpp>
#include <stdint.h>

uint8_t hash(const LibQOI::Color &c)
{
    return (c.r * 3 + c.g * 5 + c.b * 7 + c.a * 11) % 64;
}