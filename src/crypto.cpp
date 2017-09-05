/* Dynamic Searchable Symmetric Encryption
 * Coding: Shuanghong He, CS school of HUST
 * E-mail: 740310627@qq.com
 */
#include <tomcrypt.h>
#include "global.h"
#include "dsse.h"
#include "crypto.h"

static prng_state prng;
static hash_state hash;

int crypto_init( void )
{
	/* DEBUG
	print_debug();
	printf( "crypto init\n" ); //*/

	/* init pseudo random gererator seeded by `/dev/random` */
	if( register_prng(&fortuna_desc) == -1 )
	{
		print_error();
		printf( "registering fortuna failed\n" );
		return 0;
	}
	if( rng_make_prng(128, find_prng("fortuna"), &prng, 0) != CRYPT_OK )
	{
		print_error();
		printf( "seeding fortuna failed\n" );
		unregister_prng( &fortuna_desc );
		return 0;
	}

	/* init sha256 */
	if( register_hash(&sha256_desc) == -1 )
	{
		print_error();
		printf( "registing sha256 failed\n" );
		prng_descriptor[find_prng("fortuna")].done( &prng );
		unregister_prng( &fortuna_desc );
		return 0;
	}
	/* init hmac */
	// hmac doesn't need to be initialized

	return 1;
}

void crypto_free( void )
{
	prng_descriptor[find_prng("fortuna")].done( &prng );
	unregister_prng( &fortuna_desc );
	unregister_hash( &sha256_desc );
}

/* generate random data */
int crypto_random( unsigned char blob[], unsigned int length )
{
    unsigned int rcount;
	rcount = prng_descriptor[find_prng("fortuna")].read( blob, length, &prng );
	if( rcount != length )
	{
		print_error();
		printf( "generate random data failed\n" );
		return 0;
	}

	return 1;
}

int crypto_hmac( unsigned char key[],
				 unsigned char blob[],
				 unsigned int  blob_length,
				 unsigned char result[],
				 unsigned long result_length )
{
	int status;
	unsigned char direct[MAXBLOCKSIZE];
	unsigned long direct_length = MAXBLOCKSIZE;

	status = hmac_memory( find_hash("sha256"), key, SKEY_LEN,
						  blob, blob_length,
						  direct, &direct_length );
	if( status != CRYPT_OK )
	{
		print_error();
		printf( "%s\n", error_to_string(status) );
		return 0;
	}

	/* copy last important bytes to result */
	for( int i = result_length - 1, j = direct_length - 1;
		 i >= 0 && j >= 0; i--, j-- )
		result[i] = direct[j];
	return 1;
}

int crypto_hmad( unsigned char key[],
				 unsigned char blob1[],
				 unsigned int  blob1_length,
				 unsigned char blob2[],
				 unsigned int  blob2_length,
				 unsigned char result[],
				 unsigned long result_length )
{
	int status;
	unsigned char input[blob1_length+blob2_length];
	unsigned char direct[MAXBLOCKSIZE];
	unsigned long direct_length = MAXBLOCKSIZE;

	for( int i = 0; i < blob1_length; i++ )
		input[i] = blob1[i];
	for( int i = 0, j = blob1_length; i < blob2_length; i++, j++ )
		input[j] = blob2[i];

	status = hmac_memory( find_hash("sha256"), key, SKEY_LEN,
						  input, blob1_length+blob2_length,
						  direct, &direct_length );
	if( status != CRYPT_OK )
	{
		print_error();
		printf( "%s\n", error_to_string(status) );
		return 0;
	}

	/* copy last important bytes to result */
	for( int i = result_length - 1, j = direct_length - 1;
		 i >= 0 && j >= 0; i--, j-- )
		result[i] = direct[j];
	return 1;
}

int crypto_hash( unsigned char blob1[],
				 unsigned char blob2[],
				 unsigned char result[],
				 unsigned long result_length )
{
	int hash_idx = find_hash( "sha256" );
	unsigned char direct[32];

	hash_descriptor[hash_idx].init( &hash );
	hash_descriptor[hash_idx].process( &hash, blob1, BASE_LEN); // H( P(k2, w) )
	hash_descriptor[hash_idx].process( &hash, blob2, BASE_LEN); // H( R )
	hash_descriptor[hash_idx].done( &hash, direct );

	/* copy last important bytes to input buffer */
	for( int i = result_length - 1, j = 32 - 1;
		 i >= 0 && j >= 0; i--, j-- )
		result[i] = direct[j];

	return 1;
}

