#pragma once
#include "libraryHeaders.h"

// Comparison functions for sorting
bool compareX(const Point& a, const Point& b) {
    return a.x < b.x;
}

bool compareY(const Point& a, const Point& b) {
    return a.y < b.y;
}

// Recursive function for the Divide and Conquer algorithm
std::pair<Point, Point> closestPairRecursive(std::vector<Point>& Px, std::vector<Point>& Py, std::vector<DivideAndConquerStep>& steps) {
    size_t n = Px.size();

    if (n <= 3) {
        double minDist = std::numeric_limits<double>::max();
        std::pair<Point, Point> minPair;

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                double dist = distance(Px[i], Px[j]);

                // Record the step
                DivideAndConquerStep step;
                step.pointA = Px[i];
                step.pointB = Px[j];
                step.distance = dist;
                step.currentClosestPair = minPair;
                step.currentMinDistance = minDist;
                step.showDivisionLine = false;
                steps.push_back(step);

                if (dist < minDist) {
                    minDist = dist;
                    minPair = { Px[i], Px[j] };
                }
            }
        }
        return minPair;
    }

    size_t mid = n / 2;
    Point midPoint = Px[mid];

    // Record the division step
    DivideAndConquerStep divisionStep;
    divisionStep.showDivisionLine = true;
    divisionStep.divisionLineX = midPoint.x;
    divisionStep.currentMinDistance = std::numeric_limits<double>::max();
    steps.push_back(divisionStep);

    std::vector<Point> Pxl(Px.begin(), Px.begin() + mid);
    std::vector<Point> Pxr(Px.begin() + mid, Px.end());

    std::vector<Point> Pyl, Pyr;
    for (const auto& point : Py) {
        if (point.x <= midPoint.x) {
            Pyl.push_back(point);
        }
        else {
            Pyr.push_back(point);
        }
    }

    std::pair<Point, Point> dl = closestPairRecursive(Pxl, Pyl, steps);
    std::pair<Point, Point> dr = closestPairRecursive(Pxr, Pyr, steps);

    double distLeft = distance(dl.first, dl.second);
    double distRight = distance(dr.first, dr.second);

    double d = distLeft;
    std::pair<Point, Point> minPair = dl;
    if (distRight < distLeft) {
        d = distRight;
        minPair = dr;
    }

    // Record the current minimal distance after recursion
    DivideAndConquerStep afterRecursionStep;
    afterRecursionStep.currentClosestPair = minPair;
    afterRecursionStep.currentMinDistance = d;
    afterRecursionStep.showDivisionLine = false;
    steps.push_back(afterRecursionStep);

    // Build strip
    std::vector<Point> strip;
    for (const auto& point : Py) {
        if (std::abs(point.x - midPoint.x) < d) {
            strip.push_back(point);
        }
    }

    double minStripDist = d;
    for (size_t i = 0; i < strip.size(); ++i) {
        for (size_t j = i + 1; j < strip.size() && (strip[j].y - strip[i].y) < minStripDist; ++j) {
            double dist = distance(strip[i], strip[j]);

            // Record the comparison in strip
            DivideAndConquerStep step;
            step.pointA = strip[i];
            step.pointB = strip[j];
            step.distance = dist;
            step.currentClosestPair = minPair;
            step.currentMinDistance = minStripDist;
            step.showDivisionLine = false;
            steps.push_back(step);

            if (dist < minStripDist) {
                minStripDist = dist;
                minPair = { strip[i], strip[j] };
                d = dist;
            }
        }
    }

    // Record the current minimal distance after checking strip
    DivideAndConquerStep afterStripStep;
    afterStripStep.currentClosestPair = minPair;
    afterStripStep.currentMinDistance = d;
    afterStripStep.showDivisionLine = false;
    steps.push_back(afterStripStep);

    return minPair;
}

// Divide and Conquer Algorithm for finding the closest pair of points
std::pair<Point, Point> closestPairDivideAndConquer(std::vector<Point>& points, std::vector<DivideAndConquerStep>& steps) {
    std::vector<Point> Px = points;
    std::vector<Point> Py = points;

    std::sort(Px.begin(), Px.end(), compareX);
    std::sort(Py.begin(), Py.end(), compareY);

    return closestPairRecursive(Px, Py, steps);
}
