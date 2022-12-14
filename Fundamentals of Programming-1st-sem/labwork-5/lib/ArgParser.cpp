#include "ArgParser.h"

//#include <utility>

using namespace ArgumentParser;

std::string BoolToString(bool b) { return b ? "true" : "false"; }

bool IsdigitStr(const std::string& str) {
  bool ret = true;
  for (char i : str) {
    if (!isdigit(i)) return false;
  }
  return ret;
}

ArgParser& ArgParser::AddIntArgument(std::string fullName) {
  param structOfParam;
  structOfParam.isInt = true;
  structOfParam.nameParam = std::move(fullName);
  vectParam.push_back(structOfParam);

  return *this;
}

ArgParser& ArgParser::AddIntArgument(char shortName, std::string fullName) {
  param structOfParam;
  structOfParam.isInt = true;
  structOfParam.nameParam = std::move(fullName);
  structOfParam.shortParam = shortName;
  vectParam.push_back(structOfParam);

  return *this;
}

ArgParser& ArgParser::AddIntArgument(std::string fullName, std::string description) {
  param structOfParam;
  structOfParam.isInt = true;
  structOfParam.nameParam = std::move(fullName);
  structOfParam.description = std::move(description);
  vectParam.push_back(structOfParam);

  return *this;
}

ArgParser& ArgParser::AddFlag(std::string fullName, std::string description) {
  Flag structFlag;

  structFlag.name = std::move(fullName);
  structFlag.description = std::move(description);
  structFlag.val = true;

  vectFlag.push_back(structFlag);

  return *this;
}

ArgParser& ArgParser::AddFlag(char shortName, std::string fullName) {
  Flag structFlag;

  structFlag.shortName = shortName;
  structFlag.name = std::move(fullName);
  structFlag.val = true;

  vectFlag.push_back(structFlag);

  return *this;
}

ArgParser& ArgParser::AddFlag(char shortName, std::string fullName, std::string description) {
  Flag structFlag;

  structFlag.shortName = shortName;
  structFlag.name = std::move(fullName);
  structFlag.description = std::move(description);
  structFlag.val = true;

  vectFlag.push_back(structFlag);

  return *this;
}

void ArgParser::AddHelp(char shortName, std::string fullName, std::string description) {
  param structHelp;
  structHelp.shortParam = shortName;
  structHelp.nameParam = std::move(fullName);
  structHelp.description = std::move(description);

  vectHelp.push_back(structHelp);
}

bool ArgParser::Parse(int argc, char** argv) {
  std::vector<std::string> vect;
  if (argc == 1) {
    return true;
  } else {
    for (int i = 0; i < argc; i++) {
      vect.emplace_back(argv[i]);
    }
    return Parse(vect);
  }
}

bool ArgParser::Parse(std::vector<std::string> vectorOfStr) {
  std::string paramFirst;  // to analyze sum and mult
  for (int i = 1; i < vectorOfStr.size(); i++) {
    if (vectorOfStr[i] == "--help") {
      isHelp = true;
      return true;
    }

    char paramCh = '\0';
    std::string paramStr;
    param structOfParam;

    int indEq = vectorOfStr[i].find('=');

    if (vectorOfStr[i].substr(0, 1) == "-" && vectorOfStr[i].substr(1, 1) != "-") {
      paramCh = vectorOfStr[i].substr(1, 1)[0];
      structOfParam.shortParam = paramCh;
    }

    if (vectorOfStr[i].substr(0, 2) == "--") {
      paramStr = vectorOfStr[i].substr(2, indEq - 2);
      structOfParam.nameParam = paramStr;
    }
    // if there is an empty parameter without "=", then save it and go to
    // next parameter
    if ((paramCh != '\0' || !paramStr.empty()) && indEq == -1) {
      paramFirst = paramStr;
      continue;
    }

    std::string val;
    if (indEq == -1) {
      val = vectorOfStr[i];
    } else {
      val = vectorOfStr[i].substr(indEq + 1, vectorOfStr[i].size() - indEq);
    }

    for (auto& j : vectParam) {
      if (j.shortParam == paramCh ||
          j.nameParam == paramStr || positional) {
        if (IsdigitStr(val)) {
          int dig = std::stoi(val);
          if (j.isMultiVal) {
            j.valsInt.push_back(dig);
          } else {
            j.valInt = dig;
          }
        } else {
          j.valStr = val;
        }
      }
    }
  }

  if (!vectParam.empty()) {
    for (auto& j : vectParam) {
      if (!j.valInt.has_value() &&
          j.valsInt.empty() && j.valStr.empty()) {
        return false;
      }
    }
  }

  for (auto& i : vectParam) {
    if (i.isMultiVal) {
      if (i.valsInt.size() < i.MinArgsCount) {
        return false;
      }
    }
  }

  // save StoreValue values
  for (auto& i : vectFlag) {
    if (i.storeValFlag) {
      int fl = 0;
      if (i.name == paramFirst && paramFirst == "sum") {
        *i.boolVal = true;
        fl = 1;
        continue;
      } else if (i.name == paramFirst && paramFirst == "mult") {
        *i.boolVal = true;
        fl = 1;
        continue;
      }
//      if (vectFlag[i].name == paramFirst && paramFirst != "sum" &&
//          paramFirst != "mult")
      if (fl == 0 && paramFirst != "sum" && paramFirst != "mult") {
        *i.boolVal = i.val;
      }

    }
  }

  for (auto& i : vectParam) {
    if (i.storeValFlag) {
      if (i.isInt) {
        if (i.isMultiVal)
          *i.intVals = i.valsInt;
        else
          *i.intVal = i.valInt.value();
      } else
        *i.strVal = i.valStr;
    }
  }

  return true;
}

