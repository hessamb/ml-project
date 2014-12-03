#ifndef __READ_INTERFACE__
#define __READ_INTERFACE__

struct tuple{
  int uid, iid, r, t;

  tuple(int _uid, int _iid, int _r, int _t){
    uid = _uid;
    iid = _iid;
    t = _t;
    r = _r;
  }
};


class ReadInterface {
public:
  virtual tuple* nextTuple() = 0;
  virtual void reset() = 0;
};

#endif
