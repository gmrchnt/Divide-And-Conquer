#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>

// Structure for each node in the visualization

std::atomic<long long> totalComputationTime(0);

struct Node {
    int x, y;
    long long result;
    sf::Vector2f position;
    sf::CircleShape circle;
    sf::Text text;
};

// Structure for a connection between nodes
struct Connection {
    sf::Vertex line[2];
};

// Function to create a visual representation of a node
Node createVisualNode(const sf::Font& font, int x, int y, long long result, sf::Vector2f position) {
    Node node;
    node.x = x;
    node.y = y;
    node.result = result;
    node.position = position;

    node.circle.setRadius(40.f);
    node.circle.setFillColor(sf::Color::Cyan);
    node.circle.setOutlineThickness(2.f);
    node.circle.setOutlineColor(sf::Color::Black);
    node.circle.setOrigin(node.circle.getRadius(), node.circle.getRadius());
    node.circle.setPosition(position);

    node.text.setFont(font);
    node.text.setString("x=" + std::to_string(x) + "\ny=" + std::to_string(y) + "\nres=" + std::to_string(result));
    node.text.setCharacterSize(14);
    node.text.setFillColor(sf::Color::Black);
    sf::FloatRect textBounds = node.text.getLocalBounds();
    node.text.setOrigin(textBounds.width / 2, textBounds.height / 2);
    node.text.setPosition(position);

    return node;
}

