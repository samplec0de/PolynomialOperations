//
//  utils.cpp
//  [Backend] Операции с многочленами
//
//  Created by Андрей Москалёв on 12/04/2019.
//  Copyright © 2019 Андрей Москалёв. All rights reserved.
//

#include "utils.hpp"

std::string remove_spaces(std::string src) {
    std::string res = "";
    for (char c : src) {
        if (c == ' ') {
            continue;
        }
        res += c;
    }
    return res;
}

std::string rtrim(std::string src) {
    if (src.find('.') == std::string::npos) {
        return src;
    }
    while (src.back() == '0') {
        src.pop_back();
    }
    if (src.back() == '.') {
        src.pop_back();
    }
    return src;
}

std::string dtos(double n) {
    return rtrim(std::to_string(n));
}
