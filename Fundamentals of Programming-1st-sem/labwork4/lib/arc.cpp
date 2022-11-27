#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "hamEncode.h"
#include "arc.h"

std::string getTmpName(const std::string& arcName) {
  std::filesystem::path archName{arcName};
  std::filesystem::path dir{arcName};
  dir.remove_filename();
  std::string tmpNme = dir.string() + "tmp.haf";
  return tmpNme;
}

void rename(const std::string& arcName) {
  std::filesystem::path tmpName{getTmpName(arcName)};
  std::filesystem::rename(arcName, tmpName);
}

void creatAnArc(std::string& arcName, std::vector<std::string>& nameFiles) {
  std::filesystem::path arc{arcName};
  std::filesystem::path dirOfFiles;
  std::vector<info> vectorInfo;
  int countOfFiles = nameFiles.size();

  for (int i = 0; i < countOfFiles; i++) {
    std::ifstream file(nameFiles[i], std::ios::binary);

    if (!file) throw "ERROR: Can't open a file!";

    info inf;
    dirOfFiles = nameFiles[i];
    std::string str = dirOfFiles.filename().string();

    for (int i = 0; i < std::size(str); i++) {
      inf.fileName[i] = str[i];
    }

    for (int i = std::size(str); i < std::size(inf.fileName); i++) {
      inf.fileName[i] = '\0';
    }

    inf.sizeOfFile = std::filesystem::file_size(dirOfFiles);

    vectorInfo.push_back(inf);
    file.close();
  }

  arc.remove_filename();

  if (!arc.empty())
    std::filesystem::create_directories(std::filesystem::absolute(arc));

  std::ofstream arcFile(arcName, std::ios::binary);

  arcFile.write(const_cast<char*>("HAF"), 3);
  arcFile.write(reinterpret_cast<char*>(&countOfFiles), sizeof(countOfFiles));

  for (int i = 0; i < vectorInfo.size(); i++)
    arcFile.write(reinterpret_cast<char*>(&vectorInfo[i]), sizeof(info));

  for (int i = 0; i < countOfFiles; i++) {
    std::ifstream file(nameFiles[i], std::ios::binary);
    const char byte[1] = {};
    while (file.read(const_cast<char*>(*(&byte)), 1)) {
      arcFile.write(const_cast<char*>(*(&byte)), 1);
    }
    std::cout << "File " << nameFiles[i] << " added to the archive " << '\n';
    file.close();
  }

  arcFile.close();
}

void readAnInfo(std::string& arcName, std::vector<info>& vectorInfo,
                int& countOfFiles) {
  std::ifstream f(arcName, std::ios::binary);

  char header[3];
  f.read(reinterpret_cast<char*>(&header), 3);

  //  int cntOfFile;
  f.read(reinterpret_cast<char*>(&countOfFiles), sizeof(countOfFiles));

  info inf;
  for (int i = 1; i <= countOfFiles; i++) {
    f.read(reinterpret_cast<char*>(&inf), sizeof(info));
    vectorInfo.push_back(inf);
  }
}

void extractArc(std::string& arcName, std::vector<std::string>& files) {
  std::vector<info> vectorInfo;

  int countOfFiles;
  readAnInfo(arcName, vectorInfo, countOfFiles);

  std::ifstream arcFile(arcName, std::ios::binary);

  int dataBlock = 3 + sizeof(int) + vectorInfo.size() * sizeof(info);
  arcFile.seekg(dataBlock);
  for (int i = 0; i < vectorInfo.size(); i++) {
    dataBlock += vectorInfo[i].sizeOfFile;

    bool fileInList = false;

    for (int j = 0; j < files.size(); j++) {
      if (vectorInfo[i].fileName == files[j]) {
        fileInList = true;
        break;
      }
    }

    if (files.size() != 0 && !fileInList) arcFile.seekg(dataBlock);

    if (files.size() == 0 || fileInList) {
      const char byte[1] = {};
      std::ofstream nowFile(vectorInfo[i].fileName, std::ios::binary);

      while (arcFile.tellg() < dataBlock) {
        arcFile.read(const_cast<char*>(*(&byte)), 1);
        nowFile.write(const_cast<char*>(*(&byte)), 1);
      }
      nowFile.close();
      std::cout << "File " << vectorInfo[i].fileName << " extracted"
                << std::endl;
    }
  }

  arcFile.close();
}

