
#ifndef _TOTP_H
#define _TOPT_H

#include "openssl/hmac.h"
#include "openssl/evp.h"

namespace TOTP
{
    unsigned int GenerateToken(const char* b32key);
}

#endif