// Recursive function to compute and visualize Karatsuba multiplication
void renderKaratsubaTree(int x, int y, int depth, int maxDepth, sf::Font& font,
    sf::Vector2f position, float horizontalSpacing, float verticalSpacing,
    std::vector<Node>& nodes, std::vector<Connection>& connections,
    sf::RenderWindow& window, sf::View& view) {
    if (depth > maxDepth || x < 10 || y < 10) {
        // Base case: Multiply directly and measure time
        auto start = std::chrono::high_resolution_clock::now();
        long long result = x * y;
        auto end = std::chrono::high_resolution_clock::now();

        // Calculate elapsed time in microseconds
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        std::cout << "Multiplication " << x << " * " << y << " took " << duration << " microseconds.\n";
        totalComputationTime += duration; // Accumulate the time

        nodes.push_back(createVisualNode(font, x, y, result, position));

        // Render the current state
        window.clear(sf::Color::White);
        window.setView(view);
        for (const auto& conn : connections) window.draw(conn.line, 2, sf::Lines);
        for (const auto& node : nodes) {
            window.draw(node.circle);
            window.draw(node.text);
        }
        window.display();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        return;
    }

    // Divide x and y
    int n = std::max(std::to_string(x).length(), std::to_string(y).length());
    int m = n / 2;

    int highX = x / static_cast<int>(std::pow(10, m));
    int lowX = x % static_cast<int>(std::pow(10, m));
    int highY = y / static_cast<int>(std::pow(10, m));
    int lowY = y % static_cast<int>(std::pow(10, m));

    // Positions for child nodes
    sf::Vector2f z0Pos = position + sf::Vector2f(-horizontalSpacing, verticalSpacing);
    sf::Vector2f z1Pos = position + sf::Vector2f(0.f, verticalSpacing);
    sf::Vector2f z2Pos = position + sf::Vector2f(horizontalSpacing, verticalSpacing);

    // Recursive calls
    renderKaratsubaTree(lowX, lowY, depth + 1, maxDepth, font, z0Pos, horizontalSpacing / 2, verticalSpacing, nodes, connections, window, view);
    renderKaratsubaTree(highX + lowX, highY + lowY, depth + 1, maxDepth, font, z1Pos, horizontalSpacing / 2, verticalSpacing, nodes, connections, window, view);
    renderKaratsubaTree(highX, highY, depth + 1, maxDepth, font, z2Pos, horizontalSpacing / 2, verticalSpacing, nodes, connections, window, view);

    // Calculate the result and time the computation
    auto start = std::chrono::high_resolution_clock::now();
    long long z0 = lowX * lowY;
    long long z1 = (highX + lowX) * (highY + lowY);
    long long z2 = highX * highY;
    long long result = z2 * static_cast<long long>(std::pow(10, 2 * m)) + (z1 - z2 - z0) * static_cast<long long>(std::pow(10, m)) + z0;
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate elapsed time in microseconds
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Karatsuba step (x=" << x << ", y=" << y << ") took " << duration << " microseconds.\n";
    totalComputationTime += duration;
    // Add the parent node
    Node parentNode = createVisualNode(font, x, y, result, position);
    nodes.push_back(parentNode);

    // Add connections
    connections.push_back({ { { position, sf::Color::Black }, { z0Pos, sf::Color::Black } } });
    connections.push_back({ { { position, sf::Color::Black }, { z1Pos, sf::Color::Black } } });
    connections.push_back({ { { position, sf::Color::Black }, { z2Pos, sf::Color::Black } } });

    // Render the current state
    window.clear(sf::Color::White);
    window.setView(view);
    for (const auto& conn : connections) window.draw(conn.line, 2, sf::Lines);
    for (const auto& node : nodes) {
        window.draw(node.circle);
        window.draw(node.text);
    }
    window.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

int main() {
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Karatsuba Algorithm Visualizer");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("D:/k224550DAA_Project/visualizer/Project1/Fonts/TT Rounds Neue Trial Regular.ttf")) {
        std::cerr << "Failed to load font.\n";
        return -1;
    }

    sf::RectangleShape inputBox1(sf::Vector2f(200, 30));
    inputBox1.setPosition(10, 10);
    inputBox1.setFillColor(sf::Color::White);
    inputBox1.setOutlineThickness(2);
    inputBox1.setOutlineColor(sf::Color::Black);

    sf::RectangleShape inputBox2(sf::Vector2f(200, 30));
    inputBox2.setPosition(220, 10);
    inputBox2.setFillColor(sf::Color::White);
    inputBox2.setOutlineThickness(2);
    inputBox2.setOutlineColor(sf::Color::Black);

    sf::RectangleShape button(sf::Vector2f(100, 30));
    button.setPosition(430, 10);
    button.setFillColor(sf::Color(100, 200, 100));
    button.setOutlineThickness(2);
    button.setOutlineColor(sf::Color::Black);

    sf::Text buttonText("Visualize", font, 14);
    buttonText.setPosition(440, 15);
    buttonText.setFillColor(sf::Color::Black);

    sf::Text inputText1("", font, 14);
    inputText1.setPosition(15, 15);
    inputText1.setFillColor(sf::Color::Black);

    sf::Text inputText2("", font, 14);
    inputText2.setPosition(225, 15);
    inputText2.setFillColor(sf::Color::Black);

    // Input State
    bool typing1 = false, typing2 = false;
    std::string input1 = "", input2 = "";

    // Containers for tree visualization
    std::vector<Node> nodes;
    std::vector<Connection> connections;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized) {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (inputBox1.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    typing1 = true;
                    typing2 = false;
                }
                else if (inputBox2.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    typing1 = false;
                    typing2 = true;
                }
                else if (button.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
                    if (!input1.empty() && !input2.empty()) {
                        int x = std::stoi(input1);
                        int y = std::stoi(input2);
                        nodes.clear();
                        connections.clear();
                        sf::View view = window.getDefaultView(); // Create a mutable copy of the default view
                        totalComputationTime = 0;
                        renderKaratsubaTree(x, y, 0, 4, font, { 640, 100 }, 300.f, 150.f, nodes, connections, window, view);
                        std::cout << "Total computation time: " << totalComputationTime << " microseconds.\n";
                    }
                }
                else {
                    typing1 = typing2 = false;
                }
            }

            if (event.type == sf::Event::TextEntered) {
                if (typing1 && event.text.unicode >= 32 && event.text.unicode <= 126)
                    input1 += static_cast<char>(event.text.unicode);
                else if (typing2 && event.text.unicode >= 32 && event.text.unicode <= 126)
                    input2 += static_cast<char>(event.text.unicode);
            }
        }

        window.clear(sf::Color::White);
        window.draw(inputBox1);
        window.draw(inputBox2);
        window.draw(button);
        window.draw(buttonText);
        inputText1.setString(input1);
        inputText2.setString(input2);
        window.draw(inputText1);
        window.draw(inputText2);
        window.display();
    }

    return 0;
}