#include <iostream>
#include <vector>
#include <istream>
#include <string>
#include <fstream>

std::vector<std::string> valid_options = {"l", "lines", "c", "bytes", "w", "words", "m", "chars"};

//проверка на дублирование опций
bool check_exists(std::vector<std::string> v, std::string s) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == s) {
            return true;
        }
    }
    return false;
}

//проверка на валидность введенной опции
bool check_valid(std::string s) {
    for (int i = 0; i < valid_options.size(); i++) {
        if (valid_options[i] == s) return true;
    }
    return false;
}

//функция для работы с текстовым файлом
void WordCount(int l, int c, int w, int m, std::basic_string<char> path) {

    int chars = 0, words = 0;
    int lines = 1, bytes = 0;


    std::ifstream filew(path);
    std::string word;
    while (filew >> word) {
        words++;
    }
    filew.close();

    std::ifstream file(path);
    while (file.good()) {
        int c = file.get();
        if (c != -1) //если не eof
        {
            bytes++;
            //std::cout << "c = \"" << c << "\"\n";
            //Функция isalpha проверяет аргумент,
            //передаваемый через параметр сharacter, является ли он строчной
            //или прописной буквой алфавита
            if (isalpha(c)) {
                chars++;
            }
            if (c == '\n') {
                lines++;
            }
        }

    }
    file.close();

    if (l != 0) {
        std::cout << lines << ' ';
    }
    if (w != 0) {
        std::cout << words << ' ';
    }
    if (c != 0) {
        std::cout << bytes << ' ';
    }
    if (m != 0) {
        std::cout << chars << ' ';
    }
    if (l == 0 && w == 0 && c == 0 && m == 0) {
        std::cout << lines << ' ' << words << ' '
                  << bytes << ' ';
    }
    std::cout << path << std::endl;
}

int main(int argv, char *argc[]) {
    if (argv == 1) { // если в аргументах только имя программы
        std::cout << "No arguments!\n";
    } else {
        int l = 0, m = 0, w = 0, c = 0;
        std::vector<std::basic_string<char>> paths;
        std::vector<std::string> options;
        std::string stroka, virez1, virez2, srez;
        int len;
        //парсинг полученных данных
        //начинаем цикл с 1, т.к. в 0 лежит только имя программы
        for (int i = 1; i < argv; i++) {
            stroka = argc[i];
            virez1 = stroka.substr(0, 1); //для анализа "-"
            virez2 = stroka.substr(0, 2); //для анализа "--"
            len = stroka.size();
            if (virez2 == "--") {
                //здесь только одна опция
                //отрежем первые два символа "--"
                srez = stroka.substr(2, len);
                if (check_valid(srez) && srez.length() > 1) {
                    if (!check_exists(options, srez)) options.push_back(srez);
                } else {
                    std::cout << "No valid option = \"" << stroka << "\"\n";
                    return 1;
                }
            } else if (virez1 == "-") {
                //может быть несколько опций
                //начинаем цикл с 1, т.к. в 0 лежит символ "-"
                for (int i = 1; i < len; i++) {
                    //смотрим по одному символу
                    srez = stroka.substr(i, 1);
                    if (check_valid(srez)) {
                        if (!check_exists(options, srez)) options.push_back(srez);
                    } else {
                        std::cout << "No valid option = \"" << stroka << "\"\n";
                        return 1;
                    }
                }
            } else //если нет "-" или "--", то считаем, что в параметре файл
            {
                paths.push_back(stroka);
            };
        }
        for (int i = 0; i < options.size(); i++) {
            if (options[i] == "l" || options[i] == "lines") l = 1;
            if (options[i] == "c" || options[i] == "bytes") c = 1;
            if (options[i] == "w" || options[i] == "words") w = 1;
            if (options[i] == "m" || options[i] == "chars") m = 1;
        }

        for (int i = 0; i < std::size(paths); i++) {
            WordCount(l, c, w, m, paths[i]);    //вызов функции
        }

        return 0;
    }
    return 0;
}
