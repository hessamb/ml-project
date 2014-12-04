#ifndef __TEMPORAL__
#define __TEMPORAL__

#include <cstring>
#include <cmath>
#include <iostream>
#include "../models/linalg.h"
#include "../utils/math.h"
#include "../conf.h"
#include "../read/netflix.h"

inline int get_bin(int t){
  return 0;
}

class TemporalDynamicsParams{

public:
  double mu;
  Vector b_u;
  Vector b_i;
  Matrix b_iBin;
  Matrix q_i;
  Matrix p_u;
  Matrix ap_u;
  Vector a_u;
  Vector t_u;

  TemporalDynamicsParams(int users, int items, int f): mu(0), b_u(users),
    b_i(items), b_iBin(items, TEMPORAL_BINS_COUNT), q_i(items, f),
    p_u(users, f), ap_u(users, f), a_u(users), t_u(users) {
  }
  TemporalDynamicsParams(string directory): b_u(directory + "/b_u"),
    b_i(directory + "/b_i"), b_iBin(directory + "/b_iBin"),
    q_i(directory + "/q_i"), p_u(directory + "/p_u"),
    ap_u(directory + "/ap_u"), a_u(directory + "/a_u"),
    t_u(directory + "/t_u"){

    FILE * pFile ;
    pFile = fopen ( (directory + "/mu").c_str(),"r");
    fscanf (pFile, "%lf\n " , &mu);
    fclose(pFile);
  }

  inline double dev(int u, int t){
    return sign(t_u[u] - t) * pow( abs(t_u[u] - t), TEMPORAL_DYNAMICS_BETA );
  }

  inline double value(int u, int i, int t){
    double dv = dev(u,t);
    double res = 0;
    int f = p_u[u].n;

    for (int k=0 ; k<f ; k++)
      res += ( q_i[i][k] * (p_u[u][k] + ap_u[u][k] * dv) );
    res += mu + b_u[u] + a_u[u] * dv + b_i[i] + b_iBin[i][get_bin(t)];

    return res;
  }

  inline void update(int u, int i, double r, int t, double eta, double lmbd){
    double error = (r - value(u,i,t));
    double dv = dev(u,t);
    int f = p_u[u].n;

    mu -= eta * (-2 * error);
    b_u[u] -= eta * (-2 * error + 2 * lmbd * b_u[u]);
    b_i[i] -= eta * (-2 * error + 2 * lmbd * b_i[i]);
    b_iBin[i][get_bin(t)] -= eta * (-2 * error + 2 * lmbd * b_iBin[i][get_bin(t)]);
    a_u[u] -= eta * (-2 * error * dv + 2 * lmbd * a_u[u]);


    for (int k=0 ; k<f ; k++){
      p_u[u][k] -= (q_i[i][k] * error * (-2) + p_u[u][k] * lmbd * 2) * eta;
      ap_u[u][k] -= (q_i[i][k] * error * (-2) * dv + p_u[u][k] * lmbd * 2) * eta;
      q_i[i][k] -= ( (p_u[u][k] + ap_u[u][k] * dv) * error * (-2) + q_i[i][k] * lmbd * 2) * eta;
    }

  }

  inline double rmse(ReadInterface *ri){
    tuple *cur;

    double res = 0;
    int cnt = 0;
    ri->reset();
    while( (cur = ri->nextTuple()) != NULL ){
      double error = cur->r - value(cur->uid, cur->iid, cur->t);
      res += error * error;
      cnt++;
    }
    if (cnt == 0)
      return 0;
    else{
      res /= cnt;
      return sqrt(res);
    }
  }

  inline void save(string directory){
    FILE * pFile ;
    pFile = fopen ( (directory + "/mu").c_str(),"w");
    fprintf (pFile, "%lf\n " , mu);
    fclose(pFile);

    b_u.save(directory + "/b_u");
    b_i.save(directory + "/b_i");
    b_iBin.save(directory + "/b_iBin");
    q_i.save(directory + "/q_i");
    p_u.save(directory + "/p_u");
    ap_u.save(directory + "/ap_u");
    a_u.save(directory + "/a_u");
    t_u.save(directory + "/t_u");
  }
};

TemporalDynamicsParams* learn_temporal(ReadInterface *ri, double eta, double lambda,
  int users, int items, int nstep, TemporalDynamicsParams *params = NULL){

  if (params == NULL){
    params = new TemporalDynamicsParams(users, items, TEMPORAL_RANK);
  }
  cout << "PARAMS INITIALIZED" << endl;

  int *rates = new int[users];
  memset(rates, 0, users*sizeof(int));
  tuple *cur;

  cout << "COMPUTING AVERAGE TIMES" << endl;
  ri->reset();
  while( (cur = ri->nextTuple()) != NULL ){
    params->t_u[ cur->uid ] += cur->t;
    rates[ cur->uid ]++;
  }

  for (int u = 0 ; u<users ; u++){
    if (rates[u] == 0)
      params->t_u[u] = 0;
    else
      params->t_u[u] /= rates[u];
  }

  cout << "AVERAGE TIME COMPUTED" << endl;


  cout << "STARTING TO LEARN" << endl;
  while(nstep--){
    cout << "THERE ARE " << (nstep+1) << " MORE ITERATIONS." << endl;
    int start = time(0);
    tuple *cur;
    ri->reset();
    while( (cur = ri->nextTuple()) != NULL ){
      params->update(cur->uid, cur->iid, cur->r, cur->t, eta, lambda);
    }
    int end = time(0);
    cout << "ITERATION DONE IN " << (end - start) << " SECONDS." << endl;
  }

  return params;
}

#endif
