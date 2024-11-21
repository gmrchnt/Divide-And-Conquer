#include "libraryHeaders.h"
#include "visualization.h"
#include "bruteForce.h"
#include "divideAndConquer.h"
#include <fstream>
#include <chrono> // For measuring execution time

int main(int argc, char* argv[]) {
    // Uncomment if you want to use command-line arguments
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    // Read points from file
    std::ifstream inputFile(argv[1]); // Or use "C:/Users/Mohit Computers/Desktop/New folder (2)/1.txt"
    if (!inputFile) {
        std::cerr << "Error: Unable to open file.\n";
        return 1;
    }

    std::vector<Point> points;
    double x, y;

    while (inputFile >> x >> y) {
        points.push_back({ x, y });
    }

    inputFile.close();

    // Check if there are at least 2 points
    if (points.size() < 2) {
        std::cerr << "Error: The file must contain at least 2 points.\n";
        return 1;
    }

    // Brute Force Algorithm Visualization
    std::vector<BruteForceStep> bruteForceSteps;
    auto startBruteForce = std::chrono::high_resolution_clock::now(); // Start time
    std::pair<Point, Point> bruteForceClosestPair = closestPairBruteForce(points, bruteForceSteps);
    auto endBruteForce = std::chrono::high_resolution_clock::now();   // End time
    auto bruteForceDuration = std::chrono::duration_cast<std::chrono::microseconds>(endBruteForce - startBruteForce).count();

    std::cout << "Brute Force Closest Pair: (" << bruteForceClosestPair.first.x << ", " << bruteForceClosestPair.first.y << ") and ("
        << bruteForceClosestPair.second.x << ", " << bruteForceClosestPair.second.y << ")\n";
    std::cout << "Brute Force Execution Time: " << bruteForceDuration << " microseconds\n";
    visualizeBruteForceSteps(points, bruteForceSteps);

    // Divide and Conquer Algorithm Visualization
    std::vector<DivideAndConquerStep> divideAndConquerSteps;
    auto startDivideAndConquer = std::chrono::high_resolution_clock::now(); // Start time
    std::pair<Point, Point> divideAndConquerClosestPair = closestPairDivideAndConquer(points, divideAndConquerSteps);
    auto endDivideAndConquer = std::chrono::high_resolution_clock::now();   // End time
    auto divideAndConquerDuration = std::chrono::duration_cast<std::chrono::microseconds>(endDivideAndConquer - startDivideAndConquer).count();

    std::cout << "Divide and Conquer Closest Pair: (" << divideAndConquerClosestPair.first.x << ", " << divideAndConquerClosestPair.first.y << ") and ("
        << divideAndConquerClosestPair.second.x << ", " << divideAndConquerClosestPair.second.y << ")\n";
    std::cout << "Divide and Conquer Execution Time: " << divideAndConquerDuration << " microseconds\n";
    visualizeDivideAndConquerSteps(points, divideAndConquerSteps);

    return 0;
}
