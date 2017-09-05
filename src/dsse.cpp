/* Dynamic Searchable Symmetric Encryption
 * Coding: Shuanghong He, CS school of HUST
 * E-mail: 740310627@qq.com
 */
#include <string.h>
#include "global.h"
#include "crypto.h"
#include <tomcrypt.h>
#include "dsse.h"
#include "time.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
static int handle_counter; // share by server and client

/* compare function used in avl tree */

int random_word_cmp( void* a, void* b )
{
	random_word* rw1 = (random_word*) a;
	random_word* rw2 = (random_word*) b;

	return strcmp( rw1->word, rw2->word );
}

int random_id_cmp( void* a, void* b)
{
	random_id* ri1 = (random_id*) a;
	random_id* ri2 = (random_id*) b;

	if( ri1->id < ri2->id ) // unsigned int compare, OVERFLOW ATTENTION
		return -1;
	else if( ri1->id == ri2->id )
		return 0;
	else
		return 1;
}

int cipher_word_cmp( void* a, void* b )
{
	cipher_word* cw1 = (cipher_word*) a;
	cipher_word* cw2 = (cipher_word*) b;

	return memcmp( cw1->part1, cw2->part1, BASE_LEN );
}

int cipher_id_cmp( void* a, void* b )
{
	cipher_id* ci1 = (cipher_id*) a;
	cipher_id* ci2 = (cipher_id*) b;

	return memcmp( ci1->part1, ci2->part1, BASE_LEN );
}

int cipher_mix_cmp( void* a, void* b )
{
	cipher_mix* cm1 = (cipher_mix*) a;
	cipher_mix* cm2 = (cipher_mix*) b;

	return memcmp( cm1->part1, cm2->part1, BASE_LEN );
}

/* client method */

static inline int dsse_make_head_cw( secret_keys*  skeys,
									 cipher_word*  cword,
									 unsigned int  id,
									 char          word[],
									 unsigned char next[],
									 unsigned char random[] )
{
	/* part1 */
	// F( k1, w )
	if( crypto_hmac(skeys->key1,
					(unsigned char*)word, strlen(word),
					cword->part1, BASE_LEN ) == 0 )
	{
		print_error();
		printf( "F( k1, w ) failed\n" );
		return 0;
	}

	/* part2 */
	// P(k2,w)
	if( crypto_hmac(skeys->key2,
					(unsigned char*)word, strlen(word),
					cword->part2, BASE_LEN ) == 0 )
	{
		print_error();
		printf( "P(k2,w) failed\n" );
		return 0;
	}
	crypto_hash( cword->part2, random, cword->part2, BASE_LEN*2+1 );
	// XOR
	for( int i = BASE_LEN*2, j = BASE_LEN-1; i >= 0 && j >= 0 ; i--, j-- )
		cword->part2[i] ^= next[j];
	for( int i = BASE_LEN, j = sizeof(unsigned int) - 1;
		 i >= 0 && j >= 0 ; i--, j-- )
		cword->part2[i] ^= ((unsigned char*)(&id))[j];

	/* part3 */
	memcpy( cword->part3, random, BASE_LEN );

	return 1;
}

static inline int dsse_make_next_cw( secret_keys*  skeys,
									 cipher_word*  cword,
									 unsigned char index[],
									 unsigned int  id,
									 char          word[],
									 unsigned char next[],
									 unsigned char random[] )
{
	/* part1 */
	memcpy( cword->part1, index, BASE_LEN );

	/* part2 */
	// P(k2,w)
	if( crypto_hmac(skeys->key2,
					(unsigned char*)word, strlen(word),
					cword->part2, BASE_LEN ) == 0 )
	{
		print_error();
		printf( "P(k2,w) failed\n" );
		return 0;
	}
	crypto_hash( cword->part2, random, cword->part2, BASE_LEN*2+1 );
	// XOR
	for( int i = BASE_LEN * 2, j = BASE_LEN - 1;
		 i >= 0 && j >= 0 ; i--, j-- )
		cword->part2[i] ^= next[j];
	for( int i = BASE_LEN, j = sizeof(unsigned int) - 1;
		 i >= 0 && j >= 0 ; i--, j-- )
		cword->part2[i] ^= ((unsigned char*)(&id))[j];

	/* part3 */
	memcpy( cword->part3, random, BASE_LEN );

	return 1;
}

