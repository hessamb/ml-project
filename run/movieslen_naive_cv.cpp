#include <iostream>
#include "../learn/SGDMF.h"
#include "../read/movieslen.h"
#include "../conf.h"

using namespace std ;

int main()
{
  int paramSize = 7;
  double lambda[] = {0, 0.001, 0.003, 0.01, 0.03, 0.1, 0.3};
  double eta[] = {0.0001, 0.0003, 0.001, 0.003, 0.01, 0.03, 0.1};

  double mini = 1e9;
  double best_lambda = 0, best_eta = 0;

  for (int i=0 ; i<paramSize ; i++){
    for (int j=0 ; j<paramSize ; j++){
      string ind = "0";
      double rmse = 0;
      for (int k=1 ; k<=5 ; k++){
        ind[0] = k + '0';

        int start = time(0);
        MovieslenReaderRAM *mr = new MovieslenReaderRAM("u" + ind + ".base",
        MOVIES_LEN_DATASET_SIZE);
        int loaded = time(0);

        cout << "READING DATA TOOK " << (loaded - start) << " SECONDS." << endl;


        BASIC_LAMBDA = lambda[i];
        BASIC_ETA = eta[j];

        MF mf( MOVIES_LEN_MOVIES , MOVIES_LEN_USERS , 100) ;
        mf.configure() ;
        mf.Learn(mr) ;

        MovieslenReaderRAM *mr_test = new MovieslenReaderRAM("u" + ind + ".test",
        MOVIES_LEN_DATASET_SIZE);

        rmse += mf.test(mr_test);
      }

      rmse /= 5;
      if (rmse < mini){
        mini = rmse;
        best_lambda = lambda[i];
        best_eta = eta[j];
      }

      cout << "FOR LAMBDA = " << lambda[i] << " AND ETA = " << eta[j]
           << "; RMSE = " << rmse << "." << endl;
    }
  }

  cout << "BEST LAMBDA = " << best_lambda << 	" AND BEST_ETA = " << best_eta
       << " AND BEST RMSE " << mini << endl;


  return 0 ;
}
