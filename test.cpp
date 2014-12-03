#include "netflix/read.h"
#include "learn/temporal.h"
#include "conf.h"
#include <iostream>
#include <ctime>
using namespace std;

int main(){
  int start = time(0);
  NetflixReaderRAM *nr = new NetflixReaderRAM(NETFLIX_MOVIES, 100);//NETFLIX_DATASET_SIZE);

  TemporalDynamicsParams *params = learn_temporal(nr, NETFLIX_TEMPORAL_ETA,
    NETFLIX_TEMPORAL_LAMBDA, NETFLIX_USERS, NETFLIX_MOVIES,
    NETFLIX_TEMPORAL_NSTEP);
}
