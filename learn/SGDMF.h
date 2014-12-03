#ifndef __SGDMF__
#define __SGDMF__

#include <iostream>
#include "../Models/linalg.h"
#include "../netflix/read.h"
#include "../conf.h"
using namespace std ;

class MF
{
public :
  // m = number of movies
  // n = number of users
  // k = number of features
  double LAMBDA , ALPHA ,NSTEP ;
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

  void configure (double lambda = NETFLIX_BASIC_LAMBDA, double alpha = NETFLIX_BASIC_ETA , double nstep = NETFLIX_BASIC_NUMOFFEATURES)
  {
    LAMBDA = lambda ; ALPHA = alpha ; NSTEP = nstep  ;
  }

  double test (NetflixReader buff)
  {
	double RMSE = 0 ;
	tuple* rating ;
	double predict = 0 ;
	int counter = 0 ;
	while(( rating = buff.nextTuple() ) != NULL)
    {
		predict = ave_rate + BU[rating->uid] + BI[rating->iid] + P[rating->uid] * Q[rating->iid] ;
		RMSE += pow(rating->r - predict,2);
		counter ++ ;
	}
	RMSE /= counter ;
	RMSE = sqrt(RMSE) ;
	
	return RMSE ; 
    
  }
  void Learn (NetflixReader buff)
  {
    for (int step = 0 ; step<NSTEP ; step++)
    {
      cout << "iteration :" << step << endl ;
      tuple* rating ;
      while(( rating = buff.nextTuple() ) != NULL)
      {
        double eij = rating->r - P[rating->uid] * Q[rating->iid] - BU[rating->uid] - BI[rating->iid] - ave_rate ;

        ave_rate = ave_rate + ALPHA * (2 *  eij ) ;

        BU[rating->uid] +=  ALPHA * ( 2 *  eij - BU[rating->uid] * LAMBDA   );
        BI[rating->iid] +=  ALPHA * ( 2 *  eij - BI[rating->uid] * LAMBDA   );

        P[rating->uid] += ( Q[rating->iid] * (2 * eij) - (P[rating->uid] * LAMBDA) ) * ALPHA ;
        Q[rating->iid] += ( P[rating->uid] * (2 * eij) - Q[rating->iid] * LAMBDA ) * ALPHA ;
      }
      buff.reset() ;
    }

  }
  void save(string foldername)
  {
    string pname = foldername + "/p";
	string qname = foldername + "/q" ;
	string ubias = foldername + "/bu" ;
	string ibias = foldername + "/bi" ;
	
	P.save(pname);
	Q.save(qname);
	BU.save(ubias);
	BI.save(ibias);
  }
  
  void load(string foldername)
  {
	string pname = foldername + "/p";
	string qname = foldername + "/q" ;
	string ubias = foldername + "/bu" ;
	string ibias = foldername + "/bi" ;
	
	P.load(pname);
	Q.load(qname);
	BU.load(ubias);
	BI.load(ibias);
  }

};


#endif
