#include <openssl/md5.h>

#ifndef CRYPTO
#define CRYPTO

#define HASH_LENGTH 2*MD5_DIGEST_LENGTH+1

void hash_from_string (char *string, char *hash);

#endif