static inline int dsse_make_head_ci( secret_keys*  skeys,
									 cipher_id*    cid,
									 unsigned int  id,
									 char          word[],
									 unsigned char cw1[],
									 unsigned char next[],
									 unsigned char random[] )
{
	unsigned char blob[BASE_LEN];

	/* part1 */
	// F(k1,id)
	if( crypto_hmac(skeys->key1,
					(unsigned char*)(&id), sizeof(unsigned int),
					cid->part1, BASE_LEN ) == 0 )
	{
		print_error();
		printf( "F(k1,id) failed\n" );
		return 0;
	}

	/* part2 */
	// P(k2,id)
	if( crypto_hmac(skeys->key2,
					(unsigned char*)(&id), sizeof(unsigned int),
					cid->part2, BASE_LEN ) == 0 )
	{
		print_error();
		printf( "P(k2,id) failed\n" );
		return 0;
	}
	crypto_hash( cid->part2, random, cid->part2, BASE_LEN*3+1 );
	// F(k1,id,w)
	if( crypto_hmad(skeys->key1,
					(unsigned char*)(&id), sizeof(unsigned int),
					(unsigned char*)word, strlen(word),
					blob, BASE_LEN) == 0 )
	{
		print_error();
		printf( "F(k1,id,w) failed\n");
		return 0;
	}
	// XOR
	for( int i = BASE_LEN * 3, j = BASE_LEN - 1; i >= 0 && j >= 0; i--, j-- )
		cid->part2[i] ^= next[j];
	for( int i = BASE_LEN * 2, j = BASE_LEN - 1; i >= 0 && j >= 0; i--, j-- )
		cid->part2[i] ^= blob[j];
	for( int i = BASE_LEN,     j = BASE_LEN - 1; i >= 0 && j >= 0; i--, j--)
		cid->part2[i] ^= cw1[j];

	/* part3 */
	memcpy( cid->part3, random, BASE_LEN );
	return 1;
}

static inline int dsse_make_next_ci( secret_keys*  skeys,
									 cipher_id*    cid,
									 unsigned char index[],
									 unsigned int  id,
									 char          word[],
									 unsigned char cw1[],
									 unsigned char next[],
									 unsigned char random[] )
{
	unsigned char blob[BASE_LEN];

	/* part1 */
	memcpy( cid->part1, index, BASE_LEN );

	/* part2 */
	// P(k2,id)
	if( crypto_hmac(skeys->key2,
					(unsigned char*)(&id), sizeof(unsigned int),
					cid->part2, BASE_LEN ) == 0 )
	{
		print_error();
		printf( "P(k2,id) failed\n" );
		return 0;
	}
	crypto_hash( cid->part2, random, cid->part2, BASE_LEN*3+1 );
	// F(k1,id,w)
	if( crypto_hmad(skeys->key1,
					(unsigned char*)(&id), sizeof(unsigned int),
					(unsigned char*)word, strlen(word),
					blob, BASE_LEN) == 0 )
	{
		print_error();
		printf( "F(k1,id,w) failed\n");
		return 0;
	}
	// XOR
	for( int i = BASE_LEN * 3, j = BASE_LEN - 1; i >= 0 && j >= 0; i--, j-- )
		cid->part2[i] ^= next[j];
	for( int i = BASE_LEN * 2, j = BASE_LEN - 1; i >= 0 && j >= 0; i--, j-- )
		cid->part2[i] ^= blob[j];
	for( int i = BASE_LEN,     j = BASE_LEN - 1; i >= 0 && j >= 0; i--, j-- )
		cid->part2[i] ^= cw1[j];

	/* part3 */
	memcpy( cid->part3, random, BASE_LEN );
	return 1;
}

