#include "Button.h"

Button::Button(sf::Vector2f location, const sf::Texture& normal, const sf::Texture& clicked)
{
    // ������������� �������� ��� �������� 
    this->normal.setTexture(normal);   // ���������� ������
    this->clicked.setTexture(clicked); // ������ ������
    // ������������� ���������� ������������ ������
    this->normal.setPosition(location);
    this->clicked.setPosition(location);
    // ����������� ��������� ���������� ��������� ������
    currentSpr = &this->normal;
}
bool Button::checkClick(sf::Vector2i mousePos)
{
    // ���� ������ ��� ������, �� �������, ��� ��� ��������
    if (current)
    {
        setState(false);
        return true;
    }

    // ���������, ���� �� �������� ���������� ������� ����
    if (mousePos.x >= 0)
    {
        // ���������, ��������� �� ������ ������ �������������� ������
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
