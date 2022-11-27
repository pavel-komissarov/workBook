#include <cstdlib>

#include <filesystem>
#include <iostream>

#include "HeapFile.h"
#include "bmp.h"

void CreateBmp(const std::vector<std::vector<uint64_t>>& vect_heap,
               std::basic_string<char> path_bmp, const int& iteration,
               const std::vector<Heap>& vh) {
  //let's translate iteration into string
  char buffer[20];
  _itoa_s(iteration, buffer, 10); // number to text conversion
  std::string s = static_cast<std::string>(buffer);

  path_bmp += "bmp" + s + ".bmp";
  // creating a bmp file
  int width = vect_heap.size();
  int heigth = vect_heap[0].size();
  Image image(width, heigth);
  Color clr(0, 0, 0);
  uint64_t cnt;
  for (int y = 0; y < heigth; y++) {
    for (int x = 0; x < width; x++) {
      cnt = vect_heap[x][y];
      if (cnt == 0) clr = white;
      if (cnt == 1) clr = green;
      if (cnt == 2) clr = purple;
      if (cnt == 3) clr = yellow;
      if (cnt > 3) clr = black;
      image.SetColor(clr, x, y); //set color for dot
    }
  }
  std::cout << '\r' << "Count of unstable cells = " << vh.size() << '\n';
  // if the directories do not exist, then creates
  image.Export(path_bmp, iteration);
}

void shift_rigth(std::vector<std::vector<uint64_t>>& vect_heap) {
  //add a new column (y) and fill with 0
  for (int x = 0; x < vect_heap.size() /*width*/; x++) {
    vect_heap[x].push_back(0);
  }
}

void shift_left(std::vector<std::vector<uint64_t>>& vect_heap, const int& x,
                int& y, std::vector<Heap>& vh) {
  // add a new column (y) and fill with 0
  for (int x = 0; x < vect_heap.size() /*width*/; x++) {
    auto iter = vect_heap[x].cbegin();  // a const iterator points to the first element
    vect_heap[x].insert(iter, 0);
  }
  y++;
  // shift the coordinates (y) of unstable points
  for (int i = 0; i < vh.size(); i++) {
    vh[i].y++;
  }
}

void shift_top(std::vector<std::vector<uint64_t>>& vect_heap) {
  // add a new line (x) and fill with 0
  vect_heap.push_back(std::vector<uint64_t>());
  for (int y = 0; y < vect_heap[0].size() /*heigth*/; y++) {
    vect_heap[vect_heap.size() - 1].push_back(0);
  }
}

void shift_down(std::vector<std::vector<uint64_t>>& vect_heap, int& x,
                std::vector<Heap>& vh) {
  // add a new line (x) at the beginning, the rest will move up by themselves
  auto iter = vect_heap.cbegin();  // a const iterator points to the first element
  vect_heap.insert(iter, std::vector<uint64_t>()); // add a new line to the beginning, shifting all the rest

  //fill a new line from the bottom 0
  for (int y = 0; y < vect_heap[1].size(); y++) {
    vect_heap[0].push_back(0);
  }
  x++;
  // shift the coordinates (x) of unstable points
  for (int i = 0; i < vh.size(); i++) {
    vh[i].x++;
  }
}

void Parser(const int& argc, char* argv[],
            std::vector<int>& parameters, std::vector<std::string>& paths) {
  for (int i = 1; i < argc; i += 2) {
    std::string str = argv[i];
    if (argv[i][0] == '-' && argv[i][1] == '-') {
      if (str == "--length") parameters.push_back(int(atoi(argv[i + 1])));
      if (str == "--width") parameters.push_back(int(atoi(argv[i + 1])));
      if (str == "--input") paths.push_back(std::string(argv[i + 1]));
      if (str == "--output") paths.push_back(std::string(argv[i + 1]));
      if (str == "--max-iter") parameters.push_back(int(atoi(argv[i + 1])));
      if (str == "--freq") parameters.push_back(int(atoi(argv[i + 1])));
      if (str != "--length" && str != "--width" && str != "--input" &&
          str != "--output" && str != "--max-iter" && str != "--freq") {
        throw " Invalid parameter " + str;
      }
    } else if (argv[i][0] == '-' && argv[i][1] != '-') {
      if (argv[i][1] == 'l') parameters.push_back(int(atoi(argv[i + 1])));
      if (argv[i][1] == 'w') parameters.push_back(int(atoi(argv[i + 1])));
      if (argv[i][1] == 'i') paths.push_back(static_cast<std::string>(argv[i + 1]));
      if (argv[i][1] == 'o') paths.push_back(static_cast<std::string>(argv[i + 1]));
      if (argv[i][1] == 'm') parameters.push_back(int(atoi(argv[i + 1])));
      if (argv[i][1] == 'f') parameters.push_back(int(atoi(argv[i + 1])));
      if (str != "-l" && str != "-w" && str != "-i" &&
          str != "-o" && str != "-m" && str != "-f") {
        throw " Invalid parameter " + str;
      }
    }
  }
}