static inline int dsse_make_mix( secret_keys*  skeys,
								 cipher_mix*   cmix,
								 unsigned int  id,
								 char          word[],
								 unsigned char cw1[],
								 unsigned char cid1[],
								 unsigned char random[] )
{
	/* part 1 */
	if( crypto_hmad(skeys->key1,
					(unsigned char*)(&id), sizeof(unsigned int),
					(unsigned char*)word, strlen(word),
					cmix->part1, BASE_LEN) == 0 )
	{
		print_error();
		printf( "F(k1,id,w) failed\n");
		return 0;
	}

	/* part 2 */
	if( crypto_hmad(skeys->key2,
					(unsigned char*)(&id), sizeof(unsigned int),
					(unsigned char*)word, strlen(word),
					cmix->part2, BASE_LEN) == 0 )
	{
		print_error();
		printf( "P(k2,id,w) failed\n");
		return 0;
	}

	crypto_hash( cmix->part2, random, cmix->part2, BASE_LEN*2+1 );
	// XOR
	for( int i = BASE_LEN * 2, j = BASE_LEN - 1; i >= 0 && j >= 0; i--, j-- )
		cmix->part2[i] ^= cw1[j];
	for( int i = BASE_LEN,     j = BASE_LEN - 1; i >= 0 && j >= 0; i--, j-- )
		cmix->part2[i] ^= cid1[j];

	/* part 3 */
	memcpy( cmix->part3, random, BASE_LEN );

	return 1;
}

static inline int dsse_enc( dsse_client_handle* handle,
							unsigned int        id,
							char                word[],
							cipher_word*        cword,
							cipher_id*          cid,
							cipher_mix*         cmix )
{
	random_word* rw_record; // used to acquire avl tree search result
	random_word  rw_search; // used to search in avl tree
	random_id*   ri_record;
	random_id    ri_search;
	unsigned char random[BASE_LEN];

	/* 1.seek record (w,Pw) by w */
	rw_search.word = word;
	rw_record = (random_word*)avl_find( handle->pstates.word, &rw_search );
	if( rw_record ) // 3. otherwise
	{
		/* DEBUG
		print_debug();
		printf( "pstates word %s exists\n", word ); //*/

		crypto_random( rw_search.random, BASE_LEN ); // Pw' <- random;
		crypto_random( random,           BASE_LEN ); // R  <- random

		// Cw = [ Pw, H(P(k2,w),R) XOR (0||id||Pw), R ]
		if( dsse_make_next_cw(&(handle->skeys), cword, rw_record->random,
							  id, word, rw_search.random, random) == 0 )
		{
			print_error();
			printf( "generate new cipher word failed\n" );
			return 0;
		}

		// updates Pw = Pw'
		memcpy( rw_record->random, rw_search.random, BASE_LEN );
	}
	else // 2. if not exist
	{
		/* DEBUG
		print_debug();
		printf( "pstates word %s dose not exist\n", word ); //*/

		crypto_random( rw_search.random, BASE_LEN ); // Pw <- random
		crypto_random( random,           BASE_LEN ); // R  <- random

		// Cw = [ F(k1,w), H(P(k2,w),R) XOR (0||id||Pw), R ]
		if( dsse_make_head_cw(&(handle->skeys), cword,
							  id, word, rw_search.random, random) == 0 )
		{
			print_error();
			printf( "generate new cipher word failed\n" );
			return 0;
		}

		// inserts record (w,Pw) into Tp
		if( avl_add(handle->pstates.word, &rw_search) == 0 )
		{
			print_error();
			printf( "insert record (w,Pw) into Tp failed\n" );
			return 0;
		}
	}

	// 5. Seeks record (id,Pid) by id in Tp
	ri_search.id = id;
	ri_record = (random_id*)avl_find( handle->pstates.id, &ri_search );
	if( ri_record ) // 7. Otherwise
	{
		/* DEBUG
		print_debug();
		printf( "pstates id %u exists\n", id ); //*/

		crypto_random( ri_search.random, BASE_LEN ); // Pid' <- random
		crypto_random( random,           BASE_LEN ); // R    <- random

		// Cid = [ Pid ,G(P(k2,id),R) XOR (0||Cw,1||F(k1,id,w)||Pid), R ]
		if( dsse_make_next_ci(&(handle->skeys), cid,
							  ri_record->random, id, word, cword->part1,
							  ri_search.random, random) == 0 )
		{
			print_error();
			printf( "generate new cipher id failed\n" );
			return 0;
		}

		// updates Pid = Pid'
		memcpy( ri_record->random, ri_search.random, BASE_LEN );
	}
	else // 6. If not exist
	{
		/* DEBUG
		print_debug();
		printf( "pstates id %u dose not exist\n", id ); //*/

		crypto_random( ri_search.random, BASE_LEN ); // Pid <- random
		crypto_random( random,           BASE_LEN ); // R   <- random

		// Cid = [ F(k1,id), G(P(k2,id),R) XOR (0||Cw_1||F(k1,id,w)||Pid), R ]
		if( dsse_make_head_ci(&(handle->skeys), cid,
							  id, word, cword->part1,
							  ri_search.random, random) == 0 )
		{
			print_error();
			printf( "generate new cipher id failed\n" );
			return 0;
		}

		// inserts record (id,Pid) into Tp;
		if( avl_add(handle->pstates.id, &ri_search) == 0 )
		{
			print_error();
			printf( "insert record (id,Pid) into Tp failed\n" );
			return 0;
		}
	}

	// Cid_w = [ F(k1,id,w), H(P(k2,id, w),R) XOR (0||Cid_1||Cw_1), R)
	if( dsse_make_mix(&(handle->skeys),
					  cmix, id, word, cword->part1, cid->part1, random) == 0)
	{
		print_error();
		printf( "generate cipher id word failed\n" );
		return 0;
	}

	return 1;
}

