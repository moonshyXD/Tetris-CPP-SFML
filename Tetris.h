#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <ctime>
#include <string>
#include <chrono> 
#include "Button.h"
#include "AssetManager.h"

class Tetris
{
public:
    Tetris();
    void run();
private:
    sf::RenderWindow window;
    sf::Clock clock;
    float deltaTime;
    std::vector<std::vector<int>> matrix;
    std::vector<std::vector<std::vector<int>>> figures; // Массив фигур
    std::vector<std::vector<std::vector<int>>> figures2; // Массив некст фигур
    int currentFigure, nextFigure;
    int rotation;
    int posX, posY;
    int score1 = 0;
    bool gameover;
    float fallDelay = 0.5f; // Задержка перед падением фигуры
    float minusFallDelay = 0.1f;

    void init();
    void handleEvents();
    void update();
    void draw();
    bool checkCollision(int offsetX, int offsetY, int rotation);
    void placeFigureOnMatrix();
    void removeLines();
    void rotateFigure();
    void GameOver();

    AssetManager manager;
    Button restart = Button(sf::Vector2f(250, 620), AssetManager::GetTexture("GameFiles/restart.png"),
        AssetManager::GetTexture("GameFiles/pressedRestart.png"));
};

