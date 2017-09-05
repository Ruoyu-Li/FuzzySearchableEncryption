/* Dynamic Searchable Symmetric Encryption
 * Coding: Shuanghong He, CS school of HUST
 * E-mail: 740310627@qq.com
 */
#ifndef HEADER_CRYPTO_
#define HEADER_CRYPTO_

extern int crypto_init( void );
extern void crypto_free( void );
extern int crypto_random( unsigned char blob[], unsigned int length );
extern int crypto_hmac
(
	unsigned char key[],
	unsigned char blob[],
	unsigned int  blob_length,
	unsigned char result[],
	unsigned long result_length
);
extern int crypto_hmad
(
	unsigned char key[],
	unsigned char blob1[],
	unsigned int  blob1_length,
	unsigned char blob2[],
	unsigned int  blob2_length,
	unsigned char result[],
	unsigned long result_length
);
extern int crypto_hash
(
	unsigned char blob1[],
	unsigned char blob2[],
	unsigned char result[],
	unsigned long result_length
);
extern int crypto_aes_cryp
(
        unsigned char key[],
        unsigned long id,
        unsigned int  length,
        unsigned char plain[],
        unsigned char cname[]
);
extern int crypto_aes_deci
(
        unsigned char key[],
        unsigned char cname[],
        unsigned char out2[]
);
#endif
