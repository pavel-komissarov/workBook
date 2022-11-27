#include <filesystem>
#include <iostream>
#include <string>

#include "../lib/arc.h"
#include "../lib/hamDecode.h"
#include "../lib/hamEncode.h"

// -c, –create - создание нового архива
// -f, –file = [ARHCNAME] - имя файла с архивом
// -l, –list - вывести список файлов в архиве
// -x, –extract - извлечь файлы из архива(если не указано, то все файлы)
// -a, –append - добавить файл в архив
// -d, –-delete - удалить файл из архива
// -A, –-concatenate - смерджить два архива

 //-l --file=C:\Users\pavel\Desktop\arctest\hamarc.haf
 //-c --file=C:\Users\pavel\Desktop\arctest\hamarc.haf C:\Users\pavel\Desktop\arctest\test1.txt C:\Users\pavel\Desktop\arctest\test2.txt C:\Users\pavel\Desktop\arctest\test3.txt
 //--extract --file=C:\Users\pavel\Desktop\arctest\hamarc.haf
 //--append --file=C:\Users\pavel\Desktop\arctest\hamarc.haf C:\Users\pavel\Desktop\arctest\test4.txt
 //--delete --file=C:\Users\pavel\Desktop\arctest\hamarc.haf test4.txt
 //-c --file=C:\Users\pavel\Desktop\arctest\hamarc1.haf C:\Users\pavel\Desktop\arctest\test5.txt C:\Users\pavel\Desktop\arctest\test4.txt
 //--concatenate --file=C:\Users\pavel\Desktop\arctest\hamarc2.haf C:\Users\pavel\Desktop\arctest\hamarc1.haf C:\Users\pavel\Desktop\arctest\hamarc.haf

void Parser(const int& argc, char* argv[], std::vector<std::string>& commands,
            std::vector<std::string>& files, std::string& arcFile) {
  bool isSetArcFile = false;

  if (argc == 1) {
    throw 1;
  } else {
    for (int i = 1; i < argc; i++) {
      std::string param = argv[i];

      if (param == "-c" || param == "--create") commands.push_back("create");

      if (param == "-l" || param == "--list") commands.push_back("list");

      if (param == "-x" || param == "--extract") commands.push_back("extract");

      if (param == "-a" || param == "--append") commands.push_back("append");

      if (param == "-d" || param == "--delete") commands.push_back("delete");

      if (param == "-A" || param == "--concatenate")
        commands.push_back("concatenate");

      // Begin: remember the archive file name for -f or -file=
      if (param == "-f") {
        arcFile = argv[i + 1];
        isSetArcFile = true;
        i++;
      }
      if (param.substr(0, 7) == "--file=") {
        arcFile = param.substr(7, param.length());
        isSetArcFile = true;
      }
      // End: remember the file name of the archive

      // The first parameter should be the command
      if (commands.size() == 0 ||
          commands[0] != "create" && commands[0] != "list" &&
              commands[0] != "extract" && commands[0] != "append" &&
              commands[0] != "delete" && commands[0] != "concatenate") {
        throw 2;
      }

      // after -f or -file=, -A or -concatenate, -x or -extract goes
      // (can go) list of files
      if (isSetArcFile || commands[0] == "concatenate" ||
          commands[0] == "extract") {
        // Moving to footprint. option
        for (i++; i < argc; i++) {
          param = argv[i];
          if (param.substr(0, 1) == "-" || param.substr(0, 2) == "--") {
            i--;
            break;
          }
                    /*if (param.substr(0, 2) == "--") {
                      i--;
                      break;
                    }*/
          files.push_back(argv[i]);
        }
      }
    }
    if (commands.size() > 1) {
      throw 3;
    }
    if (!isSetArcFile) {
      throw 4;
    }
  }
}

