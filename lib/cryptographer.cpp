//
// Created by olegaksenenko on 10/6/21.
//

#include "cryptographer.h"

std::string cryptographer::encrypt(const std::string &line) {
    std::string ret = line;
    for (size_t i = 0; i < ret.size(); ++i) {
        ++ret[i];
    }
    return ret;
}

std::string cryptographer::decrypt(const std::string &line) {
    std::string ret = line;
    for (size_t i = 0; i < ret.size(); ++i) {
        --ret[i];
    }
    return ret;
}
