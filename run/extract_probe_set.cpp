#include <iostream>

#include "../read/netflix.h"
#include "../conf.h"

int main(){
  NetflixReaderProbeConstruct *nr = new NetflixReaderProbeConstruct(NETFLIX_MOVIES);
  cout << "DATA LOADED" << endl;

  tuple *cur;
  while( (cur = nr->nextTuple()) != NULL){
    printf("%d %d %d %d\n", cur->uid, cur->iid, cur->r, cur->t);
  }

  return 0;
}
