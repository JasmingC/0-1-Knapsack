#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS 
#endif 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


struct ITEM
{
	int w;
	int p;
	double GP;
	double cp;
	int isSelect;
} ;

// cp
void itemInit( struct ITEM* item )
{
	double c = item->w;
	double p = item->p;
	item->cp = p / c;
	item->GP = 0;
	item->isSelect = 0;
}

// �Ƨ� 
       
void sortItem( struct ITEM** itemBuf , int Size )
{
	struct ITEM* b;
	int i;
	int n = Size;
	while( n > 1 )
	{
		for( i = 1; i < n; ++i )
			if( itemBuf[i - 1]->GP < itemBuf[i]->GP )
			{
				b = itemBuf[i - 1];
				itemBuf[i - 1] = itemBuf[i];
				itemBuf[i] = b;

			}
		n--;
	}
}

//
//
struct MEMORY
{
	int Size;
	struct ITEM* pValue;
} ;

void initBuffer( struct MEMORY* pValue )
{
	memset( pValue , 0 , sizeof( struct MEMORY ) );
}

void createArray( struct MEMORY* pValue , int inCount )
{
	int s = sizeof( struct ITEM ) * inCount;
	pValue->Size = inCount;
	pValue->pValue = (struct ITEM*) malloc( s );
	memset( pValue->pValue , 0 , s );

}

void copyArray( struct MEMORY* destBuf , struct MEMORY* srcBuf )
{
	int s = sizeof( struct ITEM ) * destBuf->Size;
	memcpy( destBuf->pValue , srcBuf->pValue , s );

}

void addValue( struct MEMORY* pValue , struct ITEM* value )
{
	int n = pValue->Size;
	struct ITEM* bBuf = pValue->pValue;
	struct ITEM* nBuf;
	pValue->Size++;

	nBuf = pValue->pValue = (struct ITEM*) malloc( sizeof( struct ITEM ) * pValue->Size );

	if( bBuf != NULL )
	{
		memcpy( nBuf , bBuf , sizeof( struct ITEM ) * n );
		free( bBuf );
	}
	memcpy( nBuf + n , value , sizeof( struct ITEM ) );

}
void deleteBuf( struct MEMORY* pValue )
{
	if( pValue->pValue != NULL )
		free( pValue->pValue );
	pValue->pValue = NULL;
	pValue->Size = 0;
}

// �^�� �̨έt���A�P�ѤU�� c ��
int build( struct ITEM** pValue , double minCount , int Size , int c , int allW )
{
	int pp , ww;
	int i , rr;

	// �H�������@�Ǥ��n��(1/3
	int r = rand() % ( Size / 3 );
	for( i = 0; i < r; ++i )
	{
		rr = rand() % Size;
		pValue[rr]->isSelect = 0;
	}

	// ��w�諸�������t�� 
	for( i = 0; i < Size; ++i )
	{
		if( pValue[i]->isSelect )
			c -= pValue[i]->w;
	}
	// �H����A�@�b�N�n
	r = Size / 2;
	for( i = 0; i < r; ++i )
	{
		rr = rand() % Size;
		if( pValue[rr]->isSelect == 0 )
			if( pValue[rr]->GP >= minCount ) // �z���N���޲z
				if( pValue[rr]->w <= c )
				{
					pValue[rr]->isSelect = 1;
					c -= pValue[rr]->w;
				}
	}
	// �p�G�٦��ѡA���e����]�j���}�l��
	if( minCount > 0 )
		for( rr = 0; rr < Size; ++rr )
		{
			if( pValue[rr]->isSelect == 0 )
				if( pValue[rr]->GP >= minCount ) // �z���N���޲z
					if( pValue[rr]->w <= c )
					{
						pValue[rr]->isSelect = 1;
						c -= pValue[rr]->w;
					}
		}
	// �����
	pp = 0;
	ww = 0;
	for( i = 0; i < Size; ++i )
		if( pValue[i]->isSelect )
		{
			pp += pValue[i]->p;
			ww += pValue[i]->w;
		}
	if( ww > allW )
		printf( "err: %d/%d\n" , ww , allW );
	//
	return pp;

}

