#include "vector"

#ifndef HIERARCHICAL_CLUSTERING_CLUSTERINGALGORITHMS_H
#define HIERARCHICAL_CLUSTERING_CLUSTERINGALGORITHMS_H

using namespace std;
using Point = std::vector<float>;

void GeneralSingleLinkage(vector<vector<float>>& distance, bool show_output);
void SLINK(vector<vector<float>>& distance, bool show_output);

#endif //HIERARCHICAL_CLUSTERING_CLUSTERINGALGORITHMS_H