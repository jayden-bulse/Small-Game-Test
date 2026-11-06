#pragma once
#include "./GameSetting.h"
#include <SFML/Graphics.hpp>

class Game;

class Scene{
    friend class Game;

    protected:
    sf::Sprite backGround1;
    sf::Sprite backGround2;
    sf::Sprite ground;
    sf::Sprite ground1;
    GameSettings settings;
    public:
    static sf::Texture& getSceneTex1();
    static sf::Texture& getSceneTex2();
    void sceneMove(float dt);
    static sf::Texture& getGroundTex();
    Scene(GameSettings s);
    void drawScene(sf::RenderTarget& target);
    void stitchBackGround();
    void groundMove(float dt);
};