int main(int argc, char* argv[]) {
  std::vector<int> parameters;
  std::vector<std::string> paths;

  if (argc == 1) {
    std::cout << "Not parameters\n";
    return 1;
  }

  try {
    Parser(argc, argv, parameters, paths);
  } catch (std::string err) {
    std::cout << err << std::endl;
    return 1;
  }

  int width = parameters[0];
  int heigth = parameters[1];
  const int maxiter = parameters[2];
  int freq = maxiter; // save frequency if -l = 0
  if (parameters[3] != 0) freq = maxiter / parameters[3];

  std::string path_heap = paths[0];
  std::string path_bmp = paths[1]; 
  std::filesystem::path dir{path_bmp}; // write a path
  // clear dir with bmp
  if (exists(dir)) { // existence check
    std::cout << "Clear dir " << path_bmp << std::endl;
    remove_all(dir);
    std::cout << " - Done\n ";
  }

  std::cout << "Initialize the array " << width << "x" << heigth
            << " with data from the file " << path_heap << std::endl;

  //A two-dimensional vector here in the rows will lie x, and in the columns y
  std::vector<std::vector<uint64_t>> vect_heap;

  //Заполняем его нулями
  for (int i = 0; i < width; i++) {
    // add a new line
    vect_heap.push_back(std::vector<uint64_t>());
    for (int j = 0; j < heigth; j++) {
      vect_heap[i].push_back(0);
    }
  }

  std::vector<Heap> vectorOfСell;

  try {
    vectorOfСell = importHeapFile(path_heap);
  } catch (std::string err) {
    std::cout << err << std::endl;
    return 1;
  }

  try {
    //write the resulting vector into a two-dimensional vector (heap)
    for (int i = 0; i < vectorOfСell.size(); i++) {
      vect_heap.at(vectorOfСell.at(i).x).at(vectorOfСell.at(i).y) = vectorOfСell.at(i).cnt;
    }
    std::cout << " - Done\n ";
  } catch (std::out_of_range) {
    std::cout << "Error: Invalid coordinates in " << path_heap << std::endl;
    return 1;
  }

  //bypass the heap in maxiter iterations
  bool stability;
  int last_iteration;

  for (int i = 1; i <= maxiter; i++) {
    std::cout << '\r' << " -> " << i;
    stability = true;
    //clear vectorOfСell to write cells with unstable states there
    //which will then be crushed
    vectorOfСell.clear();

    for (int x = 0; x < vect_heap.size(); x++) {
      for (int y = 0; y < vect_heap[x].size(); y++) {
        uint64_t cnt = vect_heap[x][y];
        if (cnt >= 4) {
          stability = false;
          //here we add unstable cells so that they are synchronized (only these
          //cells) will collapse
          vectorOfСell.push_back(Heap(x, y, cnt));
        }
      }
    }

    for (int i = 0; i < vectorOfСell.size(); i++) {
      int x = vectorOfСell[i].x;
      int y = vectorOfСell[i].y;
      uint64_t cnt = vectorOfСell[i].cnt;

      if ((x + 1) >= vect_heap.size() /*width*/) shift_top(vect_heap);
      if ((y + 1) >= vect_heap[0].size() /*heigth*/) shift_rigth(vect_heap);
      if ((y - 1) < 0) shift_left(vect_heap, x, y, vectorOfСell);
      if ((x - 1) < 0) shift_down(vect_heap, x, vectorOfСell);

      // to reduce iterations, divide by 4 and use the remainder of the division by 4
      vect_heap[x][y - 1] += cnt / 4;
      vect_heap[x][y + 1] += cnt / 4;
      vect_heap[x - 1][y] += cnt / 4;
      vect_heap[x + 1][y] += cnt / 4;
      vect_heap[x][y] = cnt % 4;
//      vect_heap[x][y - 1]++;
//      vect_heap[x][y + 1]++;
//      vect_heap[x - 1][y]++;
//      vect_heap[x + 1][y]++;
//      vect_heap[x][y] -= 4;
    }

    last_iteration = i;

    if (stability) {
      std::cout << '\r' << "The heap is stable\n";
      break;
    }

    if (i % freq == 0 || stability)
      CreateBmp(vect_heap, path_bmp, last_iteration, vectorOfСell);
  }
  //выводим последнее состояние
  if (last_iteration % freq != 0)
    CreateBmp(vect_heap, path_bmp, last_iteration, vectorOfСell);

  std::cout << "The end!";
}
