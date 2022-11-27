#pragma once
#include <vector>
#include <string>

struct Color {
  float r;
  float g;
  float b;
  Color();
  Color(float r, float g, float b);
  ~Color();
};

static Color purple = Color((float) 139, (float) 0, (float) 255);
static Color white = Color((float) 255, (float) 255, (float) 255);
static Color green = Color((float) 0, (float) 128, (float) 0);
static Color yellow = Color((float) 255, (float) 255, (float) 0);
static Color black = Color((float) 0, (float) 0, (float) 0);

struct Image {
  int width;
  int heigth;
  Image();
  Image(int width, int heigth);
  ~Image();
  Color GetColor(int& x, int& y);
  void SetColor(const Color& color, int& x, int& y);
  void Export(std::basic_string<char>& path_bmp, int iteration);
  std::vector<Color> m_color;
};