dsse_client_handle* dsse_client_init( void )
{
	if( handle_counter == 0 && crypto_init() == 0 )
		return 0;

    dsse_client_handle* dch = (dsse_client_handle*)calloc( 1, sizeof(dsse_client_handle) );
	if( dch == 0 )
	{
		crypto_free();
		return 0;
	}

	/* init client private states */
	dch->pstates.word = avl_init( sizeof(random_word), random_word_cmp );
	dch->pstates.id   = avl_init( sizeof(random_id),   random_id_cmp   );
	if( dch->pstates.word == 0 || dch->pstates.id == 0 )
	{
		print_error();
		printf( "init dsse client handle error\n" );
		if( dch->pstates.word ) avl_free( dch->pstates.word );
		if( dch->pstates.id   ) avl_free( dch->pstates.id   );
		free( dch );
		crypto_free();
		return 0;
	}

	/* init client secret keys */
    if( crypto_random(dch->skeys.key1, SKEY_LEN) == 0 || crypto_random(dch->skeys.key2, SKEY_LEN) == 0 )
	{
		print_error();
		printf( "generate private key error\n" );
		avl_free( dch->pstates.word );
		avl_free( dch->pstates.id   );
		free( dch );
		crypto_free();
		return 0;
    }
//        char enckey1[32] = {0};
//        memcpy(enckey1, dch->skeys.key1, SKEY_LEN);
//        QMessageBox::information(MainWindow::getMainWindowHandle(), "提示",enckey1, QMessageBox::Yes);

        char key1[32]={0},key2[32]={0};
        unsigned long outlen1=32,outlen2=32;
        base64_encode( dch->skeys.key1, SKEY_LEN, (unsigned char*)key1, &outlen1);
        MainWindow::getMainWindowHandle()->SetLineEditText(5,(char*)key1);
        base64_encode( dch->skeys.key2, SKEY_LEN,(unsigned char*)key2, &outlen2);
        MainWindow::getMainWindowHandle()->SetLineEditText(6,(char*)key2);

        unsigned char dkey[32]={0};
        unsigned long outlen3=16;
        base64_decode((unsigned char*)key1, strlen((char*)key1), dkey, &outlen3);
        //QMessageBox::information(MainWindow::getMainWindowHandle(), "提示",(char*)dkey, QMessageBox::Yes);

        handle_counter++;
        return dch;
}

