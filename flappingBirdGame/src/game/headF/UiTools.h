#pragma
#include <SFML/Graphics.hpp>
#include <functional>
#include <string>

class Button {
public:
    Button(sf::Font& font, const std::string& textStr, sf::Vector2f size, sf::Vector2f pos);
    // 更新悬停状态（改变颜色）
    void update(sf::Vector2f mousePos);
    // 检测点击（鼠标在按钮内 + 左键按下）
    bool isClicked(sf::Vector2f mousePos, const sf::Event& event);
    void draw(sf::RenderWindow& window);
private:
    sf::RectangleShape shape;
    sf::Text text;
};