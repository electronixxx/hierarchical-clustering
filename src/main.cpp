#include <cmath>
#include <utility>
#include <cassert>
#include "vector"
#include <tuple>
#include <limits>
#include <iostream>
#include <iomanip>
#include <fstream>

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

void singleLinkageClustering(const std::vector<Point>& data) {
    std::vector<std::vector<float>> distance;
    // compute the distance matrix
    for(int i=0; i<data.size(); i++) {
        std::vector<float> current_row;
        current_row.reserve(i+1);
        for(int j=0; j<=i; j++)
            current_row.push_back(euclideanDistance(data[i], data[j]));
        distance.push_back(current_row);
    }

    cout << "Initial Distance Matrix: \n";
    printMatrix(distance);
    size_t iterations = distance.size();

    // continuously find the min until matrix reduction to 0x0
    for(int iter=0; iter<iterations-1; iter++) {
        // find global current min (lower triangle)
        std::tuple<int, int, float> m = distanceMin(distance);
        int i = std::get<0>(m);
        int j = std::get<1>(m);
        float val = std::get<2>(m);

        cout << "Cluster: [P" << i << ", P" << j << "] -> Distance: " << val  << "\n";

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

        //printMatrix(distance);
    }
}


int main() {
    ifstream infile("input.txt");
    if (!infile) {
        cout << "No input file present." << endl;
        return -1;
    }
    std::vector<std::vector<float>> data;

    std::string line;

    while (std::getline(infile, line)) {
        data.emplace_back();

        // Break down the row into column values
        std::stringstream split(line);
        //std::cout << line;
        float value;

        while (split >> value)
            data.back().push_back(value);
    }
//    for (int i = 0; i < data.size(); i++) {
//        for (int j = 0; j < data[i].size(); j++)
//            std::cout << data[i][j] << " ";
//        std::cout << '\n';
//    }
    //return 0;


//    std::vector<std::vector<float>> data = {
//            {1.0, 1.0},
//            {1.5, 1.5},
//            {5.0, 5.0},
//            {3.0, 4.0},
//            {4.0, 4.0},
//            {3.0, 3.5},
//    };
    singleLinkageClustering(data);
}