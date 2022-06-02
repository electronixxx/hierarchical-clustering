#include "vector"

#ifndef HIERARCHICAL_CLUSTERING_CLUSTERINGALGORITHMS_H
#define HIERARCHICAL_CLUSTERING_CLUSTERINGALGORITHMS_H

using namespace std;

void GeneralSingleLinkage(vector<vector<float>>& distance, bool show_output, int nr_threads);
void SLINK(vector<vector<float>>& distance, bool show_output, int nr_threads);

#endif //HIERARCHICAL_CLUSTERING_CLUSTERINGALGORITHMS_H