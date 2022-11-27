#include <algorithm>
#include <fstream>
#include <iterator>
#include <vector>

#include "ham.h"

// Returns true if the index is the parity bit index in the vector
bool isParityIdx(short& i) {
  if (i == 0 || i == 1 || i == 3 || i == 7 || i == 15 || i == 31 || i == 63 ||
      i == 123)  // Индексы бита чётности
    return true;
  else
    return false;
}
// Returns the length of the Hamming code depending on the number of data bits
// short cntBitsHamming(short db) {
//  short pb;
//  if (db <= 247) pb = 8;
//  if (db <= 120) pb = 7;
//  if (db <= 57) pb = 6;
//  if (db <= 26) pb = 5;
//  if (db <= 11) pb = 4;
//  if (db <= 4) pb = 3;
//  if (db == 1) pb = 2;
//  return db + pb;
//}

std::vector<unsigned char> bitsToBytes(std::vector<bool>& bitsArray,
                                       int byteAdjust) {
  if (byteAdjust == 1) {
    int boolBitsArraySize = bitsArray.size();
    if ((boolBitsArraySize % 8) > 0) {
      int adjustFactor =
          ((((int)boolBitsArraySize / 8) + 1) * 8) - boolBitsArraySize;
      for (int z = 0; z < adjustFactor; z++) {
        bitsArray.push_back(0);
      }
    }
  }

  int bit8Count = 0;
  std::vector<bool> bit8Array;
  unsigned char charByte;
  std::vector<unsigned char> bytesArray;

  int boolBitsArrayCount = bitsArray.size() - 1;
  for (; boolBitsArrayCount > -1; boolBitsArrayCount--) {
    if (bit8Count < 8) {
      bit8Array.push_back(bitsArray[boolBitsArrayCount]);
      bit8Count++;
    }

    if (bit8Count == 8) {
      bitsArray.resize(boolBitsArrayCount + 1);

      std::vector<bool>(bitsArray).swap(bitsArray);

      charByte = (bit8Array[0]) | (bit8Array[1] << 1) | (bit8Array[2] << 2) |
                 (bit8Array[3] << 3) | (bit8Array[4] << 4) |
                 (bit8Array[5] << 5) | (bit8Array[6] << 6) |
                 (bit8Array[7] << 7);

      bytesArray.push_back(charByte);
      bit8Array.clear();
      bit8Count = 0;
    }
  }

  std::reverse(bytesArray.begin(), bytesArray.end());

  return bytesArray;
}

std::vector<bool> bytesToBits(std::vector<unsigned char>& bytesArray) {
  std::vector<bool> boolBitsArray;
  std::vector<bool> oneByteBits;
  int bytesArrayCount = bytesArray.size() - 1;

  // convert 1 byte to 8 bits and reduce the size of the bytesArray vector
  for (; bytesArrayCount > -1; bytesArrayCount--) {
    for (int o = 7; o >= 0; o--) {
      oneByteBits.push_back(((bytesArray[bytesArrayCount] >> o) & 1));

      bytesArray.resize(bytesArrayCount + 1);
      std::vector<unsigned char>(bytesArray).swap(bytesArray);
    }

    std::reverse(oneByteBits.begin(), oneByteBits.end());

    boolBitsArray.insert(std::end(boolBitsArray), std::begin(oneByteBits),
                         std::end(oneByteBits));
    oneByteBits.clear();
  }

  std::reverse(boolBitsArray.begin(), boolBitsArray.end());

  return boolBitsArray;
}

std::vector<unsigned char> readFileBytes(const std::string& fileName) {
  std::ifstream file(fileName, std::ios::binary);

  file.unsetf(std::ios::skipws);

  std::streampos fileSize;

  file.seekg(0, std::ios::end);
  fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  std::vector<unsigned char> vec;
  vec.reserve(fileSize);

  vec.insert(vec.begin(), std::istream_iterator<unsigned char>(file),
             std::istream_iterator<unsigned char>());

  return vec;
}

void writeFileBytes(std::string& fileName,
                    std::vector<unsigned char>& fileBytes) {
  std::ofstream file(fileName, std::ios::out | std::ios::binary);

  std::copy(fileBytes.cbegin(), fileBytes.cend(),
            std::ostream_iterator<unsigned char>(file));
}