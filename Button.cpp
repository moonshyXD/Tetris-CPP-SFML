#include "Button.h"

Button::Button(sf::Vector2f location, const sf::Texture& normal, const sf::Texture& clicked)
{
    // устанавливаем текстуры для спрайтов 
    this->normal.setTexture(normal);   // нормальная кнопка
    this->clicked.setTexture(clicked); // кнопка нажата
    // устанавливаем координаты расположения кнопок
    this->normal.setPosition(location);
    this->clicked.setPosition(location);
    // присваиваем указателю нормальное состояние кнопки
    currentSpr = &this->normal;
}
bool Button::checkClick(sf::Vector2i mousePos)
{
    // Если кнопка уже нажата, то считаем, что она отпущена
    if (current)
    {
        setState(false);
        return true;
    }

    // Проверяем, было ли передано координаты курсора мыши
    if (mousePos.x >= 0)
    {
        // Проверяем, находится ли курсор внутри прямоугольника кнопки
        if (mousePos.x > currentSpr->getPosition().x &&
            mousePos.x < currentSpr->getPosition().x + currentSpr->getGlobalBounds().width &&
            mousePos.y > currentSpr->getPosition().y &&
            mousePos.y < currentSpr->getPosition().y + currentSpr->getGlobalBounds().height)
        {
            setState(true);
            return true;
        }
    }
    return false;
}

void Button::setState(bool which)
{
    current = which;
    if (current)
    {
        currentSpr = &clicked;
        return;
    }
    currentSpr = &normal;
}
sf::Sprite* Button::getSprite()
{
    return currentSpr;
}
