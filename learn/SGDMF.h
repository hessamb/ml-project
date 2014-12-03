#include <iostream>
#include "../Models/linalg.h"
#include "../netflix/read.h"
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
	
	void configure (double lambda, double alpha , double nstep )
	{
		LAMBDA = lambda ; ALPHA = alpha ; NSTEP = nstep  ;
	}
	
	void Learn (NetflixReader buff)
	{
		for (int step = 0 ; i<NSTEP ; i++)
		{
			cout << "iteration :" << step << endl ;
			tuple rating ;
			while(( rating = buff.nexttuple() ) != NULL)
			{
				eij = rating.r - P[rating.uid] * Q[rating.iid] - BU[rating.uid] - BI[rating.iid] - ave_rate ;
				
				ave_rate = ave_rate + ALPHA * (2 *  eij ) ;
				
				BU[rating.uid] +=  ALPHA * ( 2 *  eij - BU[rating.uid] * LAMBDA   ); 
                BI[rating.iid] +=  ALPHA * ( 2 *  eij - BI[rating.uid] * LAMBDA   );
                
				P[rating.uid] += ( Q[rating.iid] * (2 * eij) - P[rating.uid] * LAMBDA ) * ALPHA ;
                Q[rating.iid] += ( P[rating.uid] * (2 * eij) - Q[rating.iid] * LAMBDA ) * ALPHA ;
			}
			buff.reset() ;
		}
			
	}
	
};
