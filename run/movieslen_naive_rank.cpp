#include <iostream>
#include "../learn/SGDMF.h"
#include "../read/movieslen.h"
#include "../conf.h"

using namespace std ;

int main()
{
  int paramSize = 11;
  int rank[] = {1, 5, 10, 30, 50, 75, 100, 150, 200, 250, 300};

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

      MF mf( MOVIES_LEN_MOVIES , MOVIES_LEN_USERS , rank[i]) ;
      mf.configure() ;
      mf.Learn(mr_train) ;

      MovieslenReaderRAM *mr_test = new MovieslenReaderRAM("u" + ind + ".test",
      MOVIES_LEN_DATASET_SIZE);

      rmseTrain += mf.test(mr_train);
      rmseTest += mf.test(mr_test);
    }

    rmseTrain /= 2;
    rmseTest /= 2;

    cout << "FOR RANK = " << rank[i] << "; RMSE TRAIN = " << rmseTrain
      << " AND RMSE TEST = " << rmseTest << endl;
  }

  return 0 ;
}
