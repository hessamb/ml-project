#include <iostream>
#include "../Models/LinAlg.h"

using namespace std ;

class MF
{
public :
	// m = number of movies 
	// n = number of users
	// k = number of features
	double LAMBDA , ALPHA ,NSTEP 
	int m,n,k;
	Matrix P,Q ; 
	Vector BU,BI ;
	double ave_rate ;
	
	MF(int mm = 0 ,int nn =0 ,int kk = 0): P(nn,kk), Q(mm,kk),BU(nn),BI(mm)
	{
		m = mm ; n = nn ; k = kk ;
		LAMBDA =0 ; ALPHA = 0 ; NSTEP = 0 ;
		ave_rate = 0 ;
	}
	
	void configure (double lambda, double alpha , double nstep , double k)
	{
		LAMBDA = lambda ; ALPHA = alpha ; NSTEP = nstep ; K = k ;
	}
	
	
	
};
