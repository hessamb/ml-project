#ifndef __NETFLIX__
#define __NETFLIX__

#include <map>
#include <cstdio>
#include <algorithm>
#include <ctime>
#include <time.h>
#include <cstdlib>
#include <fstream>

#include "interface.h"

using namespace std;

class NetflixReader: public ReadInterface{
  FILE *file;
  int movies, cur_movie;
  int cnt;
  map<int,int> mip;

  char *getCurrentFile(){
    char *s = new char[30];
    sprintf(s, "data/netflix/mv_%07d.txt", cur_movie);
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
      fprintf(stderr, "FILE %d FINISHED.\n", cur_movie);
      fclose(file);
      cur_movie++;
      if (cur_movie>movies)
        return NULL;
      file = fopen(getCurrentFile(), "r");
      fscanf(file, "%s", new char[20]);
    }

    struct tm tm = {0};
    tm.tm_year = y; tm.tm_mon = m; tm.tm_mday = d;

    time_t epoch = mktime(&tm);
    int uid = getUid(u);
    int iid = cur_movie-1;
    int t = epoch / 86400;
    return new tuple(uid, iid, r, t);
  }

  inline void jumpToFile(int movie){
    fclose(file);
    cur_movie = movie;
    file = fopen(getCurrentFile(), "r");
    fscanf(file, "%s", new char[20]);
  }

  inline virtual void reset(){
    jumpToFile(1);
  }

  inline int getUser(int u){
    if (mip.find(u) == mip.end()){
      return -1;
    }else{
      return mip[u];
    }
  }

  inline int getMovies(){
    return movies;
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
        fprintf(stderr, "ERR: There are %d < %d datapoints in the set.\n", i, dataSize);
        dataSize = i;
        break;
      }
      data[i] = cur;
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


class NetflixReaderProbeConstruct: public ReadInterface{
  ifstream file;
  NetflixReader *nr;
  int movie;

public:
  NetflixReaderProbeConstruct(int _movies): file("data/netflix/probe.txt"){
    movie = -1;
    nr = new NetflixReader(_movies);
    while( nr->nextTuple() != NULL );
    nr->reset();
  }

  inline tuple* nextTuple(){
    string line;
    if ( !getline(file, line) ){
      file.close();
      return NULL;
    }
    int len = line.length();

    if (line[len-1] == ':'){
      line.erase(len-1);
      movie = atoi(line.c_str()) -1;
      nr->jumpToFile(movie+1);
      return nextTuple();
    }else{
      int u = atoi(line.c_str());
      tuple *cur;

      while( (cur = nr->nextTuple()) != NULL ){
        int uid = nr->getUser(u);

        if (uid == cur->uid && movie == cur->iid){
//          printf("%d %d %d %d\n", cur->uid, cur->iid, cur->r, cur->t);
//          cout << cur->uid << " " << cur->iid << " " << cur->r << " " << cur->t << endl;
          return cur;
        }else if (cur->iid > movie){
          fprintf(stderr, "DATA %d,%d IN PROBE SET IS ABSENT IN DATA SET.",
            movie+1, u);
          return NULL;
        }
      }

      fprintf(stderr, "DATA %d,%d IN PROBE SET IS ABSENT IN DATA SET!",
        movie+1, u);
      return NULL;
    }
  }

  inline void reset(){
    nr->reset();
    file.seekg(0);
    movie = -1;
  }

};


class NetflixReaderNeat: public ReadInterface{
  FILE *file;

public:
  NetflixReaderNeat(string filename){
    file = fopen(filename.c_str(), "r");
  }

  inline virtual tuple* nextTuple(){
    int u, i, r, t;
    if (fscanf(file, "%d %d %d %d", &u, &i, &r, &t) == EOF)
      return NULL;
    else
      return new tuple(u, i, r, t);
  }

  inline virtual void reset(){
    fseek(file, SEEK_SET, 0);
  }

};


class NetflixReaderProbe: public NetflixReaderNeat{
public:
  NetflixReaderProbe(): NetflixReaderNeat("data/netflix/neat_probe.txt"){
  }
};

class NetflixReaderTrain: public NetflixReaderNeat{
  int dataSize, ptr;
  tuple** data;
public:
  NetflixReaderTrain(int _dataSize): NetflixReaderNeat("data/netflix/neat_train.txt"){
    srand(time(0));
    dataSize = _dataSize;

    data = new tuple*[dataSize];
    for (int i=0 ; i<dataSize ; i++){
      tuple* cur = NetflixReaderNeat::nextTuple();
      if (cur == NULL){
        fprintf(stderr, "ERR: There are %d < %d datapoints in the set.\n", i, dataSize);
        dataSize = i;
        break;
      }
      data[i] = cur;
    }
    printf("ALL DATA READ\n");
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


class NetflixReaderTrainConstruct: public ReadInterface{
  NetflixReaderProbe *nrp;
  NetflixReader *nr;
  tuple* probe;
  int movie;
  bool *mark;
  bool finalStep;

public:
  NetflixReaderTrainConstruct(int _movies){
    nrp = new NetflixReaderProbe();
    nr = new NetflixReader(_movies);

    mark = new bool[_movies];
    memset(mark, 0, _movies * sizeof(bool));

    while( nr->nextTuple() != NULL );
    nr->reset();

    probe = nrp->nextTuple();
    movie = probe->iid;
    nr->jumpToFile(movie+1);

    finalStep = false;
  }

  inline tuple* nextTuple(){
    if (finalStep){
      tuple *cur;
      do{
        cur = nr->nextTuple();
      }while(cur != NULL && mark[ cur->iid ]);
      return cur;
    }

    tuple *cur = nr->nextTuple();
    if (cur == NULL || cur->iid != movie){
      if (probe == NULL){
        nr->reset();
        finalStep = true;
        return nextTuple();
      }else{
        movie = probe->iid;
        nr->jumpToFile(movie+1);
        return nextTuple();
      }
    }else if (probe != NULL && cur->uid == probe->uid && cur->iid == probe->iid){
      mark[ probe->iid ] = true;
      probe = nrp->nextTuple();
      return nextTuple();
    }
    return cur;
  }

  inline void reset(){
    nr->reset();
    nrp->reset();

    probe = nrp->nextTuple();
    movie = probe->iid;
    nr->jumpToFile(movie+1);
  }

};





#endif
