#ifndef HIERARCHICAL_CLUSTERING_UTILITIES_H
#define HIERARCHICAL_CLUSTERING_UTILITIES_H
#include <string>

using namespace std;
using Features = std::vector<float>;
using Point = std::vector<float>;
using FloatMatrix = vector<vector<float>>;

static float euclideanDistance(Features p1, Features p2);
void calculateDistanceMatrix(unsigned long nr_objects, const vector<Point>& data, vector<vector<float>>& distance,  int nr_threads);
tuple<int, int, float> findMinDistance(FloatMatrix& distance, int nr_threads);

void printSLINKOutput(unsigned long n, vector<int> &pi, vector<float> &lambda);
void print2DMatrix(FloatMatrix& matrix);

FloatMatrix generateRandomData(int from, int to, int nr_objects, int nr_features);
FloatMatrix readDataFromFile(const std::string& file_name);
FloatMatrix getHardCodedData();
FloatMatrix getHardCodedDistance();

#endif //HIERARCHICAL_CLUSTERING_UTILITIES_H