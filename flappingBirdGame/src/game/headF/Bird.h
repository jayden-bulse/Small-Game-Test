#pragma once
#include "./GameSetting.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class Game;

class Bird
{
    friend class Game;
private:
    GameSettings settings;
    sf::CircleShape shape;
    float velocity;
    bool isDead = false;
    sf::Texture birdTexture;
    sf::Sprite player;
    float shapeHalfW;
    float shapeHalfH;
    std::string aniJsonPath;
    std::vector<sf::IntRect> textureData;
    float frameW;
    float frameH;
    sf::Clock animClock;
    int currentFrame;
    float frameDuration;   
    bool Flashing = false; 
    float DeadEffectTotalTime;
    float effectPlayTime = 0;
    float flashInterVal = 0.1f;
    float flashTime = 0.1f;

    float deadMoveDis;
    float deadMoveDisMax;
    float currentDeadMoveX = 0;
    float fallTime = 0;
    bool  DeadMoveX = false;

    bool isPlayJumpSound = false;
    sf::SoundBuffer jumpSoundBuffer;
    sf::Sound jumpSound;
public:
    explicit Bird(GameSettings s);

protected:
    void jump();
    void update(float dt);
    void dead();
    void restart();
    sf::FloatRect getBounds();
    void animate();
    void deadEffect(float deltaTime);
    void deadMove(float deltaTime);
    void deadColorChange(float dt);
    void playJumpSound();
};