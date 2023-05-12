#include <pthread.h>
#include "vector"
#include "queue"
#include "string"
#include "pthread_barrier_mac.h"
#include "map"

using namespace std;

struct MapThreadFunctionArguments {
    map<int, vector<int> > *threadMap;
    int numberOfPowers;
    queue<string> *filesQueue;
    pthread_mutex_t *mapMutex;
    pthread_barrier_t *barrier;
};