int dsse_client_add( dsse_client_handle* handle,
					 unsigned int        id,
					 char                word[],
					 cipher_word*        cword,
					 cipher_id*          cid,
					 cipher_mix*         cmix )
{
	if( dsse_enc(handle, id, word, cword, cid, cmix) == 0 )
	{
		print_error();
		printf( "encrypt id and word failed\n" );
		return 0;
	}

	return 1;
}

int dsse_client_search( dsse_client_handle* handle,
						char                word[],
						dsse_token*         token )
{
	//set_time_stamp(); // TEST
	
	// F(k1,w)
	if( crypto_hmac( handle->skeys.key1,
					 (unsigned char*)word, strlen(word),
					 token->token1, BASE_LEN) == 0 )
	{
		print_error();
		printf( "generate search token1 failed\n" );
		return 0;
	}

	// P(k2,w)
	if( crypto_hmac( handle->skeys.key2,
					 (unsigned char*)word, strlen(word),
					 token->token2, BASE_LEN) == 0 )
	{
		print_error();
		printf( "generate search token2 failed\n" );
		return 0;
	}
	
	/* TEST
	printf( "token. " );
	get_time_usage(); //*/

	return 1;
}

int dsse_client_delete_id( dsse_client_handle* handle,
						   unsigned int        id,
						   dsse_token*         token )
{
	// F(k1,id)
	if( crypto_hmac(handle->skeys.key1,
					(unsigned char*)(&id), sizeof(unsigned int),
					token->token1, BASE_LEN) == 0 )
	{
		print_error();
		printf( "generate delete id token1 failed\n" );
		return 0;
	}

	// P(k2,id)
	if( crypto_hmac(handle->skeys.key2,
					(unsigned char*)(&id), sizeof(unsigned int),
					token->token2, BASE_LEN) == 0 )
	{
		print_error();
		printf( "generate delete id token2 failed\n" );
		return 0;
	}

	return 1;
}

int dsse_client_delete_mix( dsse_client_handle* handle,
							unsigned int        id,
							char                word[],
							dsse_token*         token )
{
	// F(k1,id,w)
	if( crypto_hmad(handle->skeys.key1,
					(unsigned char*)(&id), sizeof(unsigned int),
					(unsigned char*)word, strlen(word),
					token->token1, BASE_LEN) == 0 )
	{
		print_error();
		printf( "generate delete word token1 failed\n" );
		return 0;
	}

	// P(k2,id,w)
	if( crypto_hmad(handle->skeys.key2,
					(unsigned char*)(&id), sizeof(unsigned int),
					(unsigned char*)word, strlen(word),
					token->token2, BASE_LEN) == 0 )
	{
		print_error();
		printf( "generate delete word token2 failed\n" );
		return 0;
	}

	return 1;
}

void dsse_client_free( dsse_client_handle* handle )
{
	if( handle )
	{
		avl_free( handle->pstates.word );
		avl_free( handle->pstates.id   );
		free( handle );
		handle_counter--;
		if( handle_counter == 0 ) crypto_free();
	}
}

