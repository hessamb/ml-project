#ifndef __CONF__
#define __CONF__


const int NETFLIX_MOVIES = 17770;
const int NETFLIX_USERS = 480189;
const int NETFLIX_DATASET_SIZE = 100480507;
const int NETFLIX_TRAIN_SIZE = 99072112;

double BASIC_ETA = 0.001 ;
double BASIC_LAMBDA = 0.03 ;
const double BASIC_NSTEP = 50 ;

double NETFLIX_TEMPORAL_ETA = 0.001;
double NETFLIX_TEMPORAL_LAMBDA = 0.01;
const double NETFLIX_TEMPORAL_NSTEP = 20;


const int MOVIES_LEN_MOVIES = 1682;
const int MOVIES_LEN_USERS = 943;
const int MOVIES_LEN_DATASET_SIZE = 100000;

double MOVIES_LEN_TEMPORAL_ETA = 0.001;
double MOVIES_LEN_TEMPORAL_LAMBDA = 0.1;
const double MOVIES_LEN_TEMPORAL_NSTEP = 30;


const int TEMPORAL_BINS_COUNT = 30;
int TEMPORAL_RANK = 100;
double TEMPORAL_DYNAMICS_BETA = 0.01;


#endif
