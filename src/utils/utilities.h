#include <cmath>
#include <utility>
#include <cassert>
#include "vector"
#include <tuple>
#include <limits>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <chrono>
#include <omp.h>
#include <immintrin.h>
#include <avxintrin.h>

#ifndef HIERARCHICAL_CLUSTERING_UTILITIES_H
#define HIERARCHICAL_CLUSTERING_UTILITIES_H

using namespace std;
using Features = std::vector<float>;
using Point = std::vector<float>;

static float euclideanDistance(Features p1, Features p2);
void calculateDistanceMatrix(unsigned long nr_objects, const vector<Point>& data, vector<vector<float>>& distance);
tuple<int, int, float> findMinDistance(vector<vector<float>>& distance);

void printSLINKOutput(unsigned long n, vector<int> &pi, vector<float> &lambda);
void print2DMatrix (vector<vector<float>>& matrix);

#endif //HIERARCHICAL_CLUSTERING_UTILITIES_H
