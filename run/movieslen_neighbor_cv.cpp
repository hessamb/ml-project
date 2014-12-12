#include <iostream>
#include "../learn/SGDMF.h"
#include "../read/movieslen.h"
#include "../conf.h"
#include "../learn/Neighbour.h"

using namespace std ;

int main()
{
  int lambdaSize = 8;
  int etasize = 8 ;
  double lambda[] = {0.001, 0.003, 0.006,0.01, 0.03, 0.06,0.1, 0.3};
  double eta[] = {0.001,0.003,0.005, 0.008,0.02,0.05,0.08,0.1} ;
  double mini = 1e9;
  double best_lambda = 0;
  double best_eta =  0 ;
        
  for (int i=0 ; i<lambdaSize ; i++){
    for (int j=0 ; j<etasize ; j++){
      string ind = "0";
      double rmse = 0;
      for (int k=1 ; k<=5 ; k++){
        ind[0] = k + '0';
        int start = time(0);
        MovieslenReaderRAM *mr = new MovieslenReaderRAM("u" + ind + ".base",
        MOVIES_LEN_DATASET_SIZE);
        int loaded = time(0);
        cout << "READING DATA TOOK " << (loaded - start) << " SECONDS." << endl;
        NeighborhoodModel nm( MOVIES_LEN_MOVIES , MOVIES_LEN_USERS , 75) ;
  
        nm.configure(lambda[i],eta[j] , MOVIES_LEN_NEIGHBOR_NSTEP) ;
        nm.Learn(mr) ;

        MovieslenReaderRAM *mr_test = new MovieslenReaderRAM("u" + ind + ".test",
        MOVIES_LEN_DATASET_SIZE);
        rmse += nm.test(mr_test);
      }
      rmse /= 5;
      if (rmse < mini){
        mini = rmse;
        best_lambda = lambda[i];
        best_eta = eta[j] ; 
      }
      cout << "FOR LAMBDA = " << lambda[i] << "and ETA = " <<eta[j] << " RMSE = " << rmse << "." << endl;

    }
  }  

  cout << "BEST LAMBDA = " << best_lambda <<"BEST eta = " << best_eta << " BEST RMSE " << mini << endl;


  return 0 ;
}
