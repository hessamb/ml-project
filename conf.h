#ifndef __CONF__
#define __CONF__


//const int NETFLIX_MOVIES = 17770;
const int NETFLIX_MOVIES = 10000;
const int NETFLIX_USERS = 480189;
const int NETFLIX_DATASET_SIZE = 100480507;

const double NETFLIX_BASIC_ETA = 0.002 ;
const double NETFLIX_BASIC_LAMBDA = 0.02 ;
const double NETFLIX_BASIC_NSTEP = 20 ;
const double NETFLIX_BASIC_NUMOFFEATURES = 250 ;

const double NETFLIX_TEMPORAL_ETA = 0.01;
const double NETFLIX_TEMPORAL_LAMBDA = 0.01;
const double NETFLIX_TEMPORAL_NSTEP = 20;


const int TEMPORAL_BINS_COUNT = 1;
const int TEMPORAL_RANK = 250;
const double TEMPORAL_DYNAMICS_BETA = 0.4;

#endif
