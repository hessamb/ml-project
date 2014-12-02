#include <map>
#include <cstdio>

using namespace std;

struct tuple{
  int uid, iid, t;

  tuple(int _uid, int _iid, int _t){
    uid = _uid;
    iid = _iid;
    t = _t;
  }
};

class NetflixReader{
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
    movies = _movies;
    cur_movie = 1;
    file = fopen(getCurrentFile(), "r");
    fscanf(file, "%s", new char[20]);
  }

  tuple* nextTuple(){
    int u, i;
    char date[15];
    while (fscanf(file, "%d,%d,%s", &u, &i, date) == EOF){
      printf("FILE %d finished.\n", cur_movie);
      fclose(file);
      cur_movie++;
      if (cur_movie>movies)
        return NULL;
      file = fopen(getCurrentFile(), "r");
      fscanf(file, "%s", new char[20]);
    }
    struct tm tm;
    tm.tm_sec = tm.tm_min = tm.tm_hour = 0;

    time_t epoch = 0;
    if ( strptime(date, "%Y-%m-%d", &tm) != NULL ){
      epoch = mktime(&tm);
    }else
      fprintf(stderr, "ERR: TIME FORMAT NOT SUPPORTED!");

    int uid = getUid(u);
    int iid = i-1;
    int t = epoch;
    return new tuple(uid, iid, t);
  }

};
