#include "../headF/UiTools.h"

Button::Button(sf::Font &font, const std::string &textStr, sf::Vector2f size, sf::Vector2f pos):text(font)
{
    // 设置背景
    shape.setSize(size);
    shape.setPosition(pos);
    shape.setFillColor(sf::Color(100, 200, 100)); // 默认绿色
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::White);

    // 设置文字
    text.setFont(font);
    text.setString(textStr);
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::White);

    // 文字居中
    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin(textBounds.position + textBounds.size / 2.f);
    text.setPosition({pos.x + size.x / 2.f, pos.y + size.y / 2.f});
}

// 更新悬停状态（改变颜色）
void Button::update(sf::Vector2f mousePos)
{
    if (shape.getGlobalBounds().contains(mousePos))
    {
        shape.setFillColor(sf::Color(150, 255, 150)); // 变亮
    }
    else
    {
        shape.setFillColor(sf::Color(100, 200, 100)); // 恢复
    }
}

// 检测点击（鼠标在按钮内 + 左键按下）
bool Button::isClicked(sf::Vector2f mousePos, const sf::Event &event)
{
    if (const auto *mouseBtn = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseBtn->button == sf::Mouse::Button::Left)
        {
            if (shape.getGlobalBounds().contains(mousePos))
            {
                return true;
            }
        }
    }
    return false;
}

void Button::draw(sf::RenderWindow &window)
{
    window.draw(shape);
    window.draw(text);
}
