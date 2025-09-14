#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono> 
#include <omp.h> 
#include <cfloat>

using namespace std;

// v2 uses reductions, removed no wait, more efficient use of shared and private variables


struct Point {
    double x, y;
    int cluster;
    double minDist;

    Point() : x(0.0), y(0.0), cluster(-1), minDist(__DBL_MAX__) {}
    Point(double x_, double y_) : x(x_), y(y_), cluster(-1), minDist(__DBL_MAX__) {}

    // Distance now takes a const reference and is a const member function - Euclidean distance between point p and this point
    double distance(const Point& p) const {
        return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y);
    }
};



vector<Point> readcsv() {
    vector<Point> points;
    ifstream file("sample_large.csv");
    string line;

    bool first = true;
    while (getline(file, line)) {
        if (line.empty()) continue;

        // Skip header once
        if (first) { first = false; continue; }

        stringstream ss(line);
        string token;
        double x, y;

        if (!getline(ss, token, ',')) continue;
        x = stod(token);
        if (!getline(ss, token, ',')) continue;
        y = stod(token);

        points.emplace_back(x, y);
    }
    return points;
}



// k-means using references 
void kMeansClustering(vector<Point>& points, int epochs, int k) {
    const int n = static_cast<int>(points.size());
    if (n == 0 || k <= 0) return;

    // Randomly initialise centroids from existing points
    vector<Point> centroids;
    centroids.reserve(k);
    srand(static_cast<unsigned>(time(nullptr)));
    for (int i = 0; i < k; ++i) {
        centroids.push_back(points[rand() % n]);
    }
 
    // shared accumulators (targets of reductions)
    vector<int>    nPoints(k, 0);
    vector<double> sumX(k, 0.0), sumY(k, 0.0);

    // get raw pointers for array-section reduction - fixed compilation errors
    int*    nPointsA = nPoints.data();
    double* sumXA    = sumX.data();
    double* sumYA    = sumY.data();

    #pragma omp parallel shared(points, centroids, nPointsA, sumXA, sumYA, k, n)
    {
        for (int iter = 0; iter < epochs; ++iter) {

            // 1) assign nearest centroid - parallel over points 
            #pragma omp for schedule(static) 
            for (int i = 0; i < n; ++i) {
                double best = DBL_MAX; int bestc = -1;
                for (int c = 0; c < k; ++c) {
                    double d = centroids[c].distance(points[i]);
                    if (d < best) { best = d; bestc = c; }
                }
                points[i].cluster = bestc;
                points[i].minDist = best;
            }

            // implicit barrier here because theres no "no wait" in parallel for - no need for "#pragma omp barrier"

            // 2) accumulate with ARRAY-SECTION REDUCTIONS 
            // Each thread gets private copies, combined once at the end—no atomics.
            #pragma omp for schedule(static) reduction(+: nPointsA[:k], sumXA[:k], sumYA[:k])
            for (int i = 0; i < n; ++i) {
                int c = points[i].cluster;
                if (c >= 0) {
                    nPointsA[c] += 1;
                    sumXA[c]    += points[i].x;
                    sumYA[c]    += points[i].y;
                }
                points[i].minDist = DBL_MAX; // reset for next iteration
            }

            // 3) update centroids & reset shared accumulators for next epoch
            #pragma omp for schedule(static)
            for (int c = 0; c < k; ++c) {
                if (nPoints[c] > 0) {
                    centroids[c].x = sumXA[c] / nPoints[c];
                    centroids[c].y = sumYA[c] / nPoints[c];
                }
                nPoints[c] = 0; sumX[c] = 0.0; sumY[c] = 0.0;
            }
            
        }
    }


    // Write result
    ofstream out("output.csv");
    out << "x,y,c\n";
    for (const Point& p : points) {
        out << p.x << ',' << p.y << ',' << p.cluster << '\n';
    }
}

int main() {
    const int trials = 10;

    // Load once
    vector<Point> base = readcsv();
    if (base.empty()) {
        cerr << "No points loaded.\n";
        return 1;
    }

    long long total_us = 0;

    // Run the kmeans algo 10 times and output the average result

    for (int t = 0; t < trials; ++t) {
        vector<Point> points = base;

        auto start = chrono::high_resolution_clock::now();
        kMeansClustering(points, 100, 5);   
        auto end = chrono::high_resolution_clock::now();

        total_us += chrono::duration_cast<chrono::microseconds>(end - start).count();
    }

    cout << "Average execution time over " << trials << " runs: "
         << (total_us / trials) << " µs" << endl;
    return 0;
}