void addFile(std::string& arcName, std::string& nameFile) {
  std::filesystem::path archName{arcName};
  std::filesystem::path dir{arcName};
  std::filesystem::path nameOfFile{nameFile};
  dir.remove_filename();
  std::string tmpNme = dir.string() + "tmp_forAddFile.tmp";
  std::filesystem::path tmpName{tmpNme};
  std::filesystem::rename(archName, tmpName);
  std::vector<info> vectInfo;

  int countOfFiles;
  readAnInfo(tmpNme, vectInfo, countOfFiles);

  std::vector<info> addVectInfo;

  for (int i = 0; i < vectInfo.size(); i++) {
    addVectInfo.push_back(vectInfo[i]);
  }

  std::ifstream file(nameOfFile, std::ios::binary);

  if (!file) throw "ERROR: Can't open a file!";

  info inf;
  std::filesystem::path dirOfFiles{nameFile};
  std::string str = dirOfFiles.filename().string();

  for (int i = 0; i < std::size(str); i++) {
    inf.fileName[i] = str[i];
  }

  for (int i = std::size(str); i < std::size(inf.fileName); i++) {
    inf.fileName[i] = '\0';
  }

  inf.sizeOfFile = std::filesystem::file_size(dirOfFiles);

  addVectInfo.push_back(inf);
  file.close();

  std::ofstream arcFile(arcName, std::ios::binary);
  countOfFiles++;
  arcFile.write(const_cast<char*>("HAF"), 3);
  arcFile.write(reinterpret_cast<char*>(&countOfFiles), sizeof(countOfFiles));
  for (int i = 0; i < addVectInfo.size(); i++) {
    arcFile.write(reinterpret_cast<char*>(&addVectInfo[i]), sizeof(info));
  }
  std::ifstream tmpFile(tmpName, std::ios::binary);

  int dataBlock = 3 + sizeof(int) + vectInfo.size() * sizeof(info);
  tmpFile.seekg(dataBlock);

  const char byte[1] = {};
  while (tmpFile.read(const_cast<char*>(*(&byte)), 1)) {
    arcFile.write(const_cast<char*>(*(&byte)), 1);
  }
  tmpFile.close();

  std::ifstream fileFile(nameOfFile, std::ios::binary);
  while (fileFile.read(const_cast<char*>(*(&byte)), 1)) {
    arcFile.write(const_cast<char*>(*(&byte)), 1);
  }

  std::cout << "File " << nameFile << " added to the archive \n";
  fileFile.close();
  arcFile.close();
  std::filesystem::remove(tmpName);
}

void deleteFile(std::string& arcName, std::string& nameFile) {
  std::filesystem::path archName{arcName};
  std::filesystem::path dir{arcName};
  std::filesystem::path nameOfFile{nameFile};
  dir.remove_filename();
  std::string tmpNme = dir.string() + "tmp_forDelete.tmp";
  std::filesystem::path tmpName{tmpNme};

  std::vector<info> vectInfo;

  int countOfFiles;
  readAnInfo(arcName, vectInfo, countOfFiles);

  std::vector<info> addVectInfo;

  for (int i = 0; i < vectInfo.size(); i++) {
    if (vectInfo[i].fileName != nameFile) addVectInfo.push_back(vectInfo[i]);
  }

  if (vectInfo.size() == addVectInfo.size()) throw 1;

  std::filesystem::rename(archName, tmpName);

  std::ofstream arcFile(arcName, std::ios::binary);
  countOfFiles--;
  arcFile.write(const_cast<char*>("HAF"), 3);
  arcFile.write(reinterpret_cast<char*>(&countOfFiles), sizeof(countOfFiles));

  for (int i = 0; i < addVectInfo.size(); i++) {
    arcFile.write(reinterpret_cast<char*>(&addVectInfo[i]), sizeof(info));
  }

  std::ifstream tmpFile(tmpName, std::ios::binary);

  int dataBlock = 3 + sizeof(int) + vectInfo.size() * sizeof(info);
  tmpFile.seekg(dataBlock);

  for (int i = 0; i < vectInfo.size(); i++) {
    dataBlock += vectInfo[i].sizeOfFile;
    if (vectInfo[i].fileName != nameFile) {
      while (tmpFile.tellg() < dataBlock) {
        const char byte[1] = {};
        tmpFile.read(const_cast<char*>(*(&byte)), 1);
        arcFile.write(const_cast<char*>(*(&byte)), 1);
      }
    } else
      tmpFile.seekg(dataBlock);
  }

  std::cout << "File " << nameFile << " deleted from the archive \n";
  tmpFile.close();
  arcFile.close();
  std::filesystem::remove(tmpName);
}

void mergeArcs(std::string& arcName, std::vector<std::string>& nameFile) {
  for (int i = 0; i < nameFile.size(); i++) {
    std::ifstream arcFile(nameFile[i], std::ios::binary);

    char header[3];
    arcFile.read(reinterpret_cast<char*>(&header), 3);
    if (header[0] != 'H' || header[1] != 'A' || header[2] != 'F') throw 1;
  }

  std::filesystem::path tmpDir{"tmp"};
  // if the tmp directory exists, remove
  if (std::filesystem::exists(tmpDir)) std::filesystem::remove_all(tmpDir);
  // create tmp directory
  create_directories(absolute(tmpDir));
  // make tmp directory current
  std::filesystem::current_path(absolute(tmpDir));

  for (int i = 0; i < nameFile.size(); i++) {
    std::vector<std::string> vect;

    extractArc(nameFile[i], vect);
  }

  std::vector<std::string> filesInDir;
  for (auto const& dir_entry :
       std::filesystem::directory_iterator{std::filesystem::current_path()}) {
    filesInDir.push_back(dir_entry.path().filename().string());
  }

  creatAnArc(arcName, filesInDir);
  std::filesystem::current_path("..");
  std::filesystem::remove_all(tmpDir);
}