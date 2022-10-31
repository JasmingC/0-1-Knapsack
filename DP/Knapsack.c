#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



struct MEMORY
{
	int Size ; 
	int *pValue ;
}  ;

void initArray( struct MEMORY* buf )
{
	memset( buf , 0 , sizeof( struct MEMORY ));
}

void createArray( struct MEMORY* buf , int inCount )
{
	int s = sizeof( int ) * inCount;
	buf->Size = inCount ;
	 buf->pValue = (int*) malloc( s);
	 memset( buf->pValue , 0 , s );

}

void copyArray( struct MEMORY* destBuf , struct MEMORY* srcBuf )
{
	int s = sizeof( int ) * destBuf->Size;
	memcpy( destBuf->pValue , srcBuf->pValue , s );

}

void addValue( struct MEMORY* buf , int value )
{
	int i ;
	int n = buf->Size ; 
	int *bBuf = buf->pValue ; 
	int *nBuf ;
	buf->Size ++ ; 
	nBuf = buf->pValue = (int*)malloc( sizeof( int ) * buf->Size );
	i = 0 ;
	if( bBuf != NULL )
	{
		for( i = 0 ; i< n ; ++i )
			nBuf[i] = bBuf[i] ;
		free( bBuf );
	}
	nBuf[i] = value ;

}
void delArray( struct MEMORY* buf )
{
	if( buf->pValue != NULL )
		free( buf->pValue );
	buf->pValue = NULL ;
	buf->Size = 0 ;
}


// 程式輸入點
int main( int args , const char *argc[] )
{

	struct MEMORY arrayW ;
	struct MEMORY arrayP;

	struct MEMORY teamBuf[1001];

	struct MEMORY outGetItem ;
	int c , v , value ;
	int minWdigetValue ;
	int i,j ;
	FILE *file ;

	int outMaxCost = 0;
	int a , b;
	int runW;

	// 輸入資料夾
	char dir[256] ;//= "ds4";
	printf( "Enter dir (Ex:ds3): " );
	scanf( "%s" , dir );
	//sprintf( dir ,  "ds%d" , filaIndex );
	// 輸出檔案
	char saveFile[256] ;//= "ans_ds4.txt" ;
	printf( "Enter outFileName(Ex:ans_ds3.txt): " );
	scanf( "%s" , saveFile );

	char fileName[256] ;

	// c 背包最大容量 
	sprintf( fileName , "%s\\c.txt" , dir );
	printf( "load file: %s\n" , fileName );
	file = fopen( fileName , "r" );
	if( file == NULL )
	{
		printf( "open file error: %s" , fileName );
		return 0;
	}

	fscanf( file , "%d" , &c );
	fclose( file );

	// p 每個物品分別價值 
	initArray( &arrayP );
	sprintf( fileName , "%s\\p.txt" , dir );
	printf( "load file: %s\n" , fileName );
	file = fopen( fileName , "r" );
	if( file == NULL )
	{
		printf( "open file error: %s" , fileName );
		return 0;
	}
	while( 1 )
	{
		if( feof( file ))
			break ;
		if( fscanf( file , "%d" , &v ) < 0 )
			break ;
		addValue( &arrayP , v );
	}
	fclose( file );


	// w 每個物品分別重量 
	initArray( &arrayW );
	sprintf( fileName , "%s\\w.txt" , dir );
	printf( "load file: %s\n" , fileName );
	file = fopen( fileName , "r" );
	if( file == NULL )
	{
		printf( "open file error: %s" , fileName );
		return 0;
	}
	while( 1 )
	{
		if( feof( file ) )
			break;
		if( fscanf( file , "%d" , &v ) < 0 )
			break;
		addValue( &arrayW , v );

	}

	fclose( file );

	// build

	
	// 建 array
	for( i = 0 ; i <= arrayW.Size ; ++i )
	{
		createArray( &teamBuf[i] , c + 1 );
	}
	createArray( &outGetItem , arrayW.Size );


	// 陣列
	//這個重量可拿的最大價值是多少 
	for( i = 0; i <= arrayW.Size; i++ )
	{
		printf( "build: index %d\n" , i );
		for( runW = 0; runW <= c; runW++ )
		{
			if( i == 0 || runW == 0 )
				teamBuf[i].pValue[runW] = 0;
				
			//重量沒超過 
			else if( arrayW.pValue[i - 1] <= runW )
			{
				//拿 
				a = arrayP.pValue[i - 1] + teamBuf[i - 1].pValue[runW - arrayW.pValue[i - 1]];
				//不拿 
				b = teamBuf[i - 1].pValue[runW];
				if( a > b )
				{
					teamBuf[i].pValue[runW] = a;
				} else
				{
					teamBuf[i].pValue[runW] = b;

				}
			} 
			//重量超過，不拿 
			else
			{
				teamBuf[i].pValue[runW] = teamBuf[i - 1].pValue[runW];
			}
		}
	}
	
	for( i = arrayW.Size - 1 , j = c; i >= 0; --i )
	{

		if( j - arrayW.pValue[i] >= 0 &&
			teamBuf[i + 1].pValue[j] == teamBuf[i].pValue[j - arrayW.pValue[i]] + arrayP.pValue[i] )
		{
			j -= arrayW.pValue[i];
			outGetItem.pValue[i] = 1;
		}
	}
	v = teamBuf[arrayW.Size].pValue[runW - 1] ;
	printf( "max value %d\n" , v );




	// 輸出
	//sprintf( fileName , "%s%s" , dir , saveFile );
	file = fopen( saveFile , "w" );
	fprintf( file ,  "%d\n", v  );
	for( i = 0; i < arrayP.Size; ++i )
		fprintf( file , "%d\n" , outGetItem.pValue[i] );
	fclose( file );

	system( "pause" );

	return 0 ; 
}
