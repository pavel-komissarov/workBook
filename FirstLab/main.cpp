//made by Paul
//Lab work 1


#include <iostream>
#include <vector>
#include <istream>
#include <string>
#include <fstream>


std::vector<std::string> valid_options = {"l", "lines", "c", "bytes", "w", "words", "m", "chars"};


int fileSize(std::basic_ifstream<char> &file) {
    int byteS;
    file.clear();
    byteS = file.tellg();
    file.close();
    return byteS;
}


int linesCounter(const char &nowChar, const int &lineS) {
    int lines = lineS;
    if (nowChar == '\n') {
        lines++;
    }
    return lines;
}


int charsCounter(const char &nowChar, const int &charS) {
    int chars = charS;
    if (isalpha(nowChar)) {
        chars++;
    }
    return chars;
}


int wordsCounter(const char &pastChar, const char &nowChar, const int &wordS) {
    int words = wordS;
    if (!isspace(nowChar) && isspace(pastChar)) {
        words++;
    }
    return words;
}

//duplicate check
bool check_exists(const std::vector<std::string> &v, const std::string &s) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == s) {
            return true;
        }
    }
    return false;
}

//validation of the entered option
bool check_valid(const std::string &s) {
    for (int i = 0; i < valid_options.size(); i++) {
        if (valid_options[i] == s) return true;
    }
    return false;
}

//function to work with text file
void WordCount(const int &isline, const int &isbyte,
               const int &isword, const int &ischar,
               const std::basic_string<char> &path) {

    int chars = 0;
    int words = 0;
    int lines = 1;
    int bytes;
    char pastChar = ' ';
    char nowChar;

    std::ifstream filew(path);

    while (filew.get(nowChar)) {
        words = wordsCounter(pastChar, nowChar, words);
        chars = charsCounter(nowChar, chars);
        lines = linesCounter(nowChar, lines);
        pastChar = nowChar;
    }
    bytes = fileSize(filew);
    filew.close();


    if (isline != 0) {
        std::cout << lines << ' ';
    }
    if (isbyte != 0) {
        std::cout << words << ' ';
    }
    if (isword != 0) {
        std::cout << bytes << ' ';
    }
    if (ischar != 0) {
        std::cout << chars << ' ';
    }
    if (isline == 0 && isbyte == 0 && isword == 0 && ischar == 0) {
        std::cout << lines << ' ' << words << ' '
                  << bytes << ' ';
    }
    std::cout << path << std::endl;
}

int main(int argv, char *argc[]) {
    if (argv == 1) { //if only the program name is in the arguments
        std::cout << "No arguments!\n";
    } else {
        int isline = 0;
        int ischar = 0;
        int isword = 0;
        int isbyte = 0;
        std::vector<std::basic_string<char>> paths;
        std::vector<std::string> options;
        std::string lineArg, cutout1, cutout2, slice;
        int len;
        //parsing the obtained data
        //Start cycle with 1, because in 0 there is only the name of the program
        for (int i = 1; i < argv; i++) {
            lineArg = argc[i];
            cutout1 = lineArg.substr(0, 1); //for analysis "-"
            cutout2 = lineArg.substr(0, 2); //for analysis "--"
            len = lineArg.size();
            if (cutout2 == "--") {
                //there is only one option
                //cut the first two characters "-"
                slice = lineArg.substr(2, len);
                if (check_valid(slice) && slice.length() > 1) {
                    if (!check_exists(options, slice)) options.push_back(slice);
                } else {
                    std::cout << "No valid option = \"" << lineArg << "\"\n";
                    return 1;
                }
            } else if (cutout1 == "-") {
                //can be several options
                //We start the cycle with 1, because 0 is the symbol "-"
                for (int i = 1; i < len; i++) {
                    //Looking at one symbol
                    slice = lineArg.substr(i, 1);
                    if (check_valid(slice)) {
                        if (!check_exists(options, slice)) options.push_back(slice);
                    } else {
                        std::cout << "No valid option = \"" << lineArg << "\"\n";
                        return 1;
                    }
                }
            } else //if there is no "-" or "-", we assume that in the parameter the file
            {
                paths.push_back(lineArg);
            }
        }
        for (int i = 0; i < options.size(); i++) {
            if (options[i] == "l" || options[i] == "lines") isline = 1;
            if (options[i] == "c" || options[i] == "bytes") isbyte = 1;
            if (options[i] == "w" || options[i] == "words") isword = 1;
            if (options[i] == "m" || options[i] == "chars") ischar = 1;
        }
        for (int i = 0; i < std::size(paths); i++) {
            WordCount(isline, isbyte, isword, ischar, paths[i]);    //call a function
        }
        return 0;
    }
    return 0;
}
