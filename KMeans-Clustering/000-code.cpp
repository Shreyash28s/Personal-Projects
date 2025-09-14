#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono> 


// Average execution time over 10 runs: 4265 µs

using namespace std;

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
 
    for (int iter = 0; iter < epochs; ++iter) {
        // Assign points to nearest centroid
        for (size_t c = 0; c < centroids.size(); ++c) {
            const Point& centroid = centroids[c];
            for (Point& p : points) {
                const double dist = centroid.distance(p);
                if (dist < p.minDist) {
                    p.minDist = dist;
                    p.cluster = static_cast<int>(c);
                }
            }
        }

        // Accumulate sums per cluster
        vector<int> nPoints(k, 0);
        vector<double> sumX(k, 0.0), sumY(k, 0.0);

        for (Point& p : points) {
            const int cid = p.cluster;
            if (cid >= 0 && cid < k) {
                ++nPoints[cid];
                sumX[cid] += p.x;
                sumY[cid] += p.y;
            }
            // reset for next iteration
            p.minDist = __DBL_MAX__;
        }

        // Recompute centroids (guard empty clusters)
        for (int c = 0; c < k; ++c) {
            if (nPoints[c] > 0) {
                centroids[c].x = sumX[c] / nPoints[c];
                centroids[c].y = sumY[c] / nPoints[c];
            }
            // If a cluster is empty, centroid stays where it was (simple fallback)
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
