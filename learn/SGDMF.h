#ifndef __SGDMF__
#define __SGDMF__

#include <iostream>
#include "../Models/linalg.h"
#include "../read/interface.h"
#include "../conf.h"
#include <ctime>

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

  void configure (double lambda = NETFLIX_BASIC_LAMBDA, double alpha = NETFLIX_BASIC_ETA , double nstep = NETFLIX_BASIC_NSTEP)
  {
    LAMBDA = lambda ; ALPHA = alpha ; NSTEP = nstep  ;
  }

  double test (ReadInterface * buff)
  {
    double RMSE = 0 ;
    tuple* rating ;
    double predict = 0 ;
    int counter = 0 ;
    while(( rating = buff->nextTuple() ) != NULL)
      {
      predict = ave_rate + BU[rating->uid] + BI[rating->iid] + P[rating->uid] * Q[rating->iid] ;
      RMSE += pow(rating->r - predict,2);
      counter ++ ;
    }
    RMSE /= counter ;
    RMSE = sqrt(RMSE) ;
  buff->reset();
    return RMSE ;
  }

  void Learn (ReadInterface* buff)
  {
  int ptime = time(0) ;
    for (int step = 0 ; step<NSTEP ; step++)
    {
      cout << "iteration :" << step << endl ;
      cout << "time : " << time(0) -ptime << endl ;
  ptime = time(0) ;
  tuple* rating ;
      while(( rating = buff->nextTuple() ) != NULL)
      {
    int f =  P.n ;
    double eij = rating->r - BU[rating->uid] - BI[rating->iid] - ave_rate;
    for (int i=0 ; i<f ; i++)
      eij -=   P[rating->uid][i] * Q[rating->iid][i]  ;

        ave_rate = ave_rate + ALPHA * (2 *  eij ) ;

        BU[rating->uid] +=  ALPHA * ( 2 *  eij - BU[rating->uid] * LAMBDA   );
        BI[rating->iid] +=  ALPHA * ( 2 *  eij - BI[rating->iid] * LAMBDA   );

    for (int i=0 ; i<f ; i++ )
    {
      P[rating->uid][i] += ( Q[rating->iid][i] * (2 * eij) - P[rating->uid][i] * LAMBDA ) * ALPHA ;
      Q[rating->iid][i] += ( P[rating->uid][i] * (2 * eij) - Q[rating->iid][i] * LAMBDA ) * ALPHA ;
    }
    }
      buff->reset() ;
    }

  save("alaki");

  }
  void save(string foldername)
  {
  cerr << "salam" <<endl ;
  string pname = foldername + "/p";
  string qname = foldername + "/q" ;
  string ubias = foldername + "/bu" ;
  string ibias = foldername + "/bi" ;
  string imu = foldername + "/mu" ;

  FILE *pfile ;
  pfile = fopen(imu.c_str(),"w+") ;
  fprintf(pfile,"%lf",ave_rate) ;
  fclose(pfile) ;

  P.save(pname);
  cerr << "salam1" <<endl ;
  Q.save(qname);
  cerr << "salam2" <<endl ;
  BU.save(ubias);
  cerr << "salam3" <<endl ;
  BI.save(ibias);
  cerr << "salam4" <<endl ;
  }

  // void load(string foldername)
  // {
  // string pname = foldername + "/p";
  // string qname = foldername + "/q" ;
  // string ubias = foldername + "/bu" ;
  // string ibias = foldername + "/bi" ;
  //
  // P.load(pname);
  // Q.load(qname);
  // BU.load(ubias);
  // BI.load(ibias);
  // }

};


#endif
