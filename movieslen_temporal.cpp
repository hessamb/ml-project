#include <iostream>

#include "test/temporal.h"
#include "read/movieslen.h"

int main(){
  int paramSize = 7;
  double lambda[] = {0, 0.001, 0.003, 0.01, 0.03, 0.1, 0.3};
  double beta[] = {0, 0.001, 0.003, 0.01, 0.03, 0.1, 0.3};

  double mini = 1e9;
  double best_lambda = 0;
  double best_beta = 0;

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

        TEMPORAL_DYNAMICS_BETA = beta[j];
        TemporalDynamicsParams *params = test_temporal(mr, MOVIES_LEN_TEMPORAL_ETA,
          lambda[i], MOVIES_LEN_USERS, MOVIES_LEN_MOVIES,
          MOVIES_LEN_TEMPORAL_NSTEP, false);

        MovieslenReaderRAM *mr_test = new MovieslenReaderRAM("u" + ind + ".test",
          MOVIES_LEN_DATASET_SIZE);
        rmse += params->rmse(mr_test);
      }

      rmse /= 5;
      if (rmse < mini){
        mini = rmse;
        best_lambda = lambda[i];
        best_beta = beta[j];
      }

      cout << "FOR LAMBDA = " << lambda[i] << " AND BETA = " << beta[j]
        << "; RMSE = " << rmse << "." << endl;

    }
  }

  cout << "BEST LAMBDA = " << best_lambda << ", BEST BETA = " << best_beta
    << " BEST RMSE " << mini << endl;

}
