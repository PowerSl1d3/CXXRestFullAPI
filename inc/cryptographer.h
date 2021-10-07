//
// Created by olegaksenenko on 10/6/21.
//

#ifndef CXXRESTFULLAPI_CRYPTOGRAPHER_H
#define CXXRESTFULLAPI_CRYPTOGRAPHER_H

#include <string>
#include <cstdint>

#include <openssl/aes.h>

class cryptographer {
private:
    static constexpr uint8_t key[] = {
      0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
      0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
      0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
      0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
    };
public:
    static std::string encrypt(const std::string& line);
    static std::string decrypt(const std::string& line);
};
#endif //CXXRESTFULLAPI_CRYPTOGRAPHER_H
