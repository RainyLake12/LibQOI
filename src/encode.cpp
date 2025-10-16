#include <libqoi.hpp>
#include <sstream>
#include <arpa/inet.h>
#include <stdint.h>
#include <util.hpp>
#include <stdio.h>

using std::string;

string LibQOI::Encode(LibQOI::Image img)
{
    Color hashes[64] = {};
    for (int i = 0; i < 64; i++)
    {
        hashes[i] = {0, 0, 0, 0};
    }
    
    std::stringstream stream;
    stream.write("qoif", 4);
    
    uint64_t pixelCount = (uint64_t) img.width * (uint64_t) img.height;
    
    uint32_t width = htonl(img.width), height = htonl(img.height);
    
    stream.write((char*) &width, 4);
    stream.write((char*) &height, 4);
    
    uint8_t channels = 4;
    
    stream.write((char*) &channels, 1);
    
    uint8_t colorspace = img.colorspace;
    
    stream.write((char*) &colorspace, 1);
    
    Color* pixels = img.data;
    Color lastPixel = {0, 0, 0, 255};
    Color currPixel;
    
    if (!pixels)
    {
        return "";
    }
    
    uint64_t index = 0;
    while (index < pixelCount)
    {
        currPixel = pixels[index];

        // RUN

        if (currPixel == lastPixel && pixels[index + 1] == currPixel)
        {
            uint8_t run = 0;
            Color tempPixel = currPixel;
            while (run < 62 && tempPixel == currPixel)
            {
                tempPixel = pixels[index];
                run++;
                index++;
            }

            lastPixel = currPixel;

            stream.put(192 | (run - 1));
            continue;
        }
        
        // INDEX
        
        if (hashes[hash(currPixel)] == currPixel)
        {
            uint8_t byte = hash(currPixel); // No need to OR with opcode because QOI_OP_INDEX is 0
            stream.write((char*) &byte, 1);
            lastPixel = currPixel;
            
            index++;
            
            continue;
        }

        // Variables for DIFF_SMALL and DIFF_LUMA
        int8_t dr = currPixel.r - lastPixel.r;
        int8_t dg = currPixel.g - lastPixel.g;
        int8_t db = currPixel.b - lastPixel.b;

        // DIFF_SMALL

        if (currPixel.a == lastPixel.a)
        {
            if (dr >= -2 && dr <= 1 && dg >= -2 && dg <= 1 && db >= -2 && db <= 1)
            {
                dr += 2; // bias to 0–3
                dg += 2;
                db += 2;

                uint8_t byte = 64
                    | ((dr & 3) << 4)
                    | ((dg & 3) << 2)
                    | (db & 3);

                stream.put(byte);
                hashes[hash(currPixel)] = currPixel;
                lastPixel = currPixel;
                index++;
                continue;
            }
        }
        
        // DIFF_LUMA
        if (currPixel.a == lastPixel.a)
        {
            int8_t dr_dg = dr - dg;
            int8_t db_dg = db - dg;

            if (dg >= -32 && dg <= 31 && dr_dg >= -8 && dr_dg <= 7 && db_dg >= -8 && db_dg <= 7)
            {
                
                // Add biases
                dg += 32;
                dr_dg += 8;
                db_dg += 8;
                
                uint8_t byte1 = 128 | dg;
                uint8_t byte2 = (dr_dg << 4) | db_dg;

                stream.put(byte1);
                stream.put(byte2);

                hashes[hash(currPixel)] = currPixel;
                lastPixel = currPixel;

                index++;
                continue;
            }
        }

        // RGB
        
        if (currPixel.a == lastPixel.a)
        {
            uint8_t r = currPixel.r, g = currPixel.g, b = currPixel.b;
            
            stream.put(254);
            stream.put(currPixel.r);
            stream.put(currPixel.g);
            stream.put(currPixel.b);
            
            hashes[hash(currPixel)] = currPixel;
            lastPixel = currPixel;
            
            index++;
            
            continue;
        }
        
        //RGBA
        
        stream.put(255);
        stream.put(currPixel.r);
        stream.put(currPixel.g);
        stream.put(currPixel.b);
        stream.put(currPixel.a);
        
        hashes[hash(currPixel)] = currPixel;
        lastPixel = currPixel;
        
        index++;
        
        continue;
    }
    
    stream.write("\x00\x00\x00\x00\x00\x00\x00\x01", 8);
    
    return stream.str();
}
