#include <vector>
#include <tuple>
#include <iostream>
#include <chrono>
#include "clustering/ClusteringAlgorithms.h"
#include "utils/utilities.h"

int main() {
    using namespace std;
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
    vector<vector<float>> d1;
    calculateDistanceMatrix(data.size(), data, d1);
    SLINK(d1, false);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "SLINK execution time: " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " [µs]" << endl;

    /* General Single Linkage */
    begin = chrono::steady_clock::now();
    vector<vector<float>> d2;
    calculateDistanceMatrix(data.size(), data, d2);
    GeneralSingleLinkage(d2, false);
    end = chrono::steady_clock::now();
    cout << "General Single Linkage execution time: " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " [µs]" << endl;
}