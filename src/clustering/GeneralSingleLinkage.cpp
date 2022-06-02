#include "vector"
#include <tuple>
#include <iostream>
#include <map>
#include <sstream>

#include "../utils/utilities.h"

using namespace std;
using Features = std::vector<float>;
using Point = std::vector<float>;

void GeneralSingleLinkage(vector<vector<float>> &distance, bool show_output, int nr_threads) {
    const unsigned long int nr_points = distance.size();
    map<int, string> CL; // stores the pairs of clusters

    // initial clusters = point IDs
    //#pragma omp parallel for shared(distance, CL), default(none)
    for(int i=0; i<distance.size(); i++) {
        stringstream ss;
        ss << 'P' << i;
        CL[i] = ss.str();
    }

    // continuously find the min until matrix reduction to 0x0
    for(int iter=0; iter<nr_points-1; iter++) {
        // find current global min (of the lower triangle)
        tuple<int, int, float> m = findMinDistance(distance, nr_threads);
        int i = get<0>(m);
        int j = get<1>(m);
        float val = get<2>(m);

        stringstream ss; // new formed cluster (composition of two clusters)
        ss << '(' << CL[i]<< ", " << CL[j] << ")";
        CL[std::min(i, j)] = ss.str();

        for (int g=i; g < distance.size(); g++)
            CL[g] = CL[g+1];

        CL.erase (CL.find ((int) CL.size() - 1));

        if (show_output) cout << "Cluster: " << ss.str() << " -> Distance: " << val  << "\n\n";

        vector<float> respectiveDistances; // distances from the new cluster to all the others
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
        #pragma omp parallel for shared(distance, i), default(none)
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