#pragma once
#include "libraryHeaders.h"

// Brute Force Algorithm for finding the closest pair of points
std::pair<Point, Point> closestPairBruteForce(const std::vector<Point>& points, std::vector<BruteForceStep>& steps) {
    double minDist = std::numeric_limits<double>::max();
    std::pair<Point, Point> closestPair;
    size_t n = points.size();

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            double dist = distance(points[i], points[j]);

            // Record the step
            BruteForceStep step;
            step.pointA = points[i];
            step.pointB = points[j];
            step.distance = dist;
            step.currentClosestPair = closestPair;
            step.currentMinDistance = minDist;
            steps.push_back(step);

            if (dist < minDist) {
                minDist = dist;
                closestPair = { points[i], points[j] };
            }
        }
    }
    return closestPair;
}
