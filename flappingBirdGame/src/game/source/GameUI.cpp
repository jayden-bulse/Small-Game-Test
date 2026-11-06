#include "../headF/GameUI.h"
#include <iostream>

GameUI::GameUI(sf::RenderWindow& window,GameSettings s) : scoreText(font),bestText(font),
setting(s),actionBtn(font),returnActionBtn(font),deadTip(font),exitBtn(font)
{
if(!font.openFromFile("src/game/assets/DMMono-Regular.ttf")){
   std::cerr << "UI Font Load Fail" << std::endl;
}
    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::Red);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(2.f);
    scoreText.setString("Score:0");

    bestText.setFont(font);
    bestText.setCharacterSize(24);
    bestText.setFillColor(sf::Color::Red);
    bestText.setOutlineColor(sf::Color::Black);
    bestText.setOutlineThickness(2.f);
    bestText.setString("Best Score:0");
    sf::FloatRect bestBounds = bestText.getLocalBounds();
    bestText.setOrigin({bestBounds.position.x + bestBounds.size.x/2,bestBounds.position.y+bestBounds.size.y/2.0f});
    bestText.setPosition({s.windowWidth- bestBounds.size.x,30.f});
    //确定scoreText显示位置
    sf::FloatRect bounds = scoreText.getLocalBounds();
    scoreText.setOrigin({bounds.position.x + bounds.size.x/2,
    bounds.position.y+bounds.size.y/2.0f});
    scoreText.setPosition({s.windowWidth/2,30.f});

    actionBtn.setFont(font);
    actionBtn.setCharacterSize(24);
    actionBtn.setFillColor(sf::Color::Red);
    actionBtn.setOutlineColor(sf::Color::Black);
    actionBtn.setOutlineThickness(2.f);
    actionBtn.setString("Play");
sf::FloatRect ab = actionBtn.getLocalBounds();
actionBtn.setOrigin({ab.position.x + ab.size.x / 2.f,     // ★ 居中原点
                     ab.position.y + ab.size.y / 2.f});
actionBtn.setPosition({s.windowWidth / 2.f, s.windowHeight * 0.55f});   // ★ 屏幕中下方

    setText(deadTip,60,"You Dead",0.5,0.48-0.1);
    
    setText(returnActionBtn,30,"Return Play",0.5,0.55-0.1);
    returnActionBtn.setFillColor(sf::Color(0,210,50));
    setText(exitBtn,30,"Exit",0.5,0.62-0.1);
    exitBtn.setFillColor(sf::Color(0,210,50));
}

void GameUI::setText(sf::Text& text,float size,std::string s,float x,float y){
    text.setFont(font);
    text.setCharacterSize(size);
    text.setFillColor(sf::Color::Red);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(2.f);
    text.setString(s);
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x+bounds.size.x/2,bounds.position.y+bounds.size.y/2});
    if(x!=-1 && y!=-1){
        text.setPosition({setting.windowWidth*x,setting.windowHeight*y});
    }
}

void GameUI::drawScore(sf::RenderTarget& target){
    target.draw(scoreText);
    target.draw(bestText);
}

void GameUI::drawUnrest(sf::RenderTarget& target){
    drawScore(target);
}

void GameUI::drawDead(sf::RenderTarget& target){
    target.draw(deadTip);
    target.draw(returnActionBtn);
    target.draw(exitBtn);
}

void GameUI::drawRest(sf::RenderTarget& target){
    target.draw(actionBtn);
}

void GameUI::restart(){
    this->score = 0;
    scoreText.setString("Score:0"); 
}

void GameUI::update(int score,int bestScore){
    if(bestScore!=0){
        this->bestScore = bestScore;
        std::string BshowString = "Best Score:"+std::to_string(bestScore);
        bestText.setString(BshowString);
    }
    this->score = score;
    std::string showString = "Score:" + std::to_string(score);
    scoreText.setString(showString);
}

bool GameUI::isClickPlay(sf::Vector2f mousePos, const sf::Event &event)
{
    return checkButtonPress(mousePos,event,actionBtn);
}

bool GameUI::isClickRePlay(sf::Vector2f mousePos, const sf::Event &event)
{
    return checkButtonPress(mousePos,event,returnActionBtn);
}

bool GameUI::isClickExit(sf::Vector2f mousePos, const sf::Event &event)
{
    return checkButtonPress(mousePos,event,exitBtn);    
}

bool GameUI::checkButtonPress(sf::Vector2f mousePos, const sf::Event &event,sf::Text text){
       if (const auto *mouseBtn = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseBtn->button == sf::Mouse::Button::Left)
        {
            if (text.getGlobalBounds().contains(mousePos))
            {
                return true;
            }
        }
    }
    return false;
}
