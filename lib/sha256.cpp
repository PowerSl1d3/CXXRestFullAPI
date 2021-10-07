//
// Created by olegaksenenko on 10/6/21.
//

#include "sha256.h"

std::string sha256::toHex(const uint8_t s) {
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(s);
    return ss.str();
}

std::string sha256::compute(const std::string& line) {
    uint8_t hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, line.c_str(), line.size());
    SHA256_Final(hash, &sha256);

    std::string ret;
    for (size_t i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        ret += toHex(hash[i]);
    }
    return ret;
}
