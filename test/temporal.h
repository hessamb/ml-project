#include <iostream>
#include <ctime>

#include "../read/interface.h"
#include "../learn/temporal.h"
#include "../conf.h"

using namespace std;

TemporalDynamicsParams* test_temporal(ReadInterface *ri, double eta,
  double lambda, int users, int items, int nstep, bool save = true){
  int start = time(0);

  cout << "LEARNING MODEL" << endl;
  TemporalDynamicsParams *params = learn_temporal(ri, eta, lambda, users,
    items, nstep);
  cout << "MODEL LEARNED SUCCESSFULLY" << endl;

  int learned = time(0);


  cout << "LEARNING TOOK " << (learned - start) << " SECONDS." << endl;

  cout << "COMPUTING TRAINING RMSE" << endl;
  cout << "TRAINING RMSE = " << params->rmse(ri) << endl;

  int measured = time(0);

  if (save){
    cout << "SAVING PARAMETERS" << endl;
    params->save("params/temporal");
    cout << "PARAMETERS SAVED" << endl;

    int saved = time(0);
    cout << "SAVING TOOK " << (saved - measured) << " SECONDS." << endl;
  }

  int finish = time(0);
  cout << "THE WHOLE PROCESS TOOK " << (finish - start) << " SECONDS." << endl;

  return params;
}
