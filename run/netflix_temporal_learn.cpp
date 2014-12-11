#include <iostream>

#include "../test/temporal.h"
#include "../read/netflix.h"

int main(){
  setbuf(stdout, NULL);

  int start = time(0);

  NetflixReaderTrain *nr = new NetflixReaderTrain(NETFLIX_TRAIN_SIZE);

  int loaded = time(0);

  cout << "READING DATA TOOK " << (loaded - start) << " SECONDS." << endl;

  test_temporal(nr, NETFLIX_TEMPORAL_ETA, NETFLIX_TEMPORAL_LAMBDA,
  NETFLIX_USERS, NETFLIX_MOVIES, NETFLIX_TEMPORAL_NSTEP);
}
