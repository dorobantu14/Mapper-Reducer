#include <pthread.h>
#include "vector"
#include "queue"
#include "string"
#include "pthread_barrier_mac.h"
#include "map"

using namespace std;

struct ReduceThreadFunctionArguments {
    vector<map<int, vector<int> > > *mapList;
    map<int, vector<int> > *finalMap;
    int reducersExponent;
    pthread_barrier_t *barrier;
    string testPath;
};
