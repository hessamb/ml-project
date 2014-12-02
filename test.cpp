#include "netflix/read.h"
#include "conf.h"
#include <iostream>
#include <ctime>
using namespace std;

int main(){
  int start = time(0);
  NetflixReaderRAM *nr = new NetflixReaderRAM(MOVIES_TOTAL, DATASET_SIZE);
  tuple *cur;

  int middle = time(0);
  while((cur = nr->nextTuple()) != NULL)
    cur->uid++;
  int end = time(0);

  cout << "READING ALL FILES TOOK " << (middle - start) << " SECONDS." << endl;
  cout << "ITERATION ON DATAPOINTS TOOK " << (end - middle) << " SECONDS." << endl;
}