dsse_server_handle* dsse_server_init( void )
{
	if( handle_counter == 0 && crypto_init() == 0 )
		return 0;

    dsse_server_handle* dsh = (dsse_server_handle*)calloc( 1, sizeof(dsse_server_handle) );
	if( dsh == 0 )
	{
		crypto_free();
		return 0;
	}

	dsh->word = avl_init( sizeof(cipher_word), cipher_word_cmp );
	dsh->id   = avl_init( sizeof(cipher_id)  , cipher_id_cmp   );
	dsh->mix  = avl_init( sizeof(cipher_mix) , cipher_mix_cmp  );

	if( dsh->word == 0 || dsh->id  == 0 || dsh->mix == 0 )
	{
		print_error();
		printf( "init dsse server handle failed\n" );
		if( dsh->word ) free( dsh->word );
		if( dsh->id   ) free( dsh->id   );
		if( dsh->mix  ) free( dsh->mix  );
		crypto_free();
		free( dsh );
		return 0;
	}

	handle_counter++;
	return dsh;
}

int dsse_server_add( dsse_server_handle* handle,
					 cipher_word*        cword,
					 cipher_id*          cid,
					 cipher_mix*         cmix )
{
	//* RELEASE
	if( avl_add(handle->word, cword) == 0
	 || avl_add(handle->id  , cid  ) == 0
	 || avl_add(handle->mix , cmix ) == 0 )
	{
		print_error();
		printf( "add cipher text to server failed\n" );
		avl_delete( handle->word, cword );
		avl_delete( handle->id  , cid   );
		avl_delete( handle->mix , cmix  );
		return 0;
	} //*/

	return 1;
}

int dsse_server_search( dsse_server_handle* handle,
						dsse_token* token,
						list_handle* I,
						list_handle* J )
{
	unsigned int index = 1;
	cipher_word* word_record;
	cipher_word  word_search;
	cipher_word* word_old = 0;
	unsigned char tag;
	unsigned int id;
	unsigned char pw[BASE_LEN];
	unsigned char pw_old[BASE_LEN] = {0};

	memcpy( word_search.part1, token->token1, BASE_LEN );
	//set_time_stamp(); // TEST
    word_record = (cipher_word*)avl_find( handle->word, &word_search );

	while( word_record )
	{
		//set_time_stamp(); // TEST

		// Cw_2 XOR H(P(k2,w),Cw_3)
		crypto_hash( token->token2, word_record->part3,
					 word_search.part2, BASE_LEN*2+1 );
		for( int i = 0; i < BASE_LEN * 2 + 1; i++ )
			word_search.part2[i] ^= word_record->part2[i];

		// computes tag||id||Pw_i'
		tag = word_search.part2[0];
		for( int i = BASE_LEN, j = sizeof(unsigned int) - 1;
			 i >= 0 && j >= 0; i--, j-- )
			((unsigned char*)(&id))[j] = word_search.part2[i];
		for( int i = BASE_LEN * 2, j = BASE_LEN - 1;
			 i >= 0 && j >= 0; i--, j-- )
			pw[j] = word_search.part2[i];

		if( tag == 0 )
		{
			if( index < 2 ) index++;
			memcpy( word_search.part1, pw, BASE_LEN );
			list_add( I, &id ); //printf( "I %u ", id ); // I
		}
		else if( tag == 1 && index >= 2 )
		{
			//set_time_stamp(); // TEST

			for( int i = 0; i < BASE_LEN; i++ )
				pw_old[i] ^= pw[i];
			for( int i = BASE_LEN * 2, j = BASE_LEN - 1;
				 i >= 0 && j >= 0; i--, j-- )
				word_old->part2[i] ^= pw_old[j];

			memcpy( word_search.part1, pw, BASE_LEN );
			avl_delete( handle->word, word_record );

			index = 10; // delete flag

			list_add( J, &id ); //printf( "I' %u ", id ); // I'
		}
		else if( tag == 1 && index == 1 )
		{
			index++;
			memcpy( word_search.part1, pw, BASE_LEN );
			list_add( J, &id ); //printf( "I' %u ", id ); // I'
		}
		else
		{
			print_error();
            break;
		}

		if( index == 10 )
			index = 2;
		else
			word_old = word_record;
		memcpy( pw_old, pw, BASE_LEN );

		//set_time_stamp(); // TEST
        word_record = (cipher_word*)avl_find( handle->word, &word_search );
		/* TEST
		printf( "search. " );
		get_time_usage(); //*/
	}

	return 1;
}

