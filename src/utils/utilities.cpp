#include <cmath>
#include <cassert>
#include <vector>
#include <tuple>
#include <limits>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <random>

using namespace std;
using Features = vector<float>;
using Point = vector<float>;
using FloatMatrix = vector<vector<float>>;

// finds the distance between two points in an n-dimensional space. p1 and p2 need to have same dimension
static float euclideanDistance(Features p1, Features p2) {
    assert(p1.size() == p2.size());
    float sums = 0.;
    #pragma openmp simd // reduction(+,sums)
    for (int i = 0; i < p1.size(); i++) { // loop through all the features of the point
        float diff = p2[i] - p1[i];
        sums = sums + powf(diff, 2);
    }
    return sqrtf(sums);
}

// calculates the distance matrix from a vector of Object PointsWW
void calculateDistanceMatrix(unsigned long const nr_objects, const vector<Point>& data, vector<vector<float>>& distance) {
    // allocate the necessary memory for a triangular matrix
    distance = vector<vector<float>>(nr_objects);
    #pragma omp parallel for schedule(dynamic) shared(nr_objects, distance) default(none)
    for (int i=0; i<nr_objects; i++)
        distance[i] = vector<float>(i+1);

    #pragma omp parallel for num_threads(1) schedule(dynamic) shared(data, distance) default(none)
    for (int i = 0; i < data.size(); i++)
        #pragma omp parallel for num_threads(1) schedule(dynamic) shared(i, data, distance) default(none)
        for (int j = 0; j <= i; j++)
            distance[i][j] = euclideanDistance(data[i], data[j]); // compute distance for each point
}

// finds the minimum distance from all current distances and returns the position with the value
tuple<int, int, float> findMinDistance(vector<vector<float>>& distance) {
    const float inf = numeric_limits<float>::infinity();

    float val = inf;
    int index_i = 0;
    int index_j = 0;

    #pragma omp parallel for num_threads(1) reduction(min:val) shared(distance, index_i, index_j) default (none)
    for(int i=1; i<distance.size(); i++) {
        for(int j=0; j<distance[i].size()-1; j++) {
            #pragma omp critical
            if (distance[i][j] < val) {
                val = distance[i][j];
                index_i = i;
                index_j = j;
            }
        }
    }
    return make_tuple(index_i, index_j, val);
}

// prints the SLINK output
void printSLINKOutput(unsigned long n, vector<int> &pi, vector<float> &lambda) {
    cout << "n \t pi\t lambda\n";
    for(int i=0; i<n; i++)
        cout << i <<  " \t" << pi[i] << " \t" << lambda[i] << "\n";
}

// prints a 2D matrix
void print2DMatrix(FloatMatrix& matrix) {
    cout << setprecision(2) << fixed;
    for(auto & i : matrix) {
        for(float j : i)
            cout << j <<  " ";
        cout << "\n";
    }
    cout << "\n";
}

/*** Data Generators - Testing ***/

FloatMatrix generateRandomData(int from, int to, int nr_objects, int nr_features) {
    FloatMatrix data;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(from, to);
    for (int i=0;i< nr_objects; i++) { // 300 objects
        data.emplace_back();
        for (int j =0; j<nr_features; j++)
            data.back().push_back(dist(gen));
    }
    return data;
}

FloatMatrix readDataFromFile(const std::string& file_name) {
    FloatMatrix data;
    ifstream infile(file_name);
    if (!infile) {
        cout << "No input file present." << endl;
        return {};
    }
    string line;
    while (getline(infile, line)) {
        data.emplace_back();
        stringstream split(line); // Break down the row into column values
        float value;
        while (split >> value)
            data.back().push_back(value);
    }
    return data;
}

FloatMatrix getHardCodedData() {
    return {
        {1.0, 1.0},
        {1.5, 1.5},
        {5.0, 5.0},
        {3.0, 4.0},
        {4.0, 4.0},
        {3.0, 3.5},
    };
}

FloatMatrix getHardCodedDistance() {
    return {
            {0 ,1.2 ,5 ,5 ,4.2 ,7 ,9 ,7.6 ,11 ,4.3},
            {1.2 ,0 ,3.4 ,4.1 ,5 ,6 ,4.1 ,6.4 ,5.3 ,4.5},
            {5 ,3.4 ,0 ,2.1 ,6 ,6.2 ,4.6 ,9 ,11.3 ,22},
            {5 ,4.1 ,2.1 ,0 ,11 ,5 ,13 ,4.1 ,4.3 ,5.5},
            {4.2 ,5 ,6 ,11 ,0 ,1.9 ,7 ,9 ,5.5 ,4.3},
            {7 ,6 ,6.2 ,5 ,1.9 ,0 ,7.5 ,5.6 ,6.3 ,4.5},
            {9 ,4.1 ,4.6 ,13 ,7 ,7.5 ,0 ,3.6 ,8 ,10},
            {7.6 ,6.4 ,9 ,4.1 ,9 ,5.6 ,3.6 ,0 ,4.9 ,2.9},
            {11 ,5.3 ,11.3 ,4.3 ,5.5 ,6.3 ,8 ,4.9 ,0 ,1.4},
            {4.3 ,4.5 ,22 ,5.5 ,4.3 ,4.5 ,10 ,2.9 ,1.4 ,0}
    };
}