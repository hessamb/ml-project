#include <iostream>

#include "../read/netflix.h"
#include "../learn/temporal.h"

int main(){
  int start = time(0);
  cout << "LOADING PARAMTERS" << endl;

  TemporalDynamicsParams *params = new TemporalDynamicsParams("params/tem");
  // TemporalDynamicsParams *params = new TemporalDynamicsParams(NETFLIX_USERS,
  //   NETFLIX_MOVIES, TEMPORAL_RANK);
  int loaded = time(0);

  cout << "PARAMETERS LOADED IN " << (loaded - start) << " SECONDS." << endl;

  NetflixReaderProbe *nr = new NetflixReaderProbe(NETFLIX_MOVIES);

  cout << "COMPUTING RMSE ON PROBE SET." << endl;

  double rmse = params->rmse(nr);

  cout << "RMSE ON PROBE SET IS " << rmse << endl;
}
