#include "rainbow.h"
#include "crypto.h"

// 

/*
 * This function computes a hash given a plaintext string.
 * Here, the hash function used is MD5 provided by OpenSSL but it can be changed to whatever
 * can produce a string hash. If you change it, don't forget to change HASH_LENGTH (see crypto.h).
 * Copied from there: http://www.yoannsculo.fr/faire-un-hash-md5-dun-string-en-c/
 *
 * @param string	input plaintext to be hashed
 * @param hash		pointer to output the md5 hash (needs to be allocated)
 */
void hash_from_string(char *string, char *hash) {

	int i;
	char unsigned md5[MD5_DIGEST_LENGTH] = {0};
			
	MD5((const unsigned char *)string, strlen(string), md5);
					
	for (i=0; i < MD5_DIGEST_LENGTH; i++) {     	
        	sprintf(hash + 2*i, "%02x", md5[i]);
        }

}
