#include <iostream>
#include <vector>
#include <istream>
#include <string>
#include <fstream>

//function for working with a text file
void WordCount(int l, int b, int w, int c, std::basic_string<char> path) {
    std::ifstream file(path);
    int chars = 0, words = 1;
    int lines = 1, bytes = 0;
    while (file.good()) {
        int c = file.get();
        bytes++;
        if (c != ' ' && c != '.' && c != '\n' && c != ',' && c != '?' && c != '!'
        && c != '*' && c != '"' && c != ':'
        && c != ';') {
            chars++;
        }
        if (c == '\n') {
            lines++;
        }
        if (c == ' ' || c == '\n') {
            words++;
        }
    }
    if (l !=0) {
        std::cout << lines << ' ';
    }
    if (w != 0) {
        std::cout << words << ' ';
    }
    if (c != 0) {
        std::cout << bytes << ' ';
    }
    if (b != 0) {
        std::cout << chars << ' ';
    } else {
        std::cout << lines << ' ' << words << ' '
        << bytes << ' ';
    }
    std::cout << path << std::endl;
}
int main(int argv, char *argc[]) {
    int l = 0, m = 0, w = 0, c = 0;
    std::vector<char> opt;
    std::basic_string<char> path;
    std::vector<std::basic_string<char>> paths;
    size_t sizeCh;
    for (int i = 1; i < argv; i++) {    //parsing options and paths
        sizeCh = strlen(argc[i]);   //len string
        if (argc[i][0] != '-') {
            for (int j = 1; j < sizeCh; j++) {
                if (argc[i][j] == '.') {
                }
            }
            paths.push_back(argc[i]);
        } else if (sizeCh && argc[i][1] == '-') {   //search options with '-'
            if (argc[i][2] == 'l' && l == 0) {
                l++;
            } else if (argc[i][2] == 'b' && c == 0) {
                c++;
            } else if (argc[i][2] == 'w' && w == 0) {
                w++;
            } else if (argc[i][2] == 'c' && m == 0) {
                m++;

            }

        } else if (argc[i][0] == '-' && argc[i][1] != '-') {    //search options with '--'
            for (int j = 1; j < sizeCh; j++) {
                if (argc[i][j] == 'l' && l == 0) {
                    l++;
                }
                if (argc[i][j] == 'c' && c == 0) {
                    c++;
                }
                if (argc[i][j] == 'w') {
                    w++;
                }
                if (argc[i][j] == 'm') {
                    m++;
                }
            }
        }
    }
    for (int i = 0; i < std::size(paths); i++) {
        WordCount(l, c, w, m, paths[i]);    //function call
    }
    return 0;
}
