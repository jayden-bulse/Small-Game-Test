#pragma once
#include<SFML/Graphics.hpp>
#include"GameSetting.h"
#include "UiTools.h"

class Game;
class GameUI{
    friend class Game;
    private:
    //通过管道计分
    sf::Font font;
    sf::Text scoreText;
    sf::Text bestText;
    sf::Text actionBtn;
    
    sf::Text returnActionBtn;
    sf::Text exitBtn;
    sf::Text deadTip;
    
    int score = 0;
    int bestScore = 0;
    GameSettings setting;
    public:
    GameUI(sf::RenderWindow& window,GameSettings s);
    void drawScore(sf::RenderTarget& target);
    void drawRest(sf::RenderTarget& target);
    void drawUnrest(sf::RenderTarget& target);
    bool isClickPlay(sf::Vector2f mousePos,const sf::Event& event);
    bool isClickRePlay(sf::Vector2f mousePos,const sf::Event& event);
    bool checkButtonPress(sf::Vector2f mousePos,const sf::Event& event,sf::Text text);

    void update(int score,int bestScore);
    void restart();
void setText(sf::Text& text,float size,std::string s,float x = -1,float y =-1);
void drawDead(sf::RenderTarget& target);
bool isClickExit(sf::Vector2f mousePos, const sf::Event &event);
};