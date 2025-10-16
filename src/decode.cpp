#include <libqoi.hpp>
#include <string>
#include <sstream>
#include <string.h>
#include <vector>
#include <arpa/inet.h>
#include <util.hpp>

using std::stringstream, std::vector, std::string;

int8_t int6_extend(uint8_t num)
{
    return ((int8_t) num << 2) >> 2;
}

int8_t int4_extend(uint8_t num)
{
    return ((int8_t) num << 4) >> 4;
}

int8_t int2_extend(uint8_t num)
{
    return ((int8_t) num << 6) >> 6;
}

LibQOI::Image LibQOI::Decode(string data)
{
    stringstream stream(data, std::ios::binary | std::ios::in);

    Color hashes[64] = {{0, 0, 0, 0}};

    QOIHeader header;
    stream.read((char*) &header, sizeof(QOIHeader));

    header.width = ntohl(header.width);
    header.height = ntohl(header.height);

    uint64_t pixelCount = header.width * header.height;
    vector<Color> pixels;

    pixels.reserve(pixelCount);

    Color lastPixel = {0, 0, 0, 255};

    while (pixels.size() < pixelCount) // fill every pixel
    {
        uint8_t byte;
        stream.read((char*) &byte, 1);
        
        if (byte == 0) {
            uint8_t end_seq[8];
            end_seq[0] = 0;
            stream.read((char*) &end_seq[1], 7);
            if (!memcmp(end_seq, "\x00\x00\x00\x00\x00\x00\x00\x01", 8)) break;
            stream.seekg(-7, std::ios::cur);
        }
        
        if (byte == 255)
        {
            Color color;
            stream.read((char*) &color, sizeof(Color));
            lastPixel = color;
            pixels.push_back(color);

            hashes[hash(color)] = color;

            continue;
        } else if (byte == 254)
        {
            struct
            {
                uint8_t r, g, b;
            } RGB;
            stream.read((char*) &RGB, sizeof(RGB));

            Color color = {RGB.r, RGB.g, RGB.b, lastPixel.a};
            lastPixel = color;
            pixels.push_back(color);

            hashes[hash(color)] = color;

            continue;
        } else if (byte >> 6 == 0)
        {
            Color color = hashes[byte & 63];
            lastPixel = color;
            pixels.push_back(color);

            continue;
        } else if (byte >> 6 == 1)
        {
            Color color = lastPixel;
            color.b += (int) ((byte & 3) - 2);
            color.g += (int) (((byte >> 2) & 3) - 2);
            color.r += (int) (((byte >> 4) & 3) - 2);

            hashes[hash(color)] = color;
            lastPixel = color;
            pixels.push_back(color);

            continue;
        } else if (byte >> 6 == 2)
        {
            Color color;

            uint8_t nextByte;
            stream.read((char*) &nextByte, sizeof(uint8_t));

            int8_t dg, dr_dg, db_dg, dr, db;
            dg = (byte & 63) - 32;
            db_dg = (nextByte & 15) - 8;
            dr_dg = ((nextByte >> 4) & 15) - 8;
            db = db_dg + dg;
            dr = dr_dg + dg;

            color = {
                (uint8_t)(lastPixel.r + dr),
                (uint8_t)(lastPixel.g + dg),
                (uint8_t)(lastPixel.b + db),
                lastPixel.a
            };

            hashes[hash(color)] = color;
            lastPixel = color;
            pixels.push_back(color);

            continue;
        } else if (byte >> 6 == 3)
        {
            Color color = lastPixel;
            uint8_t run = (byte & 63) + 1;

            for (int i = 0; i < run; i++)
            {
                pixels.push_back(color);
            }

            continue;
        }
    }

    Image image;
    image.data = new Color[pixels.size()];
    image.width = header.width;
    image.height = header.height;
    image.colorspace = header.colorspace;
    std::copy(pixels.begin(), pixels.end(), image.data);

    return image;
}
