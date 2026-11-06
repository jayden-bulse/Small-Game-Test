#include "../headF/PipeAPipeController.h"
#include <vector>
#include <iostream>

sf::Texture& Pipe::getCapTex(){
    static sf::Texture tex;
    static bool loaded = tex.loadFromFile("src/game/assets/photos/PipeCap.png");
    if (!loaded) std::cerr << "PipeCap.png Load Fail" << std::endl;
    return tex;
}

sf::Texture& Pipe::getBodyTex(){
    static sf::Texture tex;
    static bool loaded = tex.loadFromFile("src/game/assets/photos/PipeBody.png");
    if (!loaded) std::cerr << "PipeBody.png Load Fail" << std::endl;
    return tex;
}

Pipe::Pipe(GameSettings s, float Xpos) : settings(s),TopPipeCap(getCapTex()),TopPipeBody(getBodyTex()),BottomPipeCap(getCapTex()),BottomPipeBody(getBodyTex())
{
    this->gapTop = getRandom() * (s.gapTopMax - s.gapTopMin) + s.gapTopMin;
    this->gapBottom = gapTop + s.pipeGap / s.windowHeight;
    //加载管道图片注入Sprite
   
    TopPipeCap.setTexture(getCapTex(), true);
    TopPipeBody.setTexture(getBodyTex(), true);
    BottomPipeCap.setTexture(getCapTex(), true);
    BottomPipeBody.setTexture(getBodyTex(), true);

    sf::FloatRect CapBounds = TopPipeCap.getLocalBounds();
    float capFactor = s.PipeCapWidth / CapBounds.size.x;
    TopPipeCap.setScale({capFactor,capFactor});
    BottomPipeCap.setScale({capFactor,capFactor});
    
    //设置大小
    sf::FloatRect BodyBounds = TopPipeBody.getLocalBounds();
    float oriBodyW = BodyBounds.size.x;
    float oriBodyH = BodyBounds.size.y;
    float TargetW = s.pipeWidth;
    float TTargetH = s.windowHeight*(gapTop)-TopPipeCap.getGlobalBounds().size.y;
    float BodyFactorW = TargetW/oriBodyW;
    float TBodyFactorH = TTargetH/oriBodyH;
    TopPipeBody.setScale({BodyFactorW,TBodyFactorH});
    
    float BTargetH = s.windowHeight*(1 - gapBottom - s.groundHeight)-BottomPipeCap.getGlobalBounds().size.y;
    float BBodyFactorH = BTargetH/oriBodyH;
    BottomPipeBody.setScale({BodyFactorW,BBodyFactorH});

    //设置位置
    float Xoffset = -0.5*(s.PipeCapWidth-s.pipeWidth);
    TopPipeBody.setPosition({Xpos,0});
    TopPipeCap.setPosition({Xpos+Xoffset,TopPipeBody.getGlobalBounds().size.y});

    BottomPipeCap.setPosition({Xpos+Xoffset,gapBottom*s.windowHeight});
    BottomPipeBody.setPosition({Xpos,gapBottom*s.windowHeight+BottomPipeCap.getGlobalBounds().size.y});

}

void Pipe::move(float dt)
{
    float dx = dt * settings.pipeMoveSpeed;
    TopPipeBody.move({dx,0});
    TopPipeCap.move({dx,0});
    BottomPipeBody.move({dx,0});
    BottomPipeCap.move({dx,0});
}

std::map<std::string, std::vector<sf::FloatRect>> Pipe::getBounds()
{
    std::vector<sf::FloatRect> topBounds; 
    topBounds.push_back(TopPipeBody.getGlobalBounds());
    topBounds.push_back(TopPipeCap.getGlobalBounds());

    std::vector<sf::FloatRect> bottomBounds;
    bottomBounds.push_back(BottomPipeBody.getGlobalBounds());
    bottomBounds.push_back(BottomPipeCap.getGlobalBounds());
    std::map<std::string, std::vector<sf::FloatRect>> result;
    result["top"] = topBounds;
    result["bottom"] = bottomBounds;
    return result;
}

PipeController::PipeController(GameSettings s) : settings(s)
{
    frontPipeX = s.windowWidth + 100;
    Pipe p(s, frontPipeX);
    pipes.push_back(p);
    generateX = s.windowWidth - s.generateDistance;
}

void PipeController::restart(){
    this->frontPipeX = settings.windowWidth + 100;
    this->pipes.clear();
    this->pipes.push_back(Pipe(settings,this->frontPipeX));
}

void PipeController::updatePipes(float dt)
{
    for (int i = pipes.size() - 1; i >= 0; i--)
    {
        pipes[i].move(dt);
        if (pipes[i].TopPipeCap.getPosition().x <= settings.PipeCapWidth * -1)
        {
            pipes.erase(pipes.begin() + i);
        }
    }
    frontPipeX = pipes[0].TopPipeCap.getPosition().x;

    if (frontPipeX <= generateX && pipes.size() <= 1)
    {
        pipes.push_back(Pipe(settings, settings.windowWidth));
    }
}

const std::vector<Pipe>& PipeController::getDrawPipes() 
{
    return this->pipes;
}