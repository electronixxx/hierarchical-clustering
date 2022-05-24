#include <cmath>
#include <cassert>
#include "vector"
#include <tuple>
#include <limits>
#include <iostream>
#include <iomanip>

using namespace std;
using Features = vector<float>;
using Point = vector<float>;

// finds the distance between two points in an n-dimensional space. p1 and p2 need to have same cardinality
static float euclideanDistance(Features p1, Features p2) {
    assert(p1.size() == p2.size());
    float sums = 0;
    #pragma openmp simd //reduction(+,sums)
    for (int i = 0; i < p1.size(); i++) { // loop through all the features of the point
        float diff = p2[i] - p1[i];
        sums = sums + powf(diff, 2);
    }
    return sqrtf(sums);
}

// calculates the distance matrix from a vector of Object PointsWW
void calculateDistanceMatrix(unsigned long const nr_objects, const vector<Point>& data, vector<vector<float>>& distance) {
    distance = vector<vector<float>>(nr_objects);
    for (int i=0; i<nr_objects; i++)
        distance[i] = vector<float>(i+1);

    #pragma omp parallel for num_threads(1) schedule(dynamic) shared(data, distance) default(none)
    for (int i = 0; i < data.size(); i++)
        #pragma omp parallel for num_threads(1) schedule(dynamic) shared(i, data, distance) default(none)
        for (int j = 0; j <= i; j++)
            distance[i][j] = euclideanDistance(data[i], data[j]);
}

// finds the minimum distance from all current distances and returns the position with the value
tuple<int, int, float> findMinDistance(vector<vector<float>>& distance) {
    const float inf = numeric_limits<float>::infinity();
   // tuple<int, int, float> m = make_tuple(0, 0, inf); // current min

    float x1 = inf;
    int x2 = 0;
    int x3 = 0;

    #pragma omp parallel for num_threads(1) reduction(min:x1,x2,x3) shared(distance) default (none)
    for(int i=1; i<distance.size(); i++) {
        for(int j=0; j<distance[i].size()-1; j++) {
            if (distance[i][j] < x1) {
                x1 = distance[i][j];
                x2 = i;
                x3 = j;
            }
        }
    }
    return make_tuple(x2,x3,x1);;
}

void printSLINKOutput(unsigned long n, vector<int> &pi, vector<float> &lambda) {
    cout << "n \t pi\t lambda\n";
    for(int i=0; i<n; i++)
        cout << i <<  " \t" << pi[i] << " \t" << lambda[i] << "\n";
}

void print2DMatrix (vector<vector<float>>& matrix) {
    cout << setprecision(2) << std::fixed;
    for(auto & i : matrix) {
        for(float j : i)
            std::cout << j <<  " ";
        std::cout << "\n";
    }
    std::cout << "\n";
}