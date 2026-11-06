#pragma once

#include "GameSetting.h"
#include <SFML/Graphics.hpp>
#include<map>
#include<vector>
#include<string>
#include "GetRandom.h"

class Game;

class Pipe{
    friend class Game;
    friend class PipeController;
    protected:
        GameSettings settings;
        float gapTop;
        float gapBottom;
        sf::Sprite TopPipeCap;
        sf::Sprite TopPipeBody;
        sf::Sprite BottomPipeCap;
        sf::Sprite BottomPipeBody;

        static sf::Texture& getCapTex();
        static sf::Texture& getBodyTex();

        public:
        Pipe(GameSettings s,float Xpos);
        void move(float dt);
        std::map<std::string,std::vector<sf::FloatRect>> getBounds();
};

class PipeController{
    friend class Game;
    protected:
        GameSettings settings;
        std::vector<Pipe> pipes;
        float frontPipeX;
        float generateX;
    public:
        PipeController(GameSettings s);
        void updatePipes(float dt);
        const std::vector<Pipe>& getDrawPipes();
        void restart();
};