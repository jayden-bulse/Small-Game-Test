#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>
#include "../include/json.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include<string>
#include "GetRandom.h"

struct GameSettings
{
    // 屏幕设置
    unsigned int windowWidth = 960;
    unsigned int windowHeight = 1400;
    sf::Color backgroundColor{255, 255, 255};
    unsigned int maxFPS = 60;
    // 鸟
    float startSpeedY = 10.f;
    float gravity = 2200.f;
    float jumpFallTime = 0.32;
    float jumpSpeed = gravity * jumpFallTime * -1;
    float size = 50.f;
    sf::Vector2f startPos{200.f, 200.f};
    float maxElevaUp = -30.f;
    float maxElevaDown = 80.f;
    float maxDownV = std::pow(2*windowHeight*gravity,0.5);
    float birdFrameDuration = 1.0/24.0;
    float deadEffectTotalTime = 0.5f;
    float deadJumpSpeed = -500.f;
    // 管道设置
    float groundHeight = 0.10;
    sf::Color groundColor{0,47,171};
    sf::Color pipeColor{12,167,21};
    float pipeGap = 5.5*size;
    float gapTopMin = 0.15;
    float gapTopMax = 1-groundHeight-(pipeGap/windowHeight)*2;
    float pipeWidth = 70.f;
    float pipeMoveSpeed = -350.f;
    float generateDistance = windowWidth*0.7;
    float PipeCapWidth = pipeWidth*1.1;
    int updateDis = 10;

    //场景设置
    float sceneMoveSpeed = pipeMoveSpeed*0.25;

    //碰撞效果设置
    float totalTime = 0.2f;
    float collisionSoundLength = 1.f;
};


std::vector<sf::IntRect> loadFrames(const std::string& jsonPath);