#ifndef __MOVIES_LEN__
#define __MOVIES_LEN__

#include "interface.h"

class MovieslenReader: public ReadInterface{
  FILE *file;

public:
  MovieslenReader(string filename){
    file = fopen(("data/movieslen/" + filename).c_str(), "r");
  }

  inline virtual tuple* nextTuple(){
    int u, i, r, t;
    if (fscanf(file, "%d %d %d %d", &u, &i, &r, &t) == EOF){
      return NULL;
    }
    t /= 24 * 60 * 60;
    return new tuple(u-1, i-1, r, t);
  }

  inline virtual void reset(){
    fseek(file, 0, SEEK_SET);
  }

};



class MovieslenReaderRAM: public MovieslenReader{
  int dataSize, ptr;
  tuple** data;
public:
  MovieslenReaderRAM(string filename, int _dataSize): MovieslenReader(filename){
    srand(time(0));
    dataSize = _dataSize;

    data = new tuple*[dataSize];
    for (int i=0 ; i<dataSize ; i++){
      tuple* cur = MovieslenReader::nextTuple();
      if (cur == NULL){
        fprintf(stderr, "ERR: There are %d < %d datapoints in the set.\n", i, dataSize);
        dataSize = i;
        break;
      }
      data[i] = cur;
    }
    printf("DATA LOADED INTO MAIN MEMORY\n");
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
