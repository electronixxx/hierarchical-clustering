#include <cmath>
#include <utility>
#include <cassert>
#include "vector"
#include <tuple>
#include <limits>

using Features = std::vector<float>;
using Point = std::vector<float>;

class SlinkSingleThreaded {
public:
    SlinkSingleThreaded(std::vector<Point> data, int nr_items) : data(std::move(data)){
        this->nr_items = nr_items;
        // init of distance matrix
        for (int i = 0; i < data.size(); ++i) {
            for (int  j= 0; j < data.size(); ++j) {
                Features f1 = data[i];
                Features f2 = data[j];
                float d = euclideanDistance(f1, f2);
                distance[i][j] = d;
            }
        }
    }

    void cluster() {

    }

private:
    void updateDistanceMatrix(){}

    // returns the position and the value of the smallest element of the distance matrix
    std::tuple<int, int, float> distanceMin() {
        std::tuple<int, int, float> min = std::make_tuple(0, 0, std::numeric_limits<float>::max()); // current min
        for (auto it = distance.begin(); it != distance.end(); ++it) {
            size_t i = std::distance(distance.begin(), it);
            for (size_t j=0; j<i; j++) {
                if (distance[i][j] < std::get<2>(min)) {
                    min = std::make_tuple(i, j, std::numeric_limits<float>::max());
                }
            }
        }
        return min;
    }

    float getElement(int i, int j){
        if (i==j) // main diagonal
            return 0;
        if (i>j) { // lower triangular
            int temp = i;
            i = j;
            j = i;
        }
        return distance[i][j];
//        int current_offset = nr_items;
//        int current_row = 0;
//        int position = 0;
//        while(current_row != i) {
//            current_offset--;
//            current_row++;
//            position = position + current_offset;
//        }
//        position += (j-current_row); // i-th row
//        return distance[position];
    }
    void setElement(int i, int j, float val) {
        if (i==j) // main diagonal
            return;
        if (i>j) { // lower triangular
            int temp = i;
            i = j;
            j = i;
        }
        distance[i][j] = val;
    }

    static float euclideanDistance(Features p1, Features p2) {
        assert(p1.size() == p2.size());
        float sums = 0;
        // loop through all the features of the point
        for (int i = 0; i < p1.size(); ++i)
            sums+=powf(p2[i] - p1[i], 2);
        return sqrtf(sums);
    }

    std::vector<Point> data;
    std::vector<std::vector<float>> distance;
    int nr_items;
};