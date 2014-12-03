#ifndef __NETFLIX__
#define __NETFLIX__

#include <map>
#include <cstdio>
#include <algorithm>
#include <ctime>
#include <time.h>
#include <cstdlib>
#include "interface.h"

using namespace std;

class NetflixReader: public ReadInterface{
  FILE *file;
  int movies, cur_movie;
  int cnt;
  map<int,int> mip;

  char *getCurrentFile(){
    char *s = new char[30];
    sprintf(s, "data/mv_%07d.txt", cur_movie);
    return s;
  }

  int getUid(int u){
    if (mip.find(u) == mip.end()){
      mip[u] = cnt;
      return cnt++;
    }else{
      return mip[u];
    }
  }

public:
  NetflixReader(int _movies){
    cnt = 0;
    movies = _movies;
    cur_movie = 1;
    file = fopen(getCurrentFile(), "r");
    fscanf(file, "%s", new char[20]);
  }

  inline virtual tuple* nextTuple(){
    int u, r, y, m, d;
    while (fscanf(file, "%d,%d,%d-%d-%d", &u, &r, &y, &m, &d) == EOF){
      printf("FILE %d FINISHED.\n", cur_movie);
      fclose(file);
      cur_movie++;
      if (cur_movie>movies)
        return NULL;
      file = fopen(getCurrentFile(), "r");
      fscanf(file, "%s", new char[20]);
    }
	
	//printf("%d,%d,%d-%d-%d\n", u, r, y, m, d);
    struct tm tm = {0};
    tm.tm_year = y; tm.tm_mon = m; tm.tm_mday = d;
	
    time_t epoch = mktime(&tm);
	//printf("%d\n", epoch);
    int uid = getUid(u);
    int iid = cur_movie-1;
    int t = epoch / 86400;
    return new tuple(uid, iid, r, t);
  }

  inline virtual void reset(){
    cur_movie = 1;
    file = fopen(getCurrentFile(), "r");
    fscanf(file, "%s", new char[20]);
  }

};


class NetflixReaderRAM: public NetflixReader{
  int dataSize, ptr;
  tuple** data;
public:
  NetflixReaderRAM(int _movies, int _dataSize): NetflixReader(_movies){
    srand(time(0));
    dataSize = _dataSize;

    data = new tuple*[dataSize];
    for (int i=0 ; i<dataSize ; i++){
	  tuple* cur = NetflixReader::nextTuple();
      if (cur == NULL){
        fprintf(stderr, "ERR: There are not %d datapoints in the set.\n", dataSize);
      }
      data[i] = cur;
	  //printf("%d  %d \n",cur->r, cur->t) ;
    }
    printf("ALL FILES READ\n");
    ptr = 0;
    random_shuffle(data, data+dataSize);
    printf("RANDOM SHUFFLED\n");
  }

  inline tuple* nextTuple(){
    if (ptr == dataSize)
      return NULL;
    else
      return data[ptr++];
  }

  inline void reset(){
    ptr = 0;
  }

  inline void shuffle(){
    random_shuffle(data, data+dataSize);
  }

};


#endif
