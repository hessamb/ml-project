#include <iostream>
#include "../learn/SGDMF.h"
#include "../read/movieslen.h"
#include "../conf.h"
#include "../learn/Neighbour.h"
using namespace std ;

int main()
{
  double rmse = 0 ;
  int start = time(0) ;
  double train_error = 0 ;
  
  MovieslenReaderRAM *mr = new MovieslenReaderRAM("ua.base",
  MOVIES_LEN_DATASET_SIZE);
  int loaded = time(0);

  cout << "READING DATA TOOK " << (loaded - start) << " SECONDS." << endl;
  NeighborhoodModel nm( MOVIES_LEN_MOVIES , MOVIES_LEN_USERS , 75) ;
  nm.configure(MOVIES_LEN_NEIGHBOR_LAMBDA,MOVIES_LEN_NEIGHBOR_ETA , MOVIES_LEN_NEIGHBOR_NSTEP) ;
  nm.Learn(mr) ;
  MovieslenReaderRAM *mr_test = new MovieslenReaderRAM("ua.test",
      MOVIES_LEN_DATASET_SIZE);
  rmse = nm.test(mr_test);
  train_error = nm.test(mr) ;
  
  mr = new MovieslenReaderRAM("ub.base",
  MOVIES_LEN_DATASET_SIZE);
  mr_test = new MovieslenReaderRAM("ub.test",
      MOVIES_LEN_DATASET_SIZE);
  NeighborhoodModel nm2( MOVIES_LEN_MOVIES , MOVIES_LEN_USERS , 75) ;
  nm2.configure(MOVIES_LEN_NEIGHBOR_LAMBDA,MOVIES_LEN_NEIGHBOR_ETA , MOVIES_LEN_NEIGHBOR_NSTEP) ;
  nm2.Learn(mr) ;
  rmse += nm2.test(mr_test);
  train_error += nm2.test(mr) ;
      
  
    
  cout << " RMSE = " << rmse/2 << "and Train Error is :" << train_error/2 << endl;

  return 0 ;
}
