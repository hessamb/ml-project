#include <iostream>

#include "../read/netflix.h"
#include "../conf.h"

int main(){
  NetflixReaderProbeConstruct *nr = new NetflixReaderProbeConstruct(NETFLIX_MOVIES);
  cout << "DATA LOADED" << endl;

  FILE *file = fopen("data/netflix/neat_probe.txt", "w");

  tuple *cur;
  while( (cur = nr->nextTuple()) != NULL){
    fprintf(file, "%d %d %d %d\n", cur->uid, cur->iid, cur->r, cur->t);
//    printf("%d %d %d %d\n", cur->uid, cur->iid, cur->r, cur->t);
  }

  fclose(file);
  return 0;
}
