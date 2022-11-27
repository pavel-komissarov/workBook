#include <algorithm>
#include <iostream>
#include <vector>

#include "ham.h"
#include "hamDecode.h"

std::vector<bool> hammingCorrection(
    std::vector<bool>& hammingCntBitsHammingBit) {
  std::vector<bool> originalCntDataBitsBit;

  // For each parity bit, calculate the correct parity and compare
  short ierr = 0;  // Contains the i-th error position
  for (int i = 1; i <= kCntBitsHamming; i *= 2) {
    bool p = false;
    int pjump = 0;
    // run on data bits for each parity bit
    for (short id = i - 1; id < kCntBitsHamming; id++) {
      if (hammingCntBitsHammingBit[id] == 1) {
        if (not isParityIdx(id))  // Skipping parity bits
          p = !p;
      }
      if (++pjump == i) {
        id += pjump;  // Moving to the next group of bits equal to i
        pjump = 0;
      }
    }
    // Calculate the i-th error position
    if (hammingCntBitsHammingBit[i - 1] !=
        p) {  // If the parity bit does not match the predicted value
      if (i - 1 == 0) ierr++;
      if (i - 1 == 1) ierr += 2;
      if (i - 1 == 3) ierr += 4;
      if (i - 1 == 7) ierr += 8;
      if (i - 1 == 15) ierr += 16;
      if (i - 1 == 31) ierr += 32;
      if (i - 1 == 63) ierr += 64;
    }
  }

  if (ierr > 0) {  // If there is a bug, fix
    hammingCntBitsHammingBit[ierr - 1] = !hammingCntBitsHammingBit[ierr - 1];
    std::cout << "Correct error. Done" << std::endl;
  }

  for (short i = 0; i < kCntBitsHamming; i++) {
    if (not isParityIdx(i))  // Parity bits skipped
      originalCntDataBitsBit.push_back(hammingCntBitsHammingBit[i]);
  }

  return originalCntDataBitsBit;
}

std::vector<bool> removeHammingParity(std::vector<bool>& boolBitsArray) {
  int bitCount = 0;
  std::vector<bool> bitCntBitsHammingArray;
  std::vector<bool> bitCntDataBitsArray;
  std::vector<bool> originalBits;

  // Read blocks on kCntBitsHamming
  int boolBitsArrayCount =
      ((((int)boolBitsArray.size() / kCntBitsHamming) * kCntBitsHamming) - 1);
  // read bits from end vector
  for (; boolBitsArrayCount > -1; boolBitsArrayCount--) {
    if (bitCount < kCntBitsHamming) {
      bitCntBitsHammingArray.push_back(boolBitsArray[boolBitsArrayCount]);
      bitCount++;
    }
    if (bitCount == kCntBitsHamming) {
      boolBitsArray.resize(boolBitsArrayCount + 1);
      std::vector<bool>(boolBitsArray).swap(boolBitsArray);

      // Because read bits from the end, then restore the order
      std::reverse(bitCntBitsHammingArray.begin(),
                   bitCntBitsHammingArray.end());

      bitCntDataBitsArray = hammingCorrection(bitCntBitsHammingArray);
      bitCntBitsHammingArray.clear();

      std::reverse(bitCntDataBitsArray.begin(), bitCntDataBitsArray.end());
      originalBits.insert(std::end(originalBits),
                          std::begin(bitCntDataBitsArray),
                          std::end(bitCntDataBitsArray));
      bitCntDataBitsArray.clear();
      bitCount = 0;
    }
  }

  std::reverse(originalBits.begin(), originalBits.end());
  return originalBits;
}

void recoverHamming(std::string flNameFrom, std::string flNameTo) {
  std::cout << "Start recoverHamming..." << std::endl;
  std::vector<unsigned char> vectReadFileBytes = readFileBytes(flNameFrom);
  std::vector<bool> vectBytesToBits = bytesToBits(vectReadFileBytes);
  std::vector<bool> vectRemoveHammingParity =
      removeHammingParity(vectBytesToBits);
  std::vector<unsigned char> vectBitsToBytes =
      bitsToBytes(vectRemoveHammingParity);
  writeFileBytes(flNameTo, vectBitsToBytes);

  std::cout << "  Done" << std::endl;
  //  writeFileBytes(flNameTo,bitsToBytes(removeHammingParity(bytesToBits(readFileBytes(flNameFrom)))));
}