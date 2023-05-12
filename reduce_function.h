#include "vector"
#include "set"
#include "fstream"

using namespace std;

void *reduceFunction(void *threadFunctionArguments) {
    auto *myArguments = (struct ReduceThreadFunctionArguments *) threadFunctionArguments;
    pthread_barrier_wait(myArguments->barrier);

    // Here I am combining the lists of numbers for the exponent that was assigned
    // to the reducer thread.
    for (auto map: *myArguments->mapList) {
        myArguments->finalMap->at(myArguments->reducersExponent).insert(
                myArguments->finalMap->at(myArguments->reducersExponent).end(), map[myArguments->reducersExponent].begin(),
                map[myArguments->reducersExponent].end());
    }

    // I make a set from that combined list in order to get rid of the duplicates,
    // it helps me find easily how many different numbers I have.
    std::set<int> vectorWithoutDuplicates(myArguments->finalMap->at(myArguments->reducersExponent).begin(),
                                          myArguments->finalMap->at(myArguments->reducersExponent).end());

    // After I have the set I replace the first list with it
    myArguments->finalMap->at(myArguments->reducersExponent).assign(vectorWithoutDuplicates.begin(),
                                                                    vectorWithoutDuplicates.end());

    // I create the file where the thread write the result.
    // The result is the size of the list because now it has no more duplicates
    // and the size is the number of distinct numbers.
    std::ofstream outFile("out" + std::to_string(myArguments->reducersExponent) + ".txt");
    outFile << myArguments->finalMap->at(myArguments->reducersExponent).size();
    outFile.close();
    pthread_exit(nullptr);
}