int dsse_server_delete_id( dsse_server_handle* handle,
						   dsse_token* token )
{
	cipher_id*   id_record;
	cipher_id    id_search;
	cipher_word* word_record;
	cipher_word  word_search;
	cipher_mix   mix_search;
	unsigned char tag;

	memcpy( id_search.part1, token->token1, BASE_LEN );
    id_record = (cipher_id*)avl_find( handle->id, &id_search );
	while( id_record )
	{
		// Cid_2 XOR G(P(k2,id),Cid_3)
		crypto_hash( token->token2, id_record->part3,
					 id_search.part2, BASE_LEN*3+1 );
		for( int i = 0; i < BASE_LEN * 3 + 1; i++ )
			id_search.part2[i] ^= id_record->part2[i];

		// computes tag||Pw_i||Pid_w_i||Pid_i
		tag = id_search.part2[0];
		for( int i = BASE_LEN, j = BASE_LEN - 1;
			 i >= 0 && j >= 0; i--, j-- )
			word_search.part1[j] = id_search.part2[i];
		for( int i = BASE_LEN * 2, j = BASE_LEN - 1;
			 i >= 0 && j >= 0; i--, j-- )
			mix_search.part1[j] = id_search.part2[i];
		for( int i = BASE_LEN * 3, j = BASE_LEN - 1;
			 i >= 0 && j >= 0; i--, j-- )
			id_search.part1[j] = id_search.part2[i];
		avl_delete( handle->id, id_record );

		if( tag == 0 )
		{
            word_record = (cipher_word*)avl_find( handle->word, &word_search );
			word_record->part2[0] ^= 1;

			avl_delete( handle->mix, &mix_search );
		}

        id_record = (cipher_id*)avl_find( handle->id, &id_search );
	}
	return 1;
}

int dsse_server_delete_mix( dsse_server_handle* handle,
							dsse_token* token )
{
	cipher_mix*  mix_record;
	cipher_mix   mix_search;
	cipher_id*   id_record;
	cipher_id    id_search;
	cipher_word* word_record;
	cipher_word  word_search;
	memcpy( mix_search.part1, token->token1, BASE_LEN );
    mix_record = (cipher_mix*)avl_find( handle->mix, &mix_search );
	if( mix_record == 0 ) return 0;

	// Cid_w_2 XOR H(P(k2,id,w),Cid_w_3)
	crypto_hash( token->token2, mix_record->part3,
				 mix_search.part2, BASE_LEN*2+1 );
	for( int i = 0; i < BASE_LEN * 2 + 1; i++ )
		mix_search.part2[i] ^= mix_record->part2[i];

	// computes tag||Pw||Pid
	for( int i = BASE_LEN,     j = BASE_LEN - 1; i >= 0 && j >= 0; i--, j-- )
		id_search.part1[j] = mix_search.part2[i];
	for( int i = BASE_LEN * 2, j = BASE_LEN - 1; i >= 0 && j >= 0; i--, j-- )
		word_search.part1[j] = mix_search.part2[i];
	avl_delete( handle->mix, mix_record );

    id_record = (cipher_id*)avl_find( handle->id, &id_search );
	id_record->part2[0] ^= 1;

    word_record = (cipher_word*)avl_find( handle->word, &word_search);
	word_record->part2[0] ^= 1;

	return 1;
}

void dsse_server_free( dsse_server_handle* handle )
{
	if( handle )
	{
		avl_free( handle->word );
		avl_free( handle->id   );
		avl_free( handle->mix  );
		free( handle );
		handle_counter--;
		if( handle_counter == 0 ) crypto_free();
	}
}
