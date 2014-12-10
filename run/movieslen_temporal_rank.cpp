#include <iostream>
#include "../learn/temporal.h"
#include "../read/movieslen.h"
#include "../conf.h"

using namespace std ;

int main()
{
  int paramSize = 10;
  double rank[] = {5, 10, 30, 50, 75, 100, 150, 200, 250, 300};

  for (int i=0 ; i<paramSize ; i++){
    double rmseTest = 0, rmseTrain = 0;
    string ind = "0";

    for (char ch='a' ; ch<='b' ; ch++){
      ind[0] = ch;
      int start = time(0);
      MovieslenReaderRAM *mr_train = new MovieslenReaderRAM("u" + ind + ".base",
      MOVIES_LEN_DATASET_SIZE);
      int loaded = time(0);

      cout << "READING DATA TOOK " << (loaded - start) << " SECONDS." << endl;

      TEMPORAL_RANK = rank[i];
      TemporalDynamicsParams *params = learn_temporal(mr_train,
      MOVIES_LEN_TEMPORAL_ETA, MOVIES_LEN_TEMPORAL_LAMBDA, MOVIES_LEN_USERS,
      MOVIES_LEN_MOVIES, MOVIES_LEN_TEMPORAL_NSTEP);

      MovieslenReaderRAM *mr_test = new MovieslenReaderRAM("u" + ind + ".test",
      MOVIES_LEN_DATASET_SIZE);

      rmseTrain += params->rmse(mr_train);
      rmseTest += params->rmse(mr_test);
    }

    rmseTrain /= 2;
    rmseTest /= 2;

    cout << "FOR RANK = " << rank[i] << "; RMSE TRAIN = " << rmseTrain
    << " AND RMSE TEST = " << rmseTest << endl;
  }

  return 0 ;
}
