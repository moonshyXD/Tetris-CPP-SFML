#include "Tetris.h"

Tetris::Tetris() : window(sf::VideoMode(800, 1000), "Tetris"), deltaTime(0), gameover(false), fallDelay(0.5f) {
    matrix = std::vector<std::vector<int>>(20, std::vector<int>(10, 0)); // Игровая матрица 10x20
    currentFigure = std::rand() % 7; // Начальная случайная фигура
    nextFigure = std::rand() % 7;
    rotation = 0;
    posX = 4;
    posY = 0;

    // Инициализация фигур (квадрат, колонна, Т-образная, L-образная)
    figures = {
        {{1, 1}, {1, 1}},   // Квадрат
        {{1}, {1}, {1}, {1}},   // Колонна
        {{1, 1, 1}, {0, 1, 0}}, // T-образная
        {{1, 0}, {1, 0}, {1, 1}}, // L-образная
        {{ 1, 1 }, {1, 0}, {1, 0}}, // Г-образная
        {{ 0, 1 }, {1, 1}, {1, 0}}, // Змейка
        {{ 1, 0 }, {1, 1}, {0, 1}} // Обратная змейка
    };
    // для некст фигур
    figures2 = {
        {{1, 1}, {1, 1}},   // Квадрат
        {{1}, {1}, {1}, {1}},   // Колонна
        {{1, 1, 1}, {0, 1, 0}}, // T-образная
        {{1, 0}, {1, 0}, {1, 1}},// L-образная
        {{ 1, 1 }, {1, 0}, {1, 0}}, // Г-образная
        {{ 0, 1 }, {1, 1}, {1, 0}}, // Змейка
        {{ 1, 0 }, {1, 1}, {0, 1}} // Обратная змейка
    };
}

void Tetris::run() {
    init();
    while (window.isOpen()) {
        handleEvents();
        update();
        draw();
    }
}

void Tetris::GameOver() {
    window.clear(sf::Color::Black);
    // установка шрифта
    sf::Font font;
    if (!font.loadFromFile("GameFiles/Metropolian-Display.ttf"))
        throw("COULD NOT LOAD FONT!");

    // установка текста
    sf::Text text2;
    text2.setFont(font);
    text2.setCharacterSize(50);
    text2.setFillColor(sf::Color::Red);
    text2.setStyle(sf::Text::Bold);
    text2.setPosition(200, 400);
    text2.setString("GAME OVER!");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold);
    text.setPosition(260, 500);
    text.setString("YOUR SCORE: ");
    sf::Text score;
    score.setFont(font);
    score.setCharacterSize(30);
    score.setFillColor(sf::Color::Red);
    score.setStyle(sf::Text::Bold);
    score.setPosition(375, 550);
    std::string str_score = std::to_string(score1);
    score.setString(str_score);

    // кнопка рестарта
    if (restart.checkClick(sf::Mouse::getPosition()))
    {
        gameover = false;
        for (int i = 0; i < 20; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                matrix[i][j] = 0;
            }
        }
        minusFallDelay = 0.1f;
        score1 = 0;
        run();
    }

    window.draw(*restart.getSprite());

    window.draw(text2);
    window.draw(text);
    window.draw(score);
}

void Tetris::init() {
    std::srand(std::time(0));
    window.setFramerateLimit(144);
}

void Tetris::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
        {
            restart.checkClick(sf::Mouse::getPosition(window));
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Left) {
                if (!checkCollision(-1, 0, rotation))
                    posX--;
            }
            if (event.key.code == sf::Keyboard::Right) {
                if (!checkCollision(1, 0, rotation))
                    posX++;
            }
            if (event.key.code == sf::Keyboard::Down) {
                if (!checkCollision(0, 1, rotation))
                    posY++;
            }
            if (event.key.code == sf::Keyboard::Q) { 
                rotateFigure();
            }
            if (event.key.code == sf::Keyboard::Space) {
                while (!checkCollision(0, 1, rotation))
                    posY++;
            }
        }
    }
}

void Tetris::update() {
    deltaTime = clock.restart().asSeconds();
    fallDelay -= deltaTime; // Уменьшаем задержку каждый кадр
    if (fallDelay <= 0) { // Если время задержки истекло
        if (!checkCollision(0, 1, rotation))
            posY++;
        else {
            placeFigureOnMatrix();
            removeLines();
            removeLines();
            removeLines();
            removeLines();
            posX = 4;
            posY = 0;
            rotation = 0;
            currentFigure = nextFigure;
            nextFigure = std::rand() % 7;
            for (int i = 0; i < 10; i++)
            {
                if (matrix[0][i] == 1) {
                    gameover = true;
                }
            }
        }
        fallDelay = 0.5f - minusFallDelay; // Уменьшаем задержку для ускорения падения фигуры
    }
}

