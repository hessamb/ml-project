#ifndef __TEMPORAL__
#define __TEMPORAL__

#include <cstring>
#include <cmath>
#include "../models/linalg.h"
#include "../utils/math.h"
#include "../conf.h"

int get_bin(int t){
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

  inline double dev(int u, int t){
    return sign(t_u[u] - t) * pow( abs(t_u[u] - t), TEMPORAL_DYNAMICS_BETA );
  }

  inline double value(int u, int i, int t){
    return mu + b_u[u] + a_u[u] * dev(u,t) + b_i[i] + b_iBin[i][get_bin(t)]
      + ( q_i[i] * (p_u[u] + ap_u[u] * dev(u,t)) );
  }


  inline void update(int u, int i, double r, int t, double eta, double lmbd){
    double error = (r - value(u,i,t));

    Vector pref = p_u[u] + ap_u[u] * dev(u,t);

    mu -= eta * (-2 * error);
    b_u[u] -= eta * (-2 * error + 2 * lmbd * b_u[u]);
    b_i[i] -= eta * (-2 * error + 2 * lmbd * b_i[i]);
    b_iBin[i][get_bin(t)] -= eta * (-2 * error + 2 * lmbd * b_iBin[i][get_bin(t)]);
    a_u[u] -= eta * (-2 * error * dev(u,t) + 2 * lmbd * a_u[u]);

    p_u[u] -= (q_i[i] * error * (-2) + p_u[u] * lmbd * 2) * eta;
    ap_u[u] -= (q_i[i] * error * (-2) * dev(u,t) + p_u[u] * lmbd * 2) * eta;
    q_i[i] -= (pref * error * (-2) + q_i[i] * lmbd * 2) * eta;
  }
};

TemporalDynamicsParams* learn_temporal(NetflixReader *nr, double eta, double lambda,
  int users, int items, int nstep, TemporalDynamicsParams *params = NULL){

  if (params == NULL){
    params = new TemporalDynamicsParams(users, items, TEMPORAL_RANK);
  }

  int *rates = new int[users];
  memset(rates, 0, users*sizeof(int));
  tuple *cur;
  while( (cur = nr->nextTuple()) != NULL ){
    params->t_u[ cur->uid ] += cur->t;
    rates[ cur->uid ]++;
  }

  for (int u = 0 ; u<users ; u++){
    if (rates[u] == 0)
      params->t_u[u] = 0;
    else
      params->t_u[u] /= rates[u];
  }

  while(nstep--){
    tuple *cur;
    while( (cur = nr->nextTuple()) != NULL ){
      params->update(cur->uid, cur->iid, cur->r, cur->t, eta, lambda);
    }
    nr->reset();
  }

  return params;
}

#endif
