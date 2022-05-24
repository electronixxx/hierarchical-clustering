//#include "HierarchicalClustering.h"
//
//#include <cassert>
//#include <cassert>
//#include "vector"
//#include <tuple>
//#include <limits>
//#include <iostream>
//#include <iomanip>
//#include <fstream>
//#include <map>
//#include <chrono>
//#include <cmath>
//#include <utility>
//#include <omp.h>
//#include <immintrin.h>
//#include <avxintrin.h>
//using Features = std::vector<float>;
//
//HierarchicalClustering::HierarchicalClustering(std::vector<std::vector<float>> *d) : data(d) {
//
//}
//
//void HierarchicalClustering::calculateDistanceMatrix() {
//    unsigned long const n = (*data).size();
//
//    distance = vector<vector<float>>(n);
//    for (int i=0; i<n; i++)
//        distance[i] = vector<float>(i+1);
//
//    #pragma omp parallel for schedule(dynamic) shared(data, distance) default(none) //  for schedule(dynamic)
//    for (int i = 0; i < data.size(); i++)
//        #pragma omp parallel for schedule(dynamic) shared(i, data, distance) default(none)
//        for (int j = 0; j <= i; j++)
//            distance[i][j] = calculateEuclideanDistance((*data)[i], (*data)[j]);
//
//}
//
//static float calculateEuclideanDistance(Features p1, Features p2) {
//    assert(p1.size() == p2.size());
//
//    float sums = 0;
//    #pragma openmp simd //reduction(+,sums)
//    for (int i = 0; i < p1.size(); i++) { // loop through all the features of the point
//        float dist = p2[i] - p1[i];
//        sums = sums + powf(dist, 2);
//    }
//    return sqrtf(sums);
//}
//
//void applySLINK() {
//    const unsigned long nr_points = distance.size();
//    std::vector<int> pi(nr_points);
//    std::vector<float> lambda(nr_points);
//}