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

using namespace std;
using Features = std::vector<float>;
using Point = std::vector<float>;

static float euclideanDistance(Features p1, Features p2) {
    assert(p1.size() == p2.size());
    float sums = 0;
    for (int i = 0; i < p1.size(); ++i) // loop through all the features of the point
        sums+=powf(p2[i] - p1[i], 2);
    return sqrtf(sums);
}

std::tuple<int, int, float> distanceMin(std::vector<std::vector<float>>& distance) {
    std::tuple<int, int, float> min = std::make_tuple(0, 0, std::numeric_limits<float>::max()); // current min
    for(int i=1; i<distance.size(); i++) {
        for(int j=0; j<distance[i].size()-1; j++) {
            if (distance[i][j] < std::get<2>(min)) {
                get<0>(min) = i;
                get<1>(min) = j;
                get<2>(min) = distance[i][j];
            }
        }
    }
    return min;
}

void printMatrix(std::vector<std::vector<float>>& matrix) {
    std::cout << std::setprecision(2) << std::fixed;
    for(auto & i : matrix) {
        for(float j : i)
            std::cout << j <<  " ";
        std::cout << "\n";
    }
    std::cout << "\n";
}

void singleLinkageClustering(const std::vector<Point>& data, bool parallelize, bool show_output) {
    std::vector<std::vector<float>> distance;
    std::map<int, string> CL; // stores the pairs of clusters

    #pragma omp parallel if(parallelize) num_threads(10)
    {
        // compute the distance matrix
        for (int i = 0; i < data.size(); i++) {
            std::vector<float> current_row;
            current_row.reserve(i + 1);
            for (int j = 0; j <= i; j++)
                current_row.push_back(euclideanDistance(data[i], data[j]));
            distance.push_back(current_row);
        }
    }


    if (show_output) {
        cout << "Initial Distance Matrix: \n";
        printMatrix(distance);
    }

    size_t iterations = distance.size();

    // initial clusters = point IDs
    // clusters.reserve(data.size());
    for(int i=0; i<data.size(); i++) {
        std::stringstream ss;
        ss << 'P' << i;
        CL[i] = ss.str();
        //clusters.push_back(std::unique_ptr<Cluster>(new Number(i)) );
    }

    // continuously find the min until matrix reduction to 0x0
    for(int iter=0; iter<iterations-1; iter++) {
        // find current global min (of the lower triangle)
        std::tuple<int, int, float> m = distanceMin(distance);
        int i = std::get<0>(m);
        int j = std::get<1>(m);
        float val = std::get<2>(m);

        std::stringstream ss; // new formed cluster (composition of two clusters)
        ss << '(' << CL[i]<< ", " << CL[j] << ")";
        CL[std::min(i, j)] = ss.str();

        for (int g=i; g < distance.size(); g++)
            CL[g] = CL[g+1];

        CL.erase (CL.find ((int) CL.size() - 1));

        if (show_output) cout << "Cluster: " << ss.str() << " -> Distance: " << val  << "\n\n";

        std::vector<float> respectiveDistances; // distances from the new cluster to all the others
        // recompute distances
        for(int v=0; v<distance.size(); v++) {
            if (v==i || v==j) continue;
            // find min value between [i][v] and [j][v] taking in consideration the lower triangle indexes
            float a = (i>v) ? distance[i][v] : distance[v][i];
            float b = (j>v) ? distance[j][v] : distance[v][j];
            float min = std::min(a, b);
            respectiveDistances.push_back(min);
        }

        // del col i
        for(int c=i; c<distance.size(); c++)
            distance[c].erase(distance[c].begin() + i);
        // del row i
        distance.erase(distance.begin() + i);

        // distance update according to the indexes
        int c=0;
        for(int z=i; z<distance.size(); z++)
            distance[z][j] = respectiveDistances[c++];
        for(int z=0; z<j; z++)
            distance[j][z] = respectiveDistances[c++];

    }
}

int main() {
    std::vector<std::vector<float>> data; // input matrix

    /* Input from File */
    ifstream infile("input.txt");
    if (!infile) {
        cout << "No input file present." << endl;
        return -1;
    }
    std::string line;
    while (std::getline(infile, line)) {
        data.emplace_back();
        std::stringstream split(line); // Break down the row into column values
        float value;
        while (split >> value)
            data.back().push_back(value);
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

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    singleLinkageClustering(data, false, true);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Single Linkage execution time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " [µs]" << std::endl;
}