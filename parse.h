#ifndef __CHESS_CONFIG_PARSER_H
#define __CHESS_CONFIG_PARSER_H

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <utility>
#include <vector>

namespace {

    std::vector<std::string> tokenise_string(std::string s) {
        const std::regex re("\\s+");
        std::sregex_token_iterator it{s.begin(), s.end(), re, -1};
        std::vector<std::string> tokens{it, {}};
        tokens.erase(
            std::remove_if(tokens.begin(),
                           tokens.end(),
                           [](std::string const& str) {
                               return str.size() == 0;
                           }),
            tokens.end());

        return tokens;
    }

    std::pair<std::string, std::string> parse_line(std::string s) {
        std::vector<std::string> tokens = tokenise_string(s);
        if (tokens.size() != 3 || tokens[1] != "=") {
            return std::pair<std::string, std::string>("", "");
        }
        else {
            return std::pair<std::string, std::string>(tokens[0], tokens[2]);
        }
    }

}

using Config = std::map<std::string, std::string>;

void parse_file(std::ifstream& file, Config* dest) {
    if (!file.is_open()) return;
    dest->clear();
    std::pair<std::string, std::string> p;
    for (std::string line; std::getline(file, line); ) {
        if (line.empty() || line[0] == '#') continue;
        p = parse_line(line);
        if (p.first == "") continue;
        (*dest)[p.first] = p.second;
    }
    if (file.is_open()) file.close();
}

void parse_file(std::string filename, Config* dest) {
    std::ifstream file;
    file.open(filename);
    parse_file(file, dest);
    if (file.is_open()) file.close();
}


#endif