int crypto_aes_cryp( unsigned char key[],
                               unsigned long id,
                               unsigned int  length,
                               unsigned char plain[],
                               unsigned char cname[])
{
    FILE *cipherfile;
    int index = 0;  
    int error = 0;    
    char flag = '0';
    int k = 0, l = 0, n = 0, m = 0;
    unsigned char in[2048] = { 0 };
    unsigned char out1[2048] = { 0 };
    unsigned char out2[2048] = { 0 };
    unsigned long outlen1 = 2048;
    unsigned long outlen2 = 2048;
    symmetric_ECB ecbaes;
    unsigned char ci[1024][17] = { 0 }, pl[17] = { 0 }; 
    int i, j;
    base64_encode( plain, length, out1, &outlen1);
    i = outlen1/16, j = outlen1%16; 

    /* init aes */
    if( (error = register_cipher(&aes_desc)) != CRYPT_OK )
    {
        print_error();
        printf( "register_cipher(&aes_desc) error:%s\n", error_to_string(error) );
	return 0;
    } 

    if( (index = find_cipher("aes")) == -1 )
    {
        print_error();
        printf( "find_cipher（\"aes\") error:%s\n", error_to_string(index) );
        return 0;
    }
    
    if( (cipherfile = fopen( (char*)cname, "w" )) == NULL)
    {
        print_error();
        printf( "open %s failed!\n", cname );
        return 0;
    }

    /* start CBC pattern */
    if( (error = ecb_start( index, key, 16, 0, &ecbaes)) != CRYPT_OK)
    {
         print_error();
	 printf( "ecb_start error:%s\n", error_to_string(error) );
         return 0;
    }
    
    for( ; k < i; k++)
    {
         for( n = 0; n<16; n++ )
         {
              pl[n] = out1[m++];
         }
      
         if( (error = ecb_encrypt( pl, ci[k], 16, &ecbaes)) != CRYPT_OK)
         {
              print_error();
 	      printf( "ecb_encrypt error:%s\n", error_to_string(error) );
              return 0;
         }
   
        
    }
    if( j != 0 )
    {
         for( n = 0; m < outlen1; n++ )
         {
              pl[n] = out1[m++];
         }
         for(  ; n < 16; n++)
         {
              pl[n] = '0';
              flag++;
         }
         if( (error = ecb_encrypt( pl, ci[k], 16, &ecbaes)) != CRYPT_OK )
         {
              print_error();
 	      printf( "ecb_encrypt error:%s\n", error_to_string(error) );
              return 0;
         }

    }
    memset( pl, '0', 15);
    pl[15] = flag+15;
    if( (error = ecb_encrypt( pl, ci[k+1], 16, &ecbaes)) != CRYPT_OK )
         {
              print_error();
 	      printf( "ecb_encrypt error:%s\n", error_to_string(error) );
              return 0;
         }

    for( m = 0 ; l <= k+1; )
    {
	for(i=0;i<16;i++)
        {
	      in[m++]=ci[l][i];
	}
        l++;
	printf("");
    }

    base64_encode( in, m, out2, &outlen2);
    fwrite( out2, 1, outlen2, cipherfile ); 
    fclose( cipherfile );   
    ecb_done( &ecbaes );

    /* logout aes */
    if( (error = unregister_cipher(&aes_desc)) != CRYPT_OK )
    {
        print_error();
        printf( "unregister_cipher(&aes_desc) error:%s\n", error_to_string(error) );
	return 0;
    } 
    return 1;
}
    
int crypto_aes_deci(unsigned char key[],
                             unsigned char cname[],
                             unsigned char out2[] )
{
    FILE *cipherfile;
    int index = 0;  
    int error = 0;
    int j =0; 
    int k = 0;
    int m = 0;
    int a = 0, b = 0;
    int flag = 0;
    unsigned char plain[2048] = { 0 };
    unsigned char cipher[2048] = { 0 };
    unsigned char out[2048] = { 0 };
    unsigned long outlen = 512;
    unsigned long outlen2 = 512;
    unsigned char ci[17] = { 0 },pl[1024][17]; 

    memset(pl, 0, 1024 * 17*sizeof(unsigned char));

    symmetric_ECB ecbaes = { 0 };
    memset( out2, 0, 2048 );  
    /* init aes */
    if( (error = register_cipher(&aes_desc)) != CRYPT_OK )
    {
        print_error();
        printf( "register_cipher(&aes_desc) error:%s\n", error_to_string(error) );
	return 0;
    } 

    if( (index = find_cipher("aes")) == -1 )
    {
        print_error();
        printf( "find_cipher（\"aes\") error:%s\n", error_to_string(index) );
        return 0;
    }
    
    if( (cipherfile = fopen( (char*)cname, "r" )) == NULL)
    {
        print_error();
        printf( "open %s failed!\n", cname );
        return 0;
    }

    /*if( (plainfile = fopen( (char*)pname, "w" )) == NULL)
    {
        print_error();
        printf( "open %s failed!\n", pname );
        return 0;
    }*/

    /* start CBC pattern */
    if( (error = ecb_start( index, key, 16, 0, &ecbaes)) != CRYPT_OK)
    {
         print_error();
         printf( "ecb_start error:%s\n", error_to_string(error) );
         return 0;
    }
         
    fread( cipher, 1, 2040, cipherfile);
    base64_decode( cipher, strlen((char *)cipher), out, &outlen);
    m = outlen/16; 
    for(  ; k < m; k++)
    {
         for( a = 0; a<16; a++ )
         {
              ci[a] = out[b++];
         }
         if( (error = ecb_decrypt( ci, pl[k], 16, &ecbaes)) != CRYPT_OK)
         {
              print_error();
 	      printf( "ecb_decrypt error:%s\n", error_to_string(error) );
              return 0;
         }
    }
     flag = pl[k-1][15] - 48;
    for( ; j <= k-1; j++ )
    {
         strcat( (char*)plain, (char*)pl[j]);
    }
    plain[outlen-1-flag] = 0;
    base64_decode( plain, strlen((char *)plain), out2, &outlen2);
    //fwrite( out2, 1, strlen((char *)out2), plainfile );
    fclose( cipherfile );
  //  fclose( plainfile );
    ecb_done( &ecbaes );

    /* unregister aes */
    if( (error = unregister_cipher(&aes_desc)) != CRYPT_OK )
    {
        print_error();
        printf( "unregister_cipher(&aes_desc) error:%s\n", error_to_string(error) );
	return 0;
    } 
    return 1;
 } 