void ArgParser::StoreValues(std::vector<int>& values) {
  StoreValue(values);
}

void ArgParser::StoreValue(std::vector<int>& values) {
  vectParam[vectParam.size() - 1].storeValFlag = true;
  vectParam[vectParam.size() - 1].intVals = &values;
}

void ArgParser::StoreValue(std::string& value) {
  vectParam[vectParam.size() - 1].storeValFlag = true;
  vectParam[vectParam.size() - 1].strVal = &value;
}

void ArgParser::StoreValue(bool& value) {
  vectFlag[vectFlag.size() - 1].storeValFlag = true;
  vectFlag[vectFlag.size() - 1].boolVal = &value;
}

bool ArgParser::Help() const {
  if (isHelp)
    return true;
  else
    return false;
}

ArgParser& ArgParser::MultiValue(int value) {
  vectParam[vectParam.size() - 1].isMultiVal = true;
  vectParam[vectParam.size() - 1].MinArgsCount = value;
  return *this;
}

ArgParser& ArgParser::Positional() {
  positional = true;
  return *this;
}

ArgParser& ArgParser::AddStringArgument(char shortName, std::string fullName) {
  param structOfParam;
  structOfParam.shortParam = shortName;
  structOfParam.nameParam = std::move(fullName);
  vectParam.push_back(structOfParam);

  return *this;
}

ArgParser& ArgParser::AddStringArgument(char shortName, std::string fullName, std::string description) {
  param structOfParam;
  structOfParam.shortParam = shortName;
  structOfParam.nameParam = std::move(fullName);
  structOfParam.description = std::move(description);
  vectParam.push_back(structOfParam);

  return *this;
}

ArgParser& ArgParser::AddStringArgument(std::string fullName) {
  param structOfParam;
  structOfParam.nameParam = std::move(fullName);
  vectParam.push_back(structOfParam);

  return *this;
}

std::string ArgParser::GetStringValue(const std::string& fullName) {
  for (auto& i : vectParam) {
    if (i.nameParam == fullName) {
      return i.valStr;
    }
  }
}

int ArgParser::GetIntValue(const std::string& fullName) {
  for (auto& i : vectParam) {
    if (i.nameParam == fullName) {
      return i.valInt.value();
    }
  }
}

int ArgParser::GetIntValue(const std::string& fullName, int value) {
  for (auto& i : vectParam) {
    if (i.nameParam == fullName) {
      return i.valsInt[value];
    }
  }
}

bool ArgParser::GetFlag(const std::string& fullName) {
  for (auto& i : vectFlag) {
    if (i.name == fullName) {
      return i.val;
    }
  }
  return false;
}

void ArgParser::Default(char* value) {
  vectParam[vectParam.size() - 1].isDefault = true;
  vectParam[vectParam.size() - 1].valStr = value;
}

void ArgParser::Default(bool value) {
  vectFlag[vectFlag.size() - 1].isDefault = true;
  vectFlag[vectFlag.size() - 1].val = value;
}

std::string ArgParser::HelpDescription() {
  std::string str;
  str = this->name + '\n';
  for (auto& i : vectHelp)
    str = str + i.description + '\n';
  str = str + '\n';
  for (auto& i : vectParam) {
    if (!i.isInt) {
      str = str + "-" + i.shortParam + ",  --" + i.nameParam +
          "=<string>,  " + i.description;

      if (i.isMultiVal) {
        str = str + " [repeated, min args = " +
            std::to_string(i.MinArgsCount) + "]";
      }
      str = str + '\n';
    }
  }

  for (auto& i : vectFlag) {
    if (i.shortName != '\0') {
      str = str + "-" + i.shortName + ",  ";
    }
    str = str + "--" + i.name;
    if (!i.description.empty()) {
      str = str + ",  " + i.description;
    }
    if (i.isDefault) {
      str = str + " [default = " + BoolToString(i.val) + "]";
    }
    str = str + '\n';
  }

  for (auto& i : vectParam) {
    if (i.isInt) {
      str = str + "    --" + i.nameParam + "=<int>,  " +
          i.description;

      if (i.isMultiVal) {
        str = str + " [repeated, min args = " +
            std::to_string(i.MinArgsCount) + "]";
      }
      str = str + '\n';
    }
  }
  str = str + '\n';
  str = str + "-h, --help Display this help and exit\n";
  return str;
}
