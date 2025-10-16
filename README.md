# LibQOI

C++ library for reading and writing QOI files.

# Usage

To read a file use LibQOI::Decode();
To write a file use LibQOI::Encode();

# Where to find things

You can find include header at headers/LibQOI.hpp.
You can find source code in src/.
You can find CMakeLists.txt at CMake/CMakeLists.txt.

Decode returns a LibQOI::Image and Encode takes a LibQOI::Image.
Raw data is represented using std::string.

# Types

Color is represented by a LibQOI::Color struct:

``` c++
struct LibQOI::Color
{
  uint8_t r, g, b, a;
} __attribute__((packed));
```

Images are represented like this:

```c++
// Row-major top to bottom, left to right.
struct LibQOI::Image
{
  uint32_t width, height;
  Color* data;
  uint8_t colorspace; // 0: sRGB, linear aplha, 1: linear
} __attribute__((packed));
```

# How to build

Use CMake. CMakeLists.txt is at CMake/CMakeLists.txt.
If you are on linux you can also run build which is a script but without the .sh extension.