// �̤j��
double makeBuf( struct MEMORY* pValue )
{
	double ret = -1;
	struct ITEM* bb = pValue->pValue;
	int i;
	for( i = 0; i < pValue->Size; ++i )
		if( bb[i].isSelect )// ��W�� +10
		{
			bb[i].GP += 10;
			if( ( ret <= 0 ) || ( ret < bb[i].GP ) )
				ret = bb[i].GP;
		} else // �S�Q��W���A�ܦ���Ӫ� 90%
		{
			bb[i].GP *= 0.90;
		}
		return ret;
}
//
// �{����J�I
int main( int args , const char* argc[] )
{
	// ���榸��
	const int runCount = 1000;
	// �P�_����
	const int checkCount = 100;
	// �X�����ܴN����
	const int noReturn = 20;

	struct MEMORY pBuf;
	struct MEMORY getBuf;
	int c , n , v , value;
	int minWdigetValue;
	int i;
	FILE* file;
	FILE* wfile;
	FILE* pfile;
	struct ITEM item;
	int pp0 , pp1;
	struct ITEM** itemBuf;
	int  k;
	int ww , pp;
	int rCount = 0;
	int cCount = 0;

	initBuffer( &pBuf );
	initBuffer( &getBuf );

	// ��J��Ƨ�
	char dir[256];//= "ds4";
	printf( "Enter dir (Ex:ds3): " );
	scanf( "%s" , dir );
	//sprintf( dir ,  "ds%d" , filaIndex );
	// ��X�ɮ�
	char saveFile[256];//= "ans_ds4.txt" ;
	printf( "Enter outFileName(Ex:ans_ds3.txt): " );
	scanf( "%s" , saveFile );

	char fileName[256];

	sprintf( fileName , "%s\\c.txt" , dir );
	file = fopen( fileName , "r" );
	if( file == NULL )
	{
		printf( "open file error: %s" , fileName );
		return 0;
	}

	fscanf( file , "%d" , &c );
	fclose( file );

	// p
	sprintf( fileName , "%s\\w.txt" , dir );
	wfile = fopen( fileName , "r" );
	sprintf( fileName , "%s\\p.txt" , dir );
	pfile = fopen( fileName , "r" );
	while( 1 )
	{
		itemInit( &item );
		if( feof( wfile ) )
			break;
		if( fscanf( wfile , "%d" , &item.w ) < 0 )
			break;
		if( feof( pfile ) )
			break;
		if( fscanf( pfile , "%d" , &item.p ) < 0 )
			break;
		addValue( &pBuf , &item );
	}
	fclose( wfile );
	fclose( pfile );

	// ��
	itemBuf = (struct ITEM**) malloc( sizeof( struct ITEM* ) * pBuf.Size );
	for( i = 0; i < pBuf.Size; ++i )
		itemBuf[i] = pBuf.pValue + i;
	// �Ƨ�        
//	sortItem( itemBuf , pBuf.Size );
	//
	// �}�l�A�򥻪������
	createArray( &getBuf , pBuf.Size );
	pp1 = pp0 = build( itemBuf , 0 , pBuf.Size , c , c );
	copyArray( &getBuf , &pBuf );
	double minGA = 0;
	double nowGA = 0;
	double maxGP = 0;
	for( k = 0; k < runCount; ++k )
	{
		// �̤p��]
		minGA = ( (maxGP) *i ) / runCount;
		//
		// �C�����@������
		cCount = 0;
		for( i = 0; i < checkCount; ++i )
		{
			//�^�ǳ̨έt�� 
			pp0 = build( itemBuf , minGA , pBuf.Size , c , c );
			// �u�n������j�N�[
			if( pp1 < pp0 )
			{
				pp1 = pp0;
				copyArray( &getBuf , &pBuf );
				cCount++;
			}//else // �٭�
		}

		if( 0 )
			if( cCount == 0 )
			{
				rCount++;
				if( rCount >= noReturn )
					break;

			} else
				rCount = 0;


			// �Хܦ��[�J���N�Х�
			nowGA = makeBuf( &getBuf );
			// �٭�^�h
			copyArray( &pBuf , &getBuf );

			// �Ƨ�
			sortItem( itemBuf , pBuf.Size );

	}

	// ��X
	//sprintf( fileName , "%s%s" , dir , saveFile );
//	fprintf( file , "%d\n" , pp1 );
	ww = pp = 0;

	for( i = 0; i < getBuf.Size; ++i )
	{
		struct ITEM* ii = getBuf.pValue + i;
		fprintf( stdout , "%4d: %4d p: %4d w: %4d GA: %8f cp: %8f\n"
			, i
			, ii->isSelect , ii->p , ii->w
			, ii->GP , ii->cp );

		// �Q�襤
		if( ii->isSelect )
		{
			ww += ii->w;
			pp += ii->p;
		}
		//	fprintf( stdout , "%d\n" , getBuf.pValue[i].isSelect );
	}

	file = fopen( saveFile , "w" );
	fprintf( file , "%d\n" , pp );
	for( i = 0; i < getBuf.Size; ++i )
		fprintf( file , "%d\n" , getBuf.pValue[i].isSelect );

	fclose( file );
	
	printf( "max c: %d pp: %d ww: %d \n" , c , pp , ww );

	system( "pause" );
	return 0;
}

