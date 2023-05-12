#include "vector"
#include "string"
#include "check_if_power_function.h"
#include <fstream>

using namespace std;
void *mapFunction(void *threadFunctionArguments) {
    auto *myArguments = (struct MapThreadFunctionArguments *) threadFunctionArguments;
    vector<int> listOfPowers;
    int number = 0;
    int numberOfItems = 0;


    while (!myArguments->filesQueue->empty()) {
        pthread_mutex_lock(myArguments->mapMutex);
        if (myArguments->filesQueue->empty()) {
            pthread_mutex_unlock(myArguments->mapMutex);
            return nullptr;
        }
        std::string fileToRead = myArguments->filesQueue->front();
        myArguments->filesQueue->pop();
        std::ifstream infile(fileToRead);
        pthread_mutex_unlock(myArguments->mapMutex);
        infile >> numberOfItems;

        while (numberOfItems) {
            infile >> number;
            for (int i = 2; i <= myArguments->numberOfPowers + 1; i++) {
                if (number == 1) {
                    myArguments->threadMap->at(i).push_back(number);
                } else {
                    if (checkIfPower(number, i)) {
                        myArguments->threadMap->at(i).push_back(number);
                    }
                }
            }
            listOfPowers = {};
            numberOfItems--;
        }
        pthread_mutex_lock(myArguments->mapMutex);
        infile.close();
        pthread_mutex_unlock(myArguments->mapMutex);

    }
    pthread_barrier_wait(myArguments->barrier);
    pthread_exit(nullptr);
}
