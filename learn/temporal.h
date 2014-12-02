
class TemporalDynamicsParams{
  self.cnt = 0
  self.mu = 0
  self.b_u = np.zeros((users, 1))
  self.b_i = np.zeros((items, 1))
  self.b_iBin = np.zeros((items, conf.TEMPORAL_DYNAMICS.BINS_COUNT))
  self.q_i = np.zeros((items, f))
  self.p_u = np.zeros((users, f))
  self.ap_u = np.zeros((users, f))
  self.a_u = np.zeros((users, 1))
  self.t_u = np.zeros((users, 1))

  double mu;
  Vector b_u;
  Vector b_i;
  Matrix b_iBin;
  Matrix q_i;
  Matrix p_u;
  Matrix ap_u;
  Vector a_u;
  Vector t_u;


public:
  TemporalDynamicsParams(int users, int items, int f): mu(0), b_u(users),
    b_i(items), b_iBin(items, TEMPORAL_BINS_COUNT), q_i(items, f),
    p_u(users, f), ap_u(users, f), a_u(users), t_u(users) {
  }
  
};
