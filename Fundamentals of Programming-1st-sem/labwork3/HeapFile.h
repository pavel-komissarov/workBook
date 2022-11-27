#pragma once
#include <string>
#include <vector>

struct Heap {
  int x, y;
  uint64_t cnt;
  std::vector<Heap> m_heap; // [(x, y, cnt), ...]
  Heap();
  Heap(int x, int y, uint64_t cnt);
  ~Heap(); //destructor
};

std::vector<Heap> importHeapFile(std::basic_string<char>& path_heap);