#ifndef __MOVIES_LEN__
#define __MOVIES_LEN__

class MovieslenReader: public ReadInterface{
  FILE *file;

public:
  MoviesReader(string filePath){
    file = fopen(fileName.c_str(), "r");
  }

  inline virtual tuple* nextTuple(){
    int u, i, r, t;
    if (fscanf(file, "%d %d %d %d", &u, &i, &r, &t) == EOF){
      return NULL;
    }
    t /= 24 * 60 * 60;
    return new tuple(u, i, r, t);
  }

  inline virtual void reset(){
    fseek(file, 0, SEEK_SET);
  }

};



class MovieslenReaderRAM: public MovieslenReader{
  int dataSize, ptr;
  tuple** data;
public:
  MovieslenReaderRAM(string filePath, int _dataSize): MovieslenReader(filePath){
    srand(time(0));
    dataSize = _dataSize;

    data = new tuple*[dataSize];
    for (int i=0 ; i<dataSize ; i++){
      tuple* cur = MovieslenReader::nextTuple();
      if (cur == NULL){
        fprintf(stderr, "ERR: There are not %d datapoints in the set.\n", dataSize);
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
