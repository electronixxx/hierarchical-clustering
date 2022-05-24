cmake_minimum_required(VERSION 3.21)
project(hierarchical_clustering)

set(CMAKE_CXX_STANDARD 14)

set(GCC_COVERAGE_COMPILE_FLAGS "-fopenmp -ffast-math")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(hierarchical_clustering
        src/main.cpp
        src/utils/utilities.cpp
        src/utils/utilities.h
        src/clustering/SLINK.cpp
        src/clustering/GeneralSingleLinkage.cpp
        src/clustering/ClusteringAlgorithms.h)
