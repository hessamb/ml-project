#include "read/netflix.h"
#include "learn/temporal.h"
#include "conf.h"
#include <iostream>
#include <ctime>
using namespace std;

int main(){
  int start = time(0);

  NetflixReaderRAM *nr = new NetflixReaderRAM(NETFLIX_MOVIES, NETFLIX_DATASET_SIZE);

  int loaded = time(0);

  cout << "READING DATA TOOK " << (loaded - start) << " SECONDS." << endl;

  cout << "LEARNING MODEL" << endl;
  TemporalDynamicsParams *params = learn_temporal(nr, NETFLIX_TEMPORAL_ETA,
    NETFLIX_TEMPORAL_LAMBDA, NETFLIX_USERS, NETFLIX_MOVIES,
    NETFLIX_TEMPORAL_NSTEP);
  cout << "MODEL LEARNED SUCCESSFULLY" << endl;

  int learned = time(0);


  cout << "LEARNING TOOK " << (learned - loaded) << " SECONDS." << endl;

  cout << "COMPUTING TRAINING RMSE" << endl;
  cout << "TRAINING RMSE = " << params->rmse(nr) << endl;

  int measured = time(0);

  cout << "SAVING PARAMETERS" << endl;
  params->save("params/temporal");
  cout << "PARAMETERS SAVED" << endl;

  int saved = time(0);

  cout << "SAVING TOOK " << (saved - measured) << " SECONDS." << endl;
  cout << "THE WHOLE PROCESS TOOK " << (saved - start) << " SECONDS." << endl;
}
