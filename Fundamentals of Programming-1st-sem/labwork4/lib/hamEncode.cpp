#include <algorithm>
#include <iostream>
#include <vector>

#include "ham.h"
#include "hamEncode.h"

std::vector<bool> addHammingParity(std::vector<bool> boolBitsArray) {
  short bitCount = 0;

  std::vector<bool> bitArray;
  std::vector<bool> hammingBits;

  int boolBitsArrayCount = boolBitsArray.size() - 1;
  // read bits from the end to then reduce the size of the vector boolBitsArray,
  // because. read bits are no longer needed, and we release the memory
  // allocated under vector boolBitsArray
  for (; boolBitsArrayCount > -1; boolBitsArrayCount--) {
    if (bitCount < kCntDataBits) {
      bitArray.push_back(boolBitsArray[boolBitsArrayCount]);
      bitCount++;
    }
    if (bitCount == kCntDataBits) {
      boolBitsArray.resize(boolBitsArrayCount + 1);
      std::vector<bool>(boolBitsArray).swap(boolBitsArray);

      // Because. read bits from the end, then restore the order
      std::reverse(bitArray.begin(), bitArray.end());

      // Create Hamming code (bitsHamming) with empty parity bits
      std::vector<bool> bitsHamming;
      // short cntBitsHamm = cntBitsHamming(bitCount);  // kCntBitsHamming
      for (int i = 0; i < kCntBitsHamming; i++) bitsHamming.push_back(0);

      int vi = 0;
      for (short i = 0; i < kCntBitsHamming; i++) {
        if (isParityIdx(i))  // This is a parity bit
          bitsHamming[i] = 0;
        else {
          bitsHamming[i] = bitArray[vi];
          vi++;
        }
      }
      // Calculate parity for each parity bit,
      // which are in situ multiple degrees 2 (i *= 2)
      for (int i = 1; i <= kCntBitsHamming; i *= 2) {
        bool p = false;  // Флаг по умолчанию для бита чётности
        int pjump = 0;
        // run on bits for each parity bit
        for (int ic = i - 1; ic < kCntBitsHamming; ic++) {
          if (bitsHamming[ic] == 1) p = !p;
          if (++pjump == i) {
            ic += pjump;  // Moving to the next group of bits equal to i
            pjump = 0;
          }
        }
        if (i <= kCntBitsHamming)
          bitsHamming[i - 1] = (short int)
              p;  // Write the resulting parity bit into the Hamming code
      }

      std::reverse(bitsHamming.begin(), bitsHamming.end());

      hammingBits.insert(hammingBits.end(), bitsHamming.begin(),
                         bitsHamming.end());
      bitCount = 0;
      bitArray.clear();
    }
  }

  std::reverse(hammingBits.begin(), hammingBits.end());
  return hammingBits;
}

void applyHamming(std::basic_string<char> flNameFrom, std::string flNameTo) {
  std::cout << "Start applyHamming..." << std::endl;

  std::vector<unsigned char> vectReadFileBytes = readFileBytes(flNameFrom);
  std::vector<bool> vectBytesToBits = bytesToBits(vectReadFileBytes);
  std::vector<bool> vectAddHammingParity = addHammingParity(vectBytesToBits);
  std::vector<unsigned char> vectBitsToBytes =
      bitsToBytes(vectAddHammingParity, 1);
  writeFileBytes(flNameTo, vectBitsToBytes);

  // writeFileBytes(flNameTo,bitsToBytes(addHammingParity(bytesToBits(readFileBytes(flNameFrom))),
  // 1));

  std::cout << "  Done" << std::endl;
}