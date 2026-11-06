#include "../headF/Scene.h"
#include <iostream>
#include <algorithm>
sf::Texture& Scene::getSceneTex1(){
    static sf::Texture tex; 
    static bool isLoad = tex.loadFromFile("src/game/assets/photos/background2_clean2.png");
    if(!isLoad){std::cerr<<"background.png Load Fail"<<std::endl;}
    return tex;
}

sf::Texture& Scene::getGroundTex(){
    static sf::Texture tex; 
    static bool isLoad = tex.loadFromFile("src/game/assets/photos/ground4.png");
    tex.setRepeated(true);
    if(!isLoad){std::cerr<<"ground2.png Load Fail"<<std::endl;}
    return tex;
}

Scene::Scene(GameSettings s):settings(s),backGround1(getSceneTex1()),
backGround2(getSceneTex1()),ground(getGroundTex()),ground1(getGroundTex()){
    backGround1.setTexture(getSceneTex1(),true);
    backGround2.setTexture(getSceneTex1(),true);

    float oriSizeH = backGround1.getLocalBounds().size.y;
    float oriSizeW = backGround1.getLocalBounds().size.x;

    float factorH = s.windowHeight/oriSizeH;
    float factorW = s.windowWidth/oriSizeW;
    float scale = std::max(factorH,factorW);

    backGround1.setScale({scale,scale});
    backGround2.setScale({scale,scale});
    sf::Texture& groundTex = getGroundTex();
    ground.setTexture(groundTex,true);
    ground.setTextureRect(sf::IntRect({0,0},{static_cast<int>(s.windowWidth*2),static_cast<int>(groundTex.getSize().y)}));
    ground1.setTexture(groundTex,true);
    ground1.setTextureRect(sf::IntRect({0,0},{static_cast<int>(s.windowWidth*2),static_cast<int>(groundTex.getSize().y)}));
    float oriSizeGH = ground.getLocalBounds().size.y;
    float factorGH = (s.groundHeight*s.windowHeight)/oriSizeGH;

    ground.setScale({1,factorGH});
    ground.setPosition({0,s.windowHeight*(1-s.groundHeight)});

    ground1.setScale({1,factorGH});
    ground1.setPosition({-1*ground1.getGlobalBounds().size.x,s.windowHeight*(1-s.groundHeight)});

    sf::FloatRect bounds = backGround1.getGlobalBounds();
    backGround1.setPosition({0,0});
    backGround2.setPosition({s.windowWidth,0});
}

void Scene::sceneMove(float dt){
    float dx = dt*settings.sceneMoveSpeed;
    backGround1.move({dx,0});
    backGround2.move({dx,0});
    sf::FloatRect bounds = backGround1.getGlobalBounds();
    if(backGround1.getPosition().x+bounds.size.x<=0){
        backGround1.setPosition({bounds.size.x,0});
        backGround2.setPosition({0,0});
    }
    else if(backGround2.getPosition().x+bounds.size.x<=0){
        backGround2.setPosition({bounds.size.x,0});
        backGround1.setPosition({0,0});
    }
    groundMove(dt);
}

void Scene::groundMove(float dt)
{
    float dx = dt * settings.pipeMoveSpeed;
    ground.move({dx, 0});
    ground1.move({dx, 0});
    sf::FloatRect bounds = ground.getGlobalBounds();
    sf::FloatRect bounds1 = ground1.getGlobalBounds();
    float groundHeight = settings.windowHeight*(1-settings.groundHeight);
    if (bounds.position.x>bounds1.position.x && bounds.position.x + bounds.size.x <= settings.windowWidth)
    {
        ground.setPosition({settings.windowWidth - bounds.size.x, groundHeight});
        ground1.setPosition({settings.windowWidth, groundHeight});
    }
    else if (bounds1.position.x>bounds.position.x && bounds1.position.x + bounds1.size.x <= settings.windowWidth)
    {
        ground1.setPosition({settings.windowWidth - bounds1.size.x, groundHeight});
        ground.setPosition({settings.windowWidth, groundHeight});
    }
}

void Scene::drawScene(sf::RenderTarget& target){
    target.draw(backGround1);
    target.draw(backGround2);
    target.draw(ground);
    target.draw(ground1);
}