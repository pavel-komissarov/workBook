#include "HeapFile.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>


Heap::Heap(int x, int y, uint64_t cnt)
    : x(x), y(y), cnt(cnt) {}

Heap::~Heap() {

}

Heap::Heap() {
}

std::vector<Heap> importHeapFile(std::basic_string<char>& path_heap) {
  std::ifstream file;
  std::vector<Heap> m_heap;

  file.open(path_heap, std::ios::in);
  if (!file.is_open()) {
    throw "File " + path_heap + " could not be opened for reading";
  }
  char s[256];
  while (file.good()) {
    file.getline(s, 256);

    Heap heap(0, 0, 0);
    std::string x_str, y_str, cnt_str;
    unsigned char cnt_tab = 0;

    // print string
    // std::cout << s << "\n";

    //'\0' - empty character or end row
    for (int i = 0; s[i] != '\0'; i++) {
      if (s[i] == '\t') cnt_tab++;
      if (cnt_tab == 0) x_str = x_str + s[i];
      if (cnt_tab == 1) {
        if (s[i] == '\t') {
          // heap.x = previous number
          heap.x = stoi(x_str);
          i++;
        }
        y_str = y_str + s[i];
      }
      if (cnt_tab == 2) {
        if (s[i] == '\t') {
          // heap.y = previous number
          heap.y = stoi(y_str);
          i++;
        }
        cnt_str = cnt_str + s[i];
        // if the following is an empty character, then p.cnt = current number
        // (cnt_str)
        if (s[i + 1] == '\0') heap.cnt = stoi(cnt_str);
      }
    }
    heap = Heap(heap.x, heap.y, heap.cnt);
    m_heap.push_back(heap);
  }

  return m_heap;
}