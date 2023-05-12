#include <iostream>
#include <list>
#include <map>
#include <iterator>
#include <cstdlib>
#include <queue>
#include <pthread.h>
#include <vector>
#include "pthread_barrier_mac.h"
#include "map_function_arguments.h"
#include "reduce_function_arguments.h"
#include "map_function.h"
#include "reduce_function.h"

using namespace std;

int main(int argc, char const *argv[]) {
    // I am getting the arguments from command line
    const int numberOfMappers = atoi(argv[1]);
    const int numberOfReducers = atoi(argv[2]);
    string testFile = argv[3];


    // Here I initialize the arguments that I need for creating the threads
    // and also for joining them.
    pthread_t threads[numberOfMappers + numberOfReducers];
    int r;
    long id;
    void *status;

    // Initializing the arguments for map function
    vector<map<int, vector<int> > > threadMapList(numberOfMappers);
    map<int, vector<int> > threadMap;
    MapThreadFunctionArguments mapArgumentsList[numberOfMappers];
    pthread_mutex_t mapMutex;
    pthread_mutex_init(&mapMutex, nullptr);

    // Initializing the arguments for reduce function
    ReduceThreadFunctionArguments reduceArgumentsList[numberOfReducers];
    int reducersExponent = 2;
    map<int, vector<int> > finalMap;
    string testRootPath = testFile.substr(0, 5);

    // Here I initialize the barrier for checking if the mappers finished
    // their work.
    pthread_barrier_t barrier;
    pthread_barrier_init(&barrier, nullptr, numberOfMappers + numberOfReducers);

    // Here I create a queue for the files that the mappers have to read
    string filePath;
    queue<string> filesQueue;
    int numberOfFiles;
    std::ifstream infile(testFile);
    infile >> numberOfFiles;
    while (numberOfFiles) {
        infile >> filePath;
        filesQueue.push(filePath);
        numberOfFiles--;
    }
    infile.close();

    // Adding the keys in the final map for reducers
    for (int j = 2; j <= numberOfReducers + 1; ++j) {
        finalMap[j] = {};
    }

    // Adding maps with the specified keys in the list of maps that
    // will be used by the mappers(every mapper has its map)
    for (int i = 0; i < numberOfMappers; ++i) {
        for (int j = 2; j <= numberOfReducers + 1; ++j) {
            threadMapList[i][j] = {};
        }
    }

    // Here I start creating the threads, when I finish creating the given number
    // of mappers I start creating the reducers, I also initialize every argument
    // of their structure
    for (id = 0; id < numberOfMappers + numberOfReducers; id++) {
        if (id < numberOfMappers) {
            mapArgumentsList[id].filesQueue = &filesQueue;
            mapArgumentsList[id].numberOfPowers = numberOfReducers;
            mapArgumentsList[id].threadMap = &threadMapList.at(id);
            mapArgumentsList[id].mapMutex = &mapMutex;
            mapArgumentsList[id].barrier = &barrier;
            r = pthread_create(&threads[id], nullptr, mapFunction, &mapArgumentsList[id]);
        }

        if (id >= numberOfMappers) {
            reduceArgumentsList[id - numberOfMappers].mapList = &threadMapList;
            reduceArgumentsList[id - numberOfMappers].finalMap = &finalMap;
            reduceArgumentsList[id - numberOfMappers].reducersExponent = reducersExponent;
            reduceArgumentsList[id - numberOfMappers].barrier = &barrier;
            reduceArgumentsList[id - numberOfMappers].testPath = testRootPath;
            reducersExponent++;
            r = pthread_create(&threads[id], nullptr, reduceFunction, &reduceArgumentsList[id - numberOfMappers]);
        }

        if (r) {
            printf("Eroare la crearea thread-ului %ld\n", id);
            exit(-1);
        }
    }

    // Here I am joining the threads
    for (id = 0; id < numberOfMappers + numberOfReducers; id++) {
        r = pthread_join(threads[id], &status);

        if (r) {
            printf("Eroare la asteptarea thread-ului %ld\n", id);
            exit(-1);
        }
    }

    pthread_barrier_destroy(&barrier);
    pthread_exit(nullptr);
}
