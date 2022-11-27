#include "bmp.h"
#include <filesystem>
#include <iostream>
#include <fstream>

Color::Color()
    : r(0), g(0), b(0) {
}

Color::Color(float r, float g, float b)
    : r(r), g(g), b(b) {
}

Color::~Color() {
}

Image::Image(){

}

Image::Image(int width, int heigth)
    : width(width), heigth(heigth), m_color(std::vector<Color>(width * heigth)) {
}

Image::~Image() {
}

void Image::SetColor(const Color& color, int& x, int& y) {
  //we write from left to right, and then from bottom to top
  m_color[y * width + x].r = color.r;
  m_color[y * width + x].g = color.g;
  m_color[y * width + x].b = color.b;
}

Color Image::GetColor(int& x, int& y) {   //we read from left to right, and then from bottom to top
  return m_color[y * width + x]; // get color by coordinates
}

void Image::Export(std::basic_string<char>& path_bmp, int iteration) {
  std::cout << "Create " << path_bmp << std::endl;
  std::filesystem::path dir{path_bmp};

  //remove filename from path
  dir.remove_filename();
  //if there are directories, then ignore them, otherwise we will create them using absolute path
  create_directories(absolute(dir));

  //create file for recording in binary mode
  std::ofstream file;
  file.open(path_bmp, std::ios::out | std::ios::binary);

  if (file.is_open()) {
    std::filesystem::path fname;
    fname = dir.filename();
  }

  if (!file.is_open()) {
    std::cout << "File could not be opened" << std::endl;
    return;
  }

  unsigned char bmpAdd[3] = {0, 0, 0};
  //each row must be a multiple of 4 bytes
  //each color is specified by 3 bytes
  //calculate how many bytes we need to add up to a multiplicity of 4
  //if (width * 3) % 4=4, then 4 - (width * 3) % 4 = 0, which we don't need, so next we calculate the remainder of the division by 4
  const int kAddAmount = ((4 - (width * 3) % 4) % 4);

  const int kHeadOfFileSize = 14;
  const int kInfHeaderSize = 40;
  const int kFileSize = kHeadOfFileSize + kInfHeaderSize + width * heigth * 3 + kAddAmount * heigth;

  unsigned char headOfFile[kHeadOfFileSize];
  //File type
  headOfFile[0] = 'B';
  headOfFile[1] = 'M';
  //File size
  headOfFile[2] = kFileSize; // сдвиг
  headOfFile[3] = kFileSize >> 8;
  headOfFile[4] = kFileSize >> 16;
  headOfFile[5] = kFileSize >> 24;
  //Reserved 1 (not used)
  headOfFile[6] = 0;
  headOfFile[7] = 0;
  //Reserved 2 (not used)
  headOfFile[8] = 0;
  headOfFile[9] = 0;
  //Pixel data offset
  headOfFile[10] = kHeadOfFileSize + kInfHeaderSize; // Offset, i.e. the starting address of the byte at which
  headOfFile[11] = 0;                                      // can find bitmap data (array of pixels)
  headOfFile[12] = 0;
  headOfFile[13] = 0;

  unsigned char infHeaderSize[kInfHeaderSize];
  //Header size
  infHeaderSize[0] = kInfHeaderSize;
  infHeaderSize[1] = 0;
  infHeaderSize[2] = 0;
  infHeaderSize[3] = 0;
  //Bmp width
  infHeaderSize[4] = width;
  infHeaderSize[5] = width >> 8;
  infHeaderSize[6] = width >> 16;
  infHeaderSize[7] = width >> 24;
  //Bmp heigth
  infHeaderSize[8] = heigth;
  infHeaderSize[9] = heigth >> 8;
  infHeaderSize[10] = heigth >> 16;
  infHeaderSize[11] = heigth >> 24;
  //Planes
  infHeaderSize[12] = 1;
  infHeaderSize[13] = 0;
  //Bits per pyxel (RGB)
  infHeaderSize[14] = 24;
  infHeaderSize[15] = 0;
  //Compression (No compression)
  infHeaderSize[16] = 0;
  infHeaderSize[17] = 0;
  infHeaderSize[18] = 0;
  infHeaderSize[19] = 0;
  //Image size (No compression)
  infHeaderSize[20] = 0;
  infHeaderSize[21] = 0;
  infHeaderSize[22] = 0;
  infHeaderSize[23] = 0;
  //X pyxels per meter (Not specified)
  infHeaderSize[24] = 0;
  infHeaderSize[25] = 0;
  infHeaderSize[26] = 0;
  infHeaderSize[27] = 0;
  //Y pyxels per meter (Not specified)
  infHeaderSize[28] = 0;
  infHeaderSize[29] = 0;
  infHeaderSize[30] = 0;
  infHeaderSize[31] = 0;
  //Total colors (Color palette not used)
  infHeaderSize[32] = 0;
  infHeaderSize[33] = 0;
  infHeaderSize[34] = 0;
  infHeaderSize[35] = 0;
  //Important colors (Generally ignored)
  infHeaderSize[36] = 0;
  infHeaderSize[37] = 0;
  infHeaderSize[38] = 0;
  infHeaderSize[39] = 0;

  file.write((const char*) (headOfFile), kHeadOfFileSize);
  file.write((const char*) (infHeaderSize), kInfHeaderSize);

  for (int y = 0; y < heigth; y++) {
    for (int x = 0; x < width; x++) {
      unsigned char r = GetColor(x, y).r; // get color value
      unsigned char g = GetColor(x, y).g;
      unsigned char b = GetColor(x, y).b;

      unsigned char color[] = {b, g, r}; // change r with b

      file.write((const char*) (color), 3);
    }
    file.write((const char*) (bmpAdd), kAddAmount); // multiplicity addition
  }
  file.close();
  std::cout << " - Done\n";
}
