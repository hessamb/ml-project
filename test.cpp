#include "netflix/read.h"
#include "conf.h"
#include <iostream>
#include <ctime>
using namespace std;

int main(){
  NetflixReader *nr = new NetflixReader(MOVIES_TOTAL);
  tuple *cur;

  int start = time(0);
  while((cur = nr->nextTuple()) != NULL);
  int end = time(0);

  cout << "READING ALL FILES TOOK " << (end - start) << " SECONDS." << endl;
}
