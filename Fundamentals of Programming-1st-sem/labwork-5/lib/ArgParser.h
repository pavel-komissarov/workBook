#pragma once
#include <optional>
#include <string>
#include <vector>

namespace ArgumentParser {

class ArgParser {
 public:
  struct param {
    std::string nameParam;
    char shortParam = '\0';
    std::string description;
    bool isMultiVal = false;
    int MinArgsCount = 0;
    std::string valStr;
    std::optional<int> valInt;
    std::vector<int> valsInt;
    bool isDefault = false;
    bool isInt = false;
    bool storeValFlag = false;
    std::string* strVal;
    std::vector<int>* intVals;
    int* intVal;
  };

  struct Flag {
    char shortName = '\0';
    std::string name;
    std::string description;
    bool isDefault =
        false;  // sign that the flag has a default value
    bool val;
    bool* boolVal;
    bool storeValFlag = false;
  };

  std::vector<Flag> vectFlag;

  std::vector<param> vectParam;

  std::vector<param> vectHelp;

  std::string name;

  bool positional = false;

  bool isHelp = false;

  ArgParser(std::string nme) { name = nme; };

  ArgParser& AddIntArgument(std::string fullName);

  ArgParser& AddIntArgument(char shortName, std::string fullName);

  ArgParser& AddIntArgument(std::string fullName, std::string description);

  ArgParser& AddFlag(std::string fullName, std::string description);

  ArgParser& AddFlag(char shortName, std::string fullName);

  ArgParser& AddFlag(char shortName, std::string fullName, std::string description);

  void AddHelp(char shortName, std::string fullName, std::string description);

  bool Parse(int argc, char** argv);

  bool Parse(std::vector<std::string> vectorOfStr);

  void StoreValues(std::vector<int>& values);

  void StoreValue(std::vector<int>& values);

  void StoreValue(std::string& value);

  void StoreValue(bool& value);

  std::string HelpDescription();

  bool Help() const;

  ArgParser& MultiValue(int value = 0);

  ArgParser& Positional();

  ArgParser& AddStringArgument(char shortName, std::string fullName);

  ArgParser& AddStringArgument(char shortName, std::string fullName, std::string description);

  ArgParser& AddStringArgument(std::string fullName);

  std::string GetStringValue(const std::string& fullName);

  int GetIntValue(const std::string& fullName);

  int GetIntValue(const std::string& fullName, int value);

  bool GetFlag(const std::string& fullName);

  void Default(char* value);

  void Default(bool value);

};

}
