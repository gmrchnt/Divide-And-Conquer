#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <limits>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <functional> 
#include <SFML/Graphics.hpp>

// Point structure
struct Point {
    double x, y;
};

// Function to compute the Euclidean distance between two points
double distance(const Point& a, const Point& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

// Struct for recording steps in the brute force algorithm
struct BruteForceStep {
    Point pointA;
    Point pointB;
    double distance;
    std::pair<Point, Point> currentClosestPair;
    double currentMinDistance;
};

// Struct for recording steps in the divide and conquer algorithm
struct DivideAndConquerStep {
    Point pointA;
    Point pointB;
    double distance;
    std::pair<Point, Point> currentClosestPair;
    double currentMinDistance;
    bool showDivisionLine;
    double divisionLineX;
};

class Button {
public:
    sf::RectangleShape shape;
    sf::Text text;
    std::function<void()> onClick;

    Button() {}

    Button(float x, float y, float width, float height, const std::string& label, sf::Font& font, std::function<void()> func) {
        shape.setPosition(x, y);
        shape.setSize({ width, height });
        shape.setFillColor(sf::Color(200, 200, 200));

        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::Black);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
        text.setPosition(x + width / 2.0f, y + height / 2.0f);

        onClick = func;
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }

    bool isMouseOver(sf::RenderWindow& window) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::FloatRect bounds = shape.getGlobalBounds();
        return bounds.contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    }
};

Button playPauseButton;
