#include <vector>
#include <tuple>
#include <iostream>
#include <chrono>
#include "clustering/ClusteringAlgorithms.h"
#include "utils/utilities.h"

int nr_threads = 4;

int main() {
    using namespace std;
    int nr_threads = 1;
    FloatMatrix data = {};

    /* Input from File */
    //data = readDataFromFile("../inputs/dataset_1.txt");

    /* Fake Data */
    data = generateRandomData(1, 10, 2000, 100);

    /* Hard Coded Data */
    //data = getHardCodedData();

    /* Hard Coded Distance */
    //distance = getHardCodedDistance();


    /* SLINK */
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    nr_threads = 4;
    vector<vector<float>> d1;
    calculateDistanceMatrix(data.size(), data, d1, nr_threads);
    SLINK(d1, false, nr_threads);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "SLINK execution time: " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " [µs]" << endl;

    /* General Single Linkage */
    begin = chrono::steady_clock::now();
    nr_threads = 8;
    vector<vector<float>> d2;
    calculateDistanceMatrix(data.size(), data, d2, nr_threads);
    GeneralSingleLinkage(d2, false, nr_threads);
    end = chrono::steady_clock::now();
    cout << "General Single Linkage execution time: " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " [µs]" << endl;
}