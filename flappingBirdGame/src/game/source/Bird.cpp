#include "../headF/Bird.h"
#include <iostream>

Bird::Bird(GameSettings s) : settings(s), shape(s.size),
 velocity(s.startSpeedY),player(birdTexture),aniJsonPath("src/game/assets/photos/birdTexture/texture.json"),
currentFrame(0),frameDuration(s.birdFrameDuration),DeadEffectTotalTime(s.deadEffectTotalTime),deadMoveDisMax(s.PipeCapWidth*2),
jumpSound(jumpSoundBuffer)
{
    auto result = birdTexture.loadFromFile("src/game/assets/photos/birdTexture/texture.png");
    if(result != 1){
        std::cerr<<"bird.png Load Fail"<<std::endl;
    }

    bool loadR = jumpSoundBuffer.loadFromFile("src/game/assets/sounds/bird_flap2.wav");
    if(!loadR)std::cout<<"jumpSoundFile load fail"<<std::endl;
    jumpSound.setBuffer(jumpSoundBuffer);

    textureData = loadFrames(aniJsonPath);
    frameW = textureData[0].size.x;
    frameH = textureData[0].size.y;

    player.setTexture(birdTexture,true);
    player.setTextureRect(textureData[currentFrame]);

    sf::FloatRect bounds = player.getLocalBounds();
    float originSize = bounds.size.x;
    float targetSize = s.size;
    float scaleFactor = targetSize/originSize;
    player.scale({scaleFactor,scaleFactor});

    //让鸟向内缩一些
    float margin = 3.f;

    this->shapeHalfW = bounds.size.x/2*scaleFactor;
    this->shapeHalfH = bounds.size.y/2*scaleFactor;
    player.setOrigin({bounds.size.x/2,bounds.size.y/2});
    player.setPosition(s.startPos);
}

void Bird::jump()
{
    velocity = settings.jumpSpeed;
    jumpSound.stop();
    playJumpSound();
}

void Bird::playJumpSound(){
    float ranNum = getRandom()*0.4+0.8;
    jumpSound.setPitch(ranNum);
    jumpSound.setVolume(300.f);
    jumpSound.play();
}

void Bird::update(float dt)
{
    float dy = dt * velocity;
    velocity += settings.gravity * dt;
    player.move({0.f, dy});

    sf::FloatRect bounds = this->getBounds();
    if (bounds.position.y<= 0)
    {
        player.move({0, - bounds.position.y});
        velocity = 0.f;
    }
    else if (bounds.position.y+bounds.size.y >= settings.windowHeight - settings.groundHeight * settings.windowHeight)
    {
        player.move({0, settings.windowHeight - settings.groundHeight * settings.windowHeight - (bounds.position.y + bounds.size.y)});
        velocity = 0.f;
    }

    if (!isDead)
    {
        float absV = std::abs(velocity);
        float upper = std::abs(settings.jumpSpeed);
        if (velocity > 0)
        {
            upper = std::abs(settings.maxDownV);
        }
        float ratio = (absV) / upper;
        ratio = std::min(1.f, ratio);
        float angleMax = settings.maxElevaDown;
        if (velocity < 0)
        {
            angleMax = settings.maxElevaUp;
        }
        float rotation = angleMax * ratio;
        sf::Angle a = sf::degrees(rotation);
        player.setRotation(a);
    }
}

void Bird::dead()
{
    this->isDead = true;
    this->effectPlayTime = 0.f;
    this->Flashing = true;

}

void Bird::restart()
{
    velocity = settings.startSpeedY;
    player.setPosition(settings.startPos);
    this->isDead = false;
}

sf::FloatRect Bird::getBounds()
{
    sf::FloatRect bounds = player.getGlobalBounds();
    float margin = 3;
    bounds.position.x += margin;
    bounds.position.y += margin;
    bounds.size.x -= 2*margin;
    bounds.size.y -= 2*margin;
    return bounds;
}

void Bird::animate(){
    if(animClock.getElapsedTime().asSeconds()>=frameDuration){
        animClock.restart();
        currentFrame = (currentFrame+1)%textureData.size();
        player.setTextureRect(textureData[currentFrame]);
    }
}

void Bird::deadColorChange(float dt)
{
    if (isDead && Flashing)
    {
        effectPlayTime += dt;
        flashTime -= dt;
        if (flashTime < 0)
        {
            flashTime = flashInterVal;
            if (player.getColor() == sf::Color::Red)
            {
                player.setColor(sf::Color::White);
            }
            else if (player.getColor() == sf::Color::White)
            {
                player.setColor(sf::Color::Red);
            }
        }
        if (effectPlayTime >= settings.deadEffectTotalTime)
        {
            Flashing = false;
            effectPlayTime = 0.f;
            flashTime = flashInterVal;
            player.setColor(sf::Color::White);
        }
    }
}

void Bird::deadEffect(float deltaTime){
    float dt = deltaTime;
    sf::FloatRect bounds = player.getGlobalBounds();
    // deadColorChange(deltaTime);

    float degree = player.getRotation().asDegrees();
    if(isDead && degree!=280.f && bounds.position.y +bounds.size.y<=settings.windowHeight*(1-settings.groundHeight-0.001)){
        float rotation = deltaTime*90.f;
        if( degree > 280.f){
            player.rotate(sf::degrees(-1*rotation));
            if(player.getRotation().asDegrees() < 280.f){
                player.setRotation(sf::degrees(280.f));
            }
        }

        else if(degree < 280.f){
            float dDegree = 280.f - degree;
            if(dDegree>=180.f){
                player.rotate(sf::degrees(-1*rotation));
            }
            else{
                player.rotate(sf::degrees(rotation));
            }
            if(player.getRotation().asDegrees() < 280.f && player.getRotation().asDegrees()>90.f){
                player.setRotation(sf::degrees(280.f));
            }
        }
    }
    deadMove(deltaTime);
}


void Bird::deadMove(float deltaTime)
{
    if (this->DeadMoveX)
    {
        float dx = -1 * deltaTime * (deadMoveDis / fallTime);
        this->currentDeadMoveX += -1 * dx;
        player.move({dx, 0});
        if (currentDeadMoveX >= deadMoveDis)
        {
            player.move({currentDeadMoveX - deadMoveDis, 0});
            this->DeadMoveX = false;
            currentDeadMoveX = 0;
            fallTime = 0;
        }
    }
}
