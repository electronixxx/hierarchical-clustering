#include <vector>
#include <tuple>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <chrono>
#include <random>
#include "clustering/ClusteringAlgorithms.h"
#include "utils/utilities.h"

using namespace std;

////    distance = {
////            {0 ,1.2 ,5 ,5 ,4.2 ,7 ,9 ,7.6 ,11 ,4.3},
////            {1.2 ,0 ,3.4 ,4.1 ,5 ,6 ,4.1 ,6.4 ,5.3 ,4.5},
////            {5 ,3.4 ,0 ,2.1 ,6 ,6.2 ,4.6 ,9 ,11.3 ,22},
////            {5 ,4.1 ,2.1 ,0 ,11 ,5 ,13 ,4.1 ,4.3 ,5.5},
////            {4.2 ,5 ,6 ,11 ,0 ,1.9 ,7 ,9 ,5.5 ,4.3},
////            {7 ,6 ,6.2 ,5 ,1.9 ,0 ,7.5 ,5.6 ,6.3 ,4.5},
////            {9 ,4.1 ,4.6 ,13 ,7 ,7.5 ,0 ,3.6 ,8 ,10},
////            {7.6 ,6.4 ,9 ,4.1 ,9 ,5.6 ,3.6 ,0 ,4.9 ,2.9},
////            {11 ,5.3 ,11.3 ,4.3 ,5.5 ,6.3 ,8 ,4.9 ,0 ,1.4},
////            {4.3 ,4.5 ,22 ,5.5 ,4.3 ,4.5 ,10 ,2.9 ,1.4 ,0}
////    };


int main() {
    vector<vector<float>> data; // input matrix

//    /* Input from File */
//    ifstream infile("input.txt");
//    if (!infile) {
//        cout << "No input file present." << endl;
//        return -1;
//    }
//    string line;
//    while (getline(infile, line)) {
//        data.emplace_back();
//        stringstream split(line); // Break down the row into column values
//        float value;
//        while (split >> value)
//            data.back().push_back(value);
//    }

    /* Fake data generator */
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(1, 10);
    for (int i=0;i< 200; i++) { // 300 objects
        data.emplace_back();
        for (int j =0; j<100; j++) {
            data.back().push_back(dist(gen));
        }
    }

    /* Hard Coded Input */
//    data = {
//        {1.0, 1.0},
//        {1.5, 1.5},
//        {5.0, 5.0},
//        {3.0, 4.0},
//        {4.0, 4.0},
//        {3.0, 3.5},
//    };

//    vector<vector<float>> distance;
//    calculateDistanceMatrix(data.size(), data, distance);

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    vector<vector<float>> d1;
    calculateDistanceMatrix(data.size(), data, d1);
    SLINK(d1, false);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << "SLINK execution time: " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " [µs]" << endl;

    begin = chrono::steady_clock::now();
    vector<vector<float>> d2;
    calculateDistanceMatrix(data.size(), data, d2);
    GeneralSingleLinkage(d2, false);
    end = chrono::steady_clock::now();
    cout << "Single Linkage execution time: " << chrono::duration_cast<chrono::microseconds>(end - begin).count() << " [µs]" << endl;
}