#include "../headF/effect.h"
#include <iostream>
sf::Texture& impactEffect::getEffectTex(){
    static sf::Texture tex;
    static bool isLoad = tex.loadFromFile("src/game/assets/photos/hit_spark2.png");
    if(!isLoad){std::cerr<<"hit_spark.png Load Fail"<<std::endl;}
    return tex;
}

impactEffect::impactEffect(GameSettings s):totalTime(s.totalTime),effect(getEffectTex()),frameQuntity(5),frameDuration(totalTime/(frameQuntity)){
    effect.setTexture(getEffectTex(),true);
    sf::FloatRect bounds = effect.getLocalBounds();
    frameW = bounds.size.x/frameQuntity;
    frameH = bounds.size.y/2.0;
    
    currentFrame = 0;

    effect.setTextureRect({{0,0},{frameW,frameH}});
    // float scaleW = tBounds.size.x*1.2/frameW;
    // float scaleH = tBounds.size.y*1.2/frameH;
    // effect.setScale({scaleW,scaleH});
    // effect.setPosition({tBounds.position.x,tBounds.position.y});
}

bool impactEffect::getPlayDone(){
    return playDone;
}

void impactEffect::setPlayDone(bool p){
    this->playDone = p;
}

sf::Sprite& impactEffect::getDrawObj()const{
    return const_cast<sf::Sprite&>(effect);
}

void impactEffect::playEffect(sf::FloatRect tBounds, float deltaTime)
{
    if (!playDone)
    {
        timer += deltaTime;
        currentTime += deltaTime;
        float scaleW = tBounds.size.x * 3 / frameW;
        float scaleH = tBounds.size.y * 3 / frameH;
        effect.setScale({scaleW, scaleH});
        sf::FloatRect effBounds = effect.getGlobalBounds();
        float offsetX = (tBounds.size.x - effBounds.size.x)/2.0;
        float offsetY = (tBounds.size.y - effBounds.size.y)/2.0;
        effect.setPosition({tBounds.position.x+offsetX, tBounds.position.y+offsetY});

        if (timer >= frameDuration)
        {
            currentFrame = (currentFrame + 1) % frameQuntity;
            effect.setTextureRect({{frameW * currentFrame, 0}, {frameW, frameH}});
            timer = 0;
        }
        if(currentTime>=totalTime){
            playDone = true;
            currentTime = 0;
            timer = 0;
            currentFrame = 0;            
        }
    }
}