int main(int argc, char** argv) {
  std::vector<std::string> commands;
  std::vector<std::string> files;
  std::string arcFile;

  setlocale(LC_ALL, "Russian");

  try {
    Parser(argc, argv, commands, files, arcFile);
  } catch (int err) {
    if (err == 1) std::cout << "Not parameters";
    if (err == 2)
      std::cout << "Invalid options: The first command should be: -c or "
                   "--create, -l or --list , -x or --extract, -a or --append, "
                   "-d or --delete, -A or --concatenate";
    if (err == 3) std::cout << "Invalid options: More than one command";
    if (err == 4)
      std::cout << "Invalid options: The archive file is not defined";

    return 1;
  } catch (std::string err) {
    std::cout << err << std::endl;
    return 1;
  } catch (...) {
    std::cout << "Parser error" << std::endl;
    return 1;
  }

  if (commands[0] == "create") {
    try {
      std::string tmpName = getTmpName(arcFile);
      std::filesystem::path tmpDir{tmpName};
      creatAnArc(tmpName, files);
      applyHamming(tmpName, arcFile.c_str());
      std::filesystem::remove(tmpDir);
    } catch (std::string err) {
      std::cout << err << std::endl;
      return 1;
    }
  }

  if (commands[0] == "list") {
    std::string tmpName = getTmpName(arcFile);
    std::filesystem::path tmpDir{tmpName};
    recoverHamming(arcFile, tmpName);
    std::vector<info> vectorOfInfo;
    int countOfFiles;
    readAnInfo(tmpName, vectorOfInfo, countOfFiles);

    std::filesystem::remove(tmpDir);
    for (int i = 0; i < vectorOfInfo.size(); i++) {
      std::cout << "File name=" << vectorOfInfo[i].fileName << '\t'
                << "File size=" << vectorOfInfo[i].sizeOfFile << std::endl;
    }
  }

  if (commands[0] == "extract") {
    try {
      std::string tmpName = getTmpName(arcFile);
      std::filesystem::path tmpDir{tmpName};
      recoverHamming(arcFile, tmpName);
      extractArc(tmpName, files);
      std::filesystem::remove(tmpDir);
    } catch (std::string err) {
      std::cout << err << std::endl;
      return 1;
    }
  }

  if (commands[0] == "append") {
    try {
      std::string tmpName = getTmpName(arcFile);
      std::filesystem::path tmpDir{tmpName};
      recoverHamming(arcFile, tmpName);
      addFile(tmpName, files[0]);
      applyHamming(tmpName, arcFile.c_str());
      std::filesystem::remove(tmpDir);
    } catch (std::string err) {
      std::cout << err << std::endl;
      return 1;
    }
  }

  if (commands[0] == "delete") {
    try {
      std::string tmpName = getTmpName(arcFile);
      std::filesystem::path tmpDir{tmpName};
      recoverHamming(arcFile, tmpName);
      deleteFile(tmpName, files[0]);
      applyHamming(tmpName, arcFile.c_str());
      std::filesystem::remove(tmpDir);
    } catch (int err) {
      if (err == 1) std::cout << "Error: couldn't find a file in archive";
      return 1;
    }
  }

  if (commands[0] == "concatenate") {
    try {
      std::vector<std::string> tmpFiles;
      for (int i = 0; i < files.size(); i++) {
        std::string tmpName = getTmpName(files[i]) + std::to_string(i);
        recoverHamming(files[i], tmpName);
        tmpFiles.push_back(tmpName);
      }
      std::string tmpArcFile =
          getTmpName(arcFile) + std::to_string(files.size() + 1);
      std::filesystem::path tmpDir{tmpArcFile};
      mergeArcs(tmpArcFile, tmpFiles);
      applyHamming(tmpArcFile, arcFile.c_str());
      std::filesystem::remove(tmpDir);
      for (auto i : tmpFiles) {
        std::filesystem::path tmpDir{i};
        std::filesystem::remove(tmpDir);
      }
    } catch (int err) {
      if (err == 1) std::cout << "Error: current file is not an archive";
      return 1;
    }
  }

  return 0;
}
