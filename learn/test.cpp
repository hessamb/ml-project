#include<iostream>
#include "SGDMF.h"
#include "../read/netflix.h"
#include "../conf.h"

using namespace std ;

int main()
{
	NetflixReaderRAM buff(NETFLIX_MOVIES, NETFLIX_DATASET_SIZE) ;
	MF mf( NETFLIX_MOVIES , NETFLIX_USERS , 250) ; 
	
	mf.configure() ;
	mf.Learn(&buff) ;
	
	double RMSE = mf.test(&buff) ;
	cout << "RMSE is: " << RMSE ;


	return 0 ;
}