//
// Created by olegaksenenko on 10/6/21.
//

#ifndef CXXRESTFULLAPI_SHA256_H
#define CXXRESTFULLAPI_SHA256_H

#include <string>
#include <cstdint>
#include <sstream>
#include <iomanip>

#include <openssl/sha.h>

class sha256 {
private:
    static std::string toHex(const uint8_t s);
public:
    static std::string compute(const std::string& line);
};
#endif //CXXRESTFULLAPI_SHA256_H
