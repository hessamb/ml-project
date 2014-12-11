#ifndef __SGDMF__
#define __SGDMF__

#include <iostream>
#include "../Models/linalg.h"
#include "../read/interface.h"
#include "../conf.h"
#include <ctime>

using namespace std ;

class NeighborhoodModel
{
public :
  // m = number of movies
  // n = number of users
  // k = number of features
  double LAMBDA , ALPHA ,NSTEP ;
  int m,n,k;
  Matrix weight,similarity,Rating;
  Vector BU,BI ;
  double ave_rate ;
  
  
  NeighborhoodModel(int mm = 0 ,int nn =0 ,int kk = 0): weight(mm,mm), similarity(mm,mm),Rating(nn,mm),BU(nn),BI(mm)
  {
    m = mm ; n = nn ; k = kk ;
    LAMBDA =0 ; ALPHA = 0 ; NSTEP = 0 ;
    ave_rate = 0 ;
  }
  void ComputerSimilarity (int i , int j)
  {
	
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
      predict = ave_rate + BU[rating->uid] + BI[rating->iid] ;
      double temp ; 
      int cnt = 0 ;
      for ( int j=0 ; j<m ; j++ ) 
      {
        if(Rating[rating->uid][j] != 0 )
        {
            temp += ( Rating[rating->uid][j] - BI[j] - BU[rating->uid] - ave_rate )*weight[rating->iid][j] ;
            cnt ++ ;
        }
      }
      temp /= sqrt(cnt) ; 
      predict += temp ; 
      if (predict > 5)
        predict = 5;
      else if (predict < 1)
        predict = 1;
      RMSE += pow(rating->r - predict,2);
      counter ++ ;
    }
    RMSE /= counter ;
    RMSE = sqrt(RMSE) ;
    buff->reset();
    return RMSE ;
  }
  void MakeRatingMatrix (ReadInterface* buff)
  {
    while(( rating = buff->nextTuple() ) != NULL)
    {
      Rating[rating->uid][rating->iid] = rating->r ;
    }
    buff->reset() ;
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
        double errorbias = rating->r - BU[rating->uid] - BI[rating->iid] -ave_rate ;
        double eij = rating->r - BU[rating->uid] - BI[rating->iid] - ave_rate ;
        double temp = 0 ;
        int counter = 0 ; 
        for (int j=0 ; j<m ; j++)
          if(Rating[rating->uid][j] != 0 )
          {
            counter ++ ;
            temp += ( Rating[rating->uid][j] - BI[j] - BU[rating->uid] - ave_rate )*weight[rating->iid][j] ;
          }
        temp /= sqrt(counter) ;
        eij += temp ; 
      
        ave_rate = ave_rate + ALPHA * (2 *  eij ) ;

        BU[rating->uid] +=  ALPHA * ( 2 *  eij - BU[rating->uid] * LAMBDA   );
        BI[rating->iid] +=  ALPHA * ( 2 *  eij - BI[rating->iid] * LAMBDA   );

        for (int j=0 ; j<m ; j++ )
        {
          if(Rating[rating->uid][j] != 0 ){
            weight[rating->iid][j] += ALPHA * ( (1/sqrt(counter))* eij * ( Rating[rating->uid][j]- BU[rating->uid] - BI[j] -ave_rate) 
            - LAMBDA*weight[rating->iid][j] );
          }
        }
      }
      buff->reset() ;
    }

//  save("alaki");

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