void Tetris::draw() {
    window.clear(sf::Color::Black);

    // Отрисовка фигуры
    for (int i = 0; i < figures[currentFigure].size(); ++i) {
        for (int j = 0; j < figures[currentFigure][i].size(); ++j) {
            if (figures[currentFigure][i][j] == 1) {
                sf::RectangleShape block(sf::Vector2f(50, 50));
                block.setFillColor(sf::Color::Yellow);
                block.setPosition((posX + j) * 50, (posY + i) * 50);
                window.draw(block);
            }
        }
    }
    // установка шрифта
    sf::Font font;
    if (!font.loadFromFile("GameFiles/Metropolian-Display.ttf"))
        throw("COULD NOT LOAD FONT!");

    // установка текста
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::Red);
    text.setStyle(sf::Text::Bold);
    text.setPosition(525, 100);
    text.setString("YOUR SCORE: ");
    sf::Text score;
    score.setFont(font);
    score.setCharacterSize(30);
    score.setFillColor(sf::Color::Red);
    score.setStyle(sf::Text::Bold);
    score.setPosition(625, 150);
    std::string str_score = std::to_string(score1);
    score.setString(str_score);
    window.draw(text);
    window.draw(score);

    // отрсиовка игрового поля
    sf::RectangleShape square(sf::Vector2f(50, 50));
    square.setFillColor(sf::Color::Transparent);
    square.setOutlineColor(sf::Color::White);
    square.setOutlineThickness(1);
    square.setPosition(2, 2);
    for (int i = 0; i < 500; i += 50)
    {
        for (int j = 0; j <= 1000; j += 50)
        {
            square.setPosition(i, j);
            window.draw(square);
        }
    }
    // Отрисовка игрового поля x2
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] == 1) {
                sf::RectangleShape block(sf::Vector2f(50, 50));
                block.setFillColor(sf::Color::Blue);
                block.setPosition(j * 50, i * 50);
                window.draw(block);
            }
        }
    }

        //отрисовка следующей фигуры
    sf::Text nextFigureText;
    nextFigureText.setFont(font);
    nextFigureText.setCharacterSize(29);
    nextFigureText.setFillColor(sf::Color::Red);
    nextFigureText.setStyle(sf::Text::Bold);
    nextFigureText.setPosition(525, 565);
    nextFigureText.setString("NEXT FIGURE");
    window.draw(nextFigureText);
    sf::RectangleShape transSquare(sf::Vector2f(225, 225));
    transSquare.setFillColor(sf::Color::Transparent);
    transSquare.setOutlineColor(sf::Color::White);
    transSquare.setOutlineThickness(10);
    transSquare.setPosition(540, 640);
    window.draw(transSquare);
    for (int i = 0; i < figures2[nextFigure].size(); ++i) {
        for (int j = 0; j < figures2[nextFigure][i].size(); ++j) {
            if (figures2[nextFigure][i][j] == 1) {
                sf::RectangleShape block2(sf::Vector2f(50, 50));
                block2.setFillColor(sf::Color::Blue);
                switch (nextFigure) {
                case 0:
                    block2.setPosition((12.1 + j) * 50, (14 + i) * 50);
                    break;
                case 1:
                    block2.setPosition((12.5 + j) * 50, (13 + i) * 50);
                    break;
                case 2:
                    block2.setPosition((11.6 + j) * 50, (14.1 + i) * 50);
                    break;
                case 3:
                    block2.setPosition((12.2 + j) * 50, (13.5 + i) * 50);
                    break;
                case 4:
                    block2.setPosition((12.2 + j) * 50, (13.5 + i) * 50);
                    break;
                case 5:
                    block2.setPosition((12.2 + j) * 50, (13.5 + i) * 50);
                    break;
                case 6:
                    block2.setPosition((12.2 + j) * 50, (13.5 + i) * 50);
                    break;
                }
                window.draw(block2);
            }
        }
    }

    if (gameover)
        GameOver();

    window.display();
}

bool Tetris::checkCollision(int offsetX, int offsetY, int newRotation) {
    for (int i = 0; i < figures[currentFigure].size(); ++i) {
        for (int j = 0; j < figures[currentFigure][i].size(); ++j) {
            if (figures[currentFigure][i][j] == 1) {
                int x = posX + j + offsetX;
                int y = posY + i + offsetY;
                if (x < 0 || x >= 10 || y >= 20 || (y >= 0 && matrix[y][x] == 1))
                    return true;
            }
        }
    }
    return false;
}

void Tetris::placeFigureOnMatrix() {
    for (int i = 0; i < figures[currentFigure].size(); ++i) {
        for (int j = 0; j < figures[currentFigure][i].size(); ++j) {
            if (figures[currentFigure][i][j] == 1) {
                matrix[posY + i][posX + j] = 1;
            }
        }
    }
}

void Tetris::removeLines() {
    for (int i = matrix.size() - 1; i >= 0; --i) {
        bool fullLine = true;
        for (int j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] == 0) {
                fullLine = false;
                break;
            }
        }
        if (fullLine) {
            matrix.erase(matrix.begin() + i);
            matrix.insert(matrix.begin(), std::vector<int>(10, 0));
            score1 += 100;
            if (minusFallDelay <= 0.4f)
                minusFallDelay += 0.005;
        }
    }
}

void Tetris::rotateFigure() {
    // Создаем новую матрицу для повернутой фигуры
    std::vector<std::vector<int>> newFigure(figures[currentFigure][0].size(), std::vector<int>(figures[currentFigure].size()));
    for (size_t i = 0; i < figures[currentFigure].size(); ++i) {
        for (size_t j = 0; j < figures[currentFigure][i].size(); ++j) {
            newFigure[j][figures[currentFigure].size() - 1 - i] = figures[currentFigure][i][j];
        }
    }

    // Проверяем, не выходит ли фигура за пределы поля после поворота
    int newPosX = posX;
    if (newPosX + newFigure[0].size() > 10) { // Если фигура выходит за правую границу поля
        newPosX = 10 - newFigure[0].size(); // Корректируем позицию по оси X
    }

        // Проверяем, есть ли пространство для размещения повернутой фигуры
    bool canRotate = true;
    for (int i = 0; i < newFigure.size(); ++i) {
        for (int j = 0; j < newFigure[i].size(); ++j) {
            if (newFigure[i][j] == 1 && matrix[posY + i][newPosX + j] == 1) {
                // Если есть пересечения с другими блоками в игровом поле
                canRotate = false;
                break;
            }
        }
        if (!canRotate) {
            break;
        }
    }

    // Если поворот возможен, обновляем позицию и матрицу текущей фигуры
    if (canRotate) {
        posX = newPosX;
        figures[currentFigure] = newFigure;
    }
}