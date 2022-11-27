#pragma once

#include <string>
#include <vector>

const short kCntDataBits = 8;
const short kCntParityBits = 4;
const short kCntBitsHamming = kCntDataBits + kCntParityBits;

bool isParityIdx(short& i);
// short cntBitsHamming(short db);

std::vector<unsigned char> bitsToBytes(std::vector<bool>& bitsArray,
                                       int byteAdjust = 0);
std::vector<bool> bytesToBits(std::vector<unsigned char>& bytesArray);
std::vector<unsigned char> readFileBytes(const std::string& fileName);
void writeFileBytes(std::string& fileName,
                    std::vector<unsigned char>& fileBytes);