#pragma once

#include <string>
#include <vector>

const int kSizeOfFileName = 50;

struct info {
  char fileName[kSizeOfFileName];
  size_t sizeOfFile;
};

void creatAnArc(std::string& arcName, std::vector<std::string>& nameFiles);
void readAnInfo(std::string& arcName, std::vector<info>& vectorInfo,
                int& countOfFiles);
void extractArc(std::string& arcName, std::vector<std::string>& files);
void addFile(std::string& arcNAme, std::string& nameFile);
void deleteFile(std::string& arcName, std::string& nameFile);
void mergeArcs(std::string& arcName, std::vector<std::string>& nameFile);
std::string getTmpName(const std::string& arcName);
void rename(const std::string& arcName);