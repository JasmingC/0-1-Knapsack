
#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif



/*/// ------------------------------------------------------------------------------------





/*/// ------------------------------------------------------------------------------------


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <list>
#include <string>
#include <fstream>
using namespace std;

#define PAUSE system( "pause" );

//
// 物品結構
struct Item
{
	int id ; // 索引
	int w ; // 重量
	int p ; // 價格
	double cp ;
};
// 取行數
int getFileLine( FILE *file )
{
	int line = 0 ;
	char ch[1024] ;
	while( fgets( ch , sizeof( ch ) , file ) )
	{
		line ++ ;
	}
	fseek( file , 0 , SEEK_SET);
	return line ;
}

/*暴力法*/
/* 以遞延迴圈的方式處理 (Stack , LIFO , 後進先出 )
 	 到最後一件時，就判斷是否比剛剛的價值 (widget)還高，
   是的話就輸出所有物品的攜帶量回 outCount */
//           ↓最佳解         ↓目前記錄的量               ↓剩餘可攜重量      ↓目前總攜帶價值             物品數量   是否可多帶(0.最多一件,1.可帶多件
int RunA( int *outCount , int *runCount , Item *item , int c , int pos , int nWidget , int maxWidget , int N , int getMax )
{//                                        ↑物品陣列(數量為N)     ↑目前物品的判斷位置     ↑目前最大的總摧帶價值
	int i , ww ;

	// 算可買數量:背包容量 / 物品重量
	int n = c / item->w  ;

	/*不重複取*/
	if( getMax == 0  )
		if( n > 1 )
			n = 1 ;

	// 最後一個
	if( pos == ( N - 1 ))
	{
		// 能帶的都帶
		nWidget +=  n * item->p ; // 目前累積價值 = 目前累積價值 + 數量(1 or 0) * 該物品價值
		// run
		/*
		cout << "cost: " << nWidget << ":" ;
		for( i = 0 ; i < N ; ++i )
			cout << runCount[i] << ',' ;
		cout << endl ;*/

		// 如果比之前的高，就把可以帶的物品複製回 輸出
		// 這樣在 遞延迴圈 結束時，就可知道佳解為何
		//最大價值 < 目前累積價值
		if( maxWidget < nWidget )
		{
			// 放最後一個攜帶數量
			runCount[pos] = n ;
			// 把攜帶的數量回報給 回傳
			for( i = 0 ; i < N ; ++i )
				outCount[i] = runCount[i] ;
			// 更新最大價值
			maxWidget = nWidget ;
		}
		return maxWidget ;
	}

	// 可買的數量，從小到大算
  //                          ↓每多帶一件要減少的剩餘重量
	for( i = 0 ; i <= n ; ++i , c -= item->w , nWidget += item->p )
	{//                                         ↑每多帶一件產生的價值
		// ↓設定目前物品所有帶的量
		runCount[pos] = i ;

		// 以 遞延迴圈的方式處理
		//                              ↓這一個物品              ↓帶給下一個物品
		ww = RunA( outCount , runCount , item + 1 , c , pos + 1 , nWidget , maxWidget , N , getMax );
		// 如果這個價值比之前的高，就設為目前最高價
		if( maxWidget < ww )
			maxWidget = ww ;
		// 用完歸零
		runCount[pos] = 0 ;
	}
	return maxWidget ;
}


/*Greedy*/
//c = 容量
//N = 數量
int runCP( int *outCount , Item *item , int c , int N )
{
	int i , n ;
	//物品的二維陣列
	Item ** buf = new Item*[N] ;
	Item * temp ;
	int value ;

	for( i = 0 ; i < N ; ++i )
		buf[i] = item + i ;

	// CP大的放在前
	n = N ;
	while( n > 1 )
	{
		for( i = 1 ; i < n ; ++i )
		if( buf[i-1]->cp < buf[i]->cp )
		{
			temp = buf[i-1];
			buf[i-1] = buf[i] ;
			buf[i] = temp ;
		}
		n-- ;
	}

	// 放
	value = 0 ; //總價值
	for( i = 0 ; i < N ; ++i )
	{
		outCount[buf[i]->id] = 0 ;
		/*物品重量小於等於背包重量*/
		if(buf[i]->w <= c )
		{
			c -= buf[i]->w ;								//總容量減少
			value += buf[i]->p ;						//總價值稱加
			outCount[buf[i]->id] = 1 ;			//物品要取設為1
		}
	}


	// 結束
	delete buf ;

	return value ;
}



