#include "vector"
#include <limits>
#include "../utils/utilities.h"

using namespace std;
using Point = vector<float>;

void SLINK(vector<vector<float>>& distance, bool show_output) {
    const float inf = numeric_limits<float>::infinity();
    const unsigned long int nr_points = distance.size();
    vector<int> pi(nr_points);
    vector<float> lambda(nr_points);

    // 1. & 2.
    #pragma omp parallel for simd num_threads(1) schedule(dynamic) shared(nr_points, pi, lambda, inf) default(none)
    for (int n = 0; n < nr_points; n++) {
        pi[n] = n;
        lambda[n] = inf;
    }

    for (int n=0; n<nr_points; n++) {
        // 3.
        #pragma omp simd
        for (int i=0; i<n; i++) {
            if (lambda[i] >= distance[n][i]) {
                distance[n][pi[i]] = min(distance[n][pi[i]], lambda[i]);
                lambda[i] = distance[n][i];
                pi[i] = n;
            }
            if (lambda[i] < distance[n][i]){
                distance[n][pi[i]] = min(distance[n][pi[i]], distance[n][i]);
            }
        }

        // 4.
        #pragma omp parallel for simd num_threads(1) if (n>20) schedule(dynamic) shared(lambda, pi, n) default(none)
        for (int i=0; i<n; i++) {
            if (lambda[i] >= lambda[pi[i]])
                pi[i] = n;
        }
    }

    if (show_output)
        printSLINKOutput(nr_points, pi, lambda);
}