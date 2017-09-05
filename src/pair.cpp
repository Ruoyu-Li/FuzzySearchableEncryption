/* Dynamic Searchable Symmetric Encryption
 * Coding: Shuanghong He, CS school of HUST
 * E-mail: 740310627@qq.com
 */
#include <string.h>
#include <time.h>
#include "global.h"
#include "pair.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QMessageBox"
static char         file[256] = "../original/keyword.txt";

static int read_word_from_file( pair_handle* handle, char* path)
{
	FILE* fword = fopen( path, "r" );
    int scount;
    unsigned int findex = 0;
	if( fword == 0 )
	{
        QMessageBox::warning( MainWindow::getMainWindowHandle(), "错误", "打开关键字文件出错!", QMessageBox::Yes);
		return 0;
	}
	scount = fscanf( fword, "%u\n", &(handle->word_amount) );
    QString knum;
        knum = QString::number(handle->word_amount , 10);
        MainWindow::getMainWindowHandle()->SetLineEditText(0,knum);

	/* array to store keywords */
	handle->word_list = (word*)calloc( handle->word_amount , sizeof(word) );
	if( handle->word_list == 0 )
	{
        QMessageBox::warning( MainWindow::getMainWindowHandle(), "错误", "存储关键字出错!", QMessageBox::Yes);
		return 0;
	}

	/* read keywords and calculate countings by frequency */
	scount = fscanf( fword, "%s\n",
					 handle->word_list[findex].word);
	while( scount == 1 ) // a valid key is read
	{

		findex++;
		scount = fscanf( fword, "%s\n",
						 handle->word_list[findex].word);
	}

	/* DEBUG
	print_debug();
	printf( "max word count = %d\n", handle->max_word_count );//*/

	if( findex != handle->word_amount ) // check if all words are read in file
	{
		print_error();
		printf( "word amount is incorrect\n" );
		free( handle->word_list );
		handle->word_list = 0;
		return 0;
	}

	fclose( fword );

	/* check for repeated keys */
	for( int i = 0; i < handle->word_amount; i++ )
	{
		for( int j = 0; j < i; j++ )
		{
			if( strncmp(handle->word_list[i].word, handle->word_list[j].word,
						WORD_LEN) == 0 )
			{
				print_error();
				printf( "repeated key is found at line %d, %d\n", i+2, j+2 );
				free( handle->word_list );
				handle->word_list = 0;
				return 0;
			}
		}
	}
	return 1;
}



pair_handle* pair_init( unsigned int amount,
						char* path )
{
    pair_handle* ph = (pair_handle*)calloc( 1, sizeof(pair_handle) );
	if( ph == 0 ) return 0;

	ph->pair_amount = amount;
	ph->file_path   = path  ;
    read_word_from_file( ph, file );
	return ph;
}

void pair_free( pair_handle* handle )
{
	if( handle )
	{
		if( handle->word_list ) free( handle->word_list );
		if( handle->pair_list ) free( handle->pair_list );
		free( handle );
	}
}

int pair_generate( pair_handle* handle )
{


    unsigned int scount;
    unsigned int idcount;
	FILE* fpair = fopen( handle->file_path, "r" );
	if( fpair == 0 )
	{
		print_error();
		printf( "open %s failed\n", handle->file_path );
		return 0;
	}
	
	scount = fscanf( fpair, "%u\n", &idcount );
	if( scount != 1 )
	{
		print_error();
		printf( "file format is incorrect\n" );
		return 0;
	}
    handle->pair_amount = idcount*5+100;
	if( handle->pair_list ) free( handle->pair_list );
	handle->pair_list  = (pair*)calloc( handle->pair_amount, sizeof(pair) );

	if( handle->pair_list == 0 )
	{
		print_error();
		printf( "allocation failed for generating word id pairs\n" );
		if( handle->pair_list != 0 ) free( handle->pair_list );
		handle->pair_list = 0;
		return 0;
	}

	/* generate word id pairs */
	unsigned int pair_index = 0;
 	unsigned int   id_index = 0;
    char keyword[5][WORD_LEN] = { 0 };

    while( id_index < idcount )
	{
		scount = fscanf( fpair, "%*s\t%s\t%*s\t%s\t%*s\t%s\t%*s\t%s\t%*s\t%s\n",
                                keyword[0], keyword[1], keyword[2], keyword[3], keyword[4] );
		if( scount != 5 )
		{
			print_error();
			printf( "file format is incorrect\n" );
			return 0;
		}
        for( int j = 0; j < 5; j++ )
        {
			strcpy( handle->pair_list[pair_index].word , keyword[j] );
			handle->pair_list[pair_index].id   =  id_index;
			pair_index++;
		}
        id_index++;
	}

	handle->pair_amount = pair_index;
	return 1;
}

int pair_get_item( pair_handle* handle, char* word[], unsigned int* id )
{
	if( handle->pair_index < handle->pair_amount )
	{
		*word = handle->pair_list[handle->pair_index].word;
		*id   = handle->pair_list[handle->pair_index].id  ;

		handle->pair_index++;
		return 1;
	}
	else
	{
		handle->pair_index = 0;
		return 0;
	}
}