int main( int args , const char ** arge )
{
	Item *item ;
	int *outCount ; //取或不取
	int *runCount ;
	int widget ; //背包容量
	int i , n ; //n = 幾個物品
	FILE * p ; //文件:每個物品的價值
	FILE * c ; //文件:背包容量
	FILE * w ; //文件:每個物品的重量
	FILE *output ;
	int total_value ; //總價值
	const char * dir = "ds3" ;
	char fileName[1024] ;

	//
	if( args <= 1 )
	{
		printf( "not database dir ... ex cc4.exe ds3 \n" );
		system( "pause" );
		return 1 ;
	}

	dir = arge[1] ;

	sprintf( fileName , "%s\\p.txt" , dir );
	cout << "load file: " << fileName << endl ;
	p = fopen( fileName , "r" );
	if( p == NULL )return 0 ;



	sprintf( fileName , "%s\\c.txt" , dir );
	cout << "load file: " << fileName << endl;
	c = fopen( fileName , "r" );

	sprintf( fileName , "%s\\w.txt" , dir );
	cout << "load file: " << fileName << endl;
	w = fopen( fileName , "r" );

  //取行數
	n = getFileLine( p );
	item = new Item[n] ;
	for( i = 0 ; i < n ; ++i )
	{
		/*讀第i個物品的價值和重量*/
		fscanf( p , "%d" , &item[i].p  );
		fscanf( w , "%d" , &item[i].w );

		/*儲存索引和cp值:價值/重量*/
		item[i].id = i ;
		item[i].cp = (double)item[i].p / (double)item[i].w ;
	}

	//初始為0
	outCount = new int[n] ;
	runCount = new int[n] ;
	for( i = 0 ; i < n ; ++i )
	{
		outCount[i] = 0 ;
		runCount[i] = 0 ;
	}


  /*讀背包容量*/
	fscanf( c , "%d" , &widget );
	cout << "hw1 build... " << endl ;
	//
	total_value = RunA( outCount , runCount , item , widget , 0 , 0 , 0 , n , 0 );
	//
	sprintf( fileName , "%s\\ans_%s.txt" , dir , dir );
	cout << "output file: " << fileName << endl;
	output = fopen( fileName , "w" );

	/*輸出到檔案*/
	cout << "hw1 cost: " << total_value << endl ;
	fprintf( output , "%d\n" , total_value );
	for( i = 0 ; i < n ; ++i )
	{
		fprintf( output , "%d\n" , outCount[i] );
		printf( "%d," , outCount[i] );
	}
	fclose( output );

	/*

  homework 2

	*/
	cout << endl << endl ;
	cout << "hw2 build... " << endl;
	//
	total_value = runCP( outCount , item , widget , n );
	//
	sprintf( fileName , "%s\\ans_%s_greedy.txt" , dir , dir );
	cout << "output file: " << fileName << endl;
	output = fopen( fileName , "w" );

	cout << "hw2 cost: " << total_value << endl ;
	fprintf( output , "%d\n" , total_value );
	for( i = 0 ; i < n ; ++i )
	{
		fprintf( output , "%d\n" , outCount[i] );
		printf( "%d," , outCount[i] );
	}
	fclose( output );
	fclose( p );
	fclose( c );
	fclose( w );

	delete [] item ;
	delete outCount ;
	delete runCount ;
	return 0 ;
}
