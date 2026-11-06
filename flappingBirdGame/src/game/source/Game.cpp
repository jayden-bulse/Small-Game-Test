#include "../headF/Game.h"
#include <optional>
#include <iostream>
Game::Game(GameSettings s) : settings(s), pipeController(s), bird(s),
 currentState(GameState::Rest), ui(window, s),scene(s),imEffect(s),
 hitSound(hitSoundbuffer),collisionSoundLength(s.collisionSoundLength),
 scoreSound(scoreSoundBuffer)
{
    bool loadR = hitSoundbuffer.loadFromFile("src/game/assets/sounds/SFX_impacthighend02.wav");
    if(!loadR)std::cerr<<"collision.mav load fail"<<std::endl;
    hitSound.setBuffer(hitSoundbuffer);

    bool loadSR = scoreSoundBuffer.loadFromFile("src/game/assets/sounds/getScore.mp3");
    if(!loadSR){std::cout<<"load ScoreSound fail"<<std::endl;}
    scoreSound.setBuffer(scoreSoundBuffer);

    window.create(sf::VideoMode({s.windowWidth, s.windowHeight}), "Flappy Bird");
    window.setFramerateLimit(s.maxFPS);

    ground.setSize({static_cast<float>(s.windowWidth), s.windowHeight * s.groundHeight});
    ground.setPosition({0, s.windowHeight - s.windowHeight * s.groundHeight});
    ground.setFillColor(s.groundColor);

    this->isPass.push_back(0);
}

std::optional<sf::Keyboard::Key> Game::getPressSfCode(const sf::Event &event)
{
    if (const auto *key = event.getIf<sf::Event::KeyPressed>())
    {
        return key->code;
    }
    return std::nullopt;
}

void Game::restart()
{
    bird.restart();
    pipeController.restart();
    this->passPipe = 0;
    ui.restart();
    this->currentState = GameState::Playing;
}

void Game::unRestHandle(){

    while (const std::optional event = window.pollEvent())
    {
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
        else if (event->is<sf::Event::KeyPressed>())
        {
            std::optional<sf::Keyboard::Key> pressKey = getPressSfCode(*event);
            if (pressKey == sf::Keyboard::Key::Space && this->currentState == GameState::Playing)
            {
                bool spaceNow = sf::Keyboard::isKeyPressed(sf::Keyboard::Scancode::Space);
                if (spaceNow && !spacePrev)
                    bird.jump();
                spacePrev = spaceNow;
            }
            else if (pressKey == sf::Keyboard::Key::R)
            {
                this->restart();
            }
            else if (pressKey == sf::Keyboard::Key::P)
            {
                if (this->currentState == GameState::Playing)
                {
                    this->currentState = GameState::Paused;
                }
                else if (this->currentState == GameState::Paused)
                {
                    this->currentState = GameState::Playing;
                }
            }
            else if (pressKey == sf::Keyboard::Key::Escape)
            {
                restart();
                this->currentState = GameState::Rest;
            }
        }
        else if (event->is<sf::Event::KeyReleased>())
        {
            const auto *key = event->getIf<sf::Event::KeyReleased>();
            if (key->code == sf::Keyboard::Key::Space && this->currentState == GameState::Playing)
                spacePrev = false;
        }
        else if(currentState==GameState::GameOver){
            if(ui.isClickRePlay(mousePos,*event))
            {
                this->restart();
            }
            else if(ui.isClickExit(mousePos,*event))
            {
                this->restart();
                currentState = GameState::Rest;
            }
        }
    }
}

void Game::RestHandle(){
  while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())     
        {
            window.close();
            return;
        }

        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (ui.isClickPlay(mousePos, *event))
        {
            currentState = GameState::Playing;
            return;
        }
    }
}

void Game::handleEvent()
{
    if(currentState == GameState::Rest){
        RestHandle();
    }
    else{
        unRestHandle();
    }
}

void Game::CollisionSoundTimer(float deltaTime){
    if(isSoundPlay){
        collisionSoundTimer+=deltaTime;
        if(collisionSoundTimer>=collisionSoundLength){
            hitSound.stop();
            isSoundPlay = false;
            collisionSoundTimer = 0;
        }
    }
}

void Game::playCollisionSound(){
    float randomNum = 0.8 + 0.4*getRandom();
    hitSound.setPitch(randomNum);
    hitSound.setVolume(160.f);
    hitSound.play();
}

void Game::playGetScoreSound(){
    float randomNum = 0.8 + 0.4 * getRandom();
    scoreSound.setPitch(randomNum);
    scoreSound.setVolume(160.f);
    hitSound.stop();
    scoreSound.play();
}

void Game::unRestDraw()
{
    scene.drawScene(window);
    window.draw(bird.player);
    for (const Pipe &drawP : pipeController.getDrawPipes())
    {
        window.draw(drawP.BottomPipeBody);
        window.draw(drawP.BottomPipeCap);
        window.draw(drawP.TopPipeBody);
        window.draw(drawP.TopPipeCap);
    }
    if (imEffect.getPlayDone() != true)
    {
        window.draw(imEffect.getDrawObj());
    }
    ui.drawUnrest(window);
    if(currentState == GameState::GameOver){
        ui.drawDead(window);
    }
}

void Game::RestDraw()
{
    scene.drawScene(window);
    ui.drawRest(window);
}

void Game::Draw()
{
    if(currentState == GameState::Rest){
        RestDraw();
    }
    else{
        unRestDraw();
    }
}

bool Game::PassPipeCheck(){
    std::vector<Pipe> drawPipes = pipeController.getDrawPipes();
    for(int i=0;i<drawPipes.size();i++){
        float dis = drawPipes[i].TopPipeCap.getPosition().x+settings.PipeCapWidth-(bird.player.getPosition().x-bird.shapeHalfW);
        if(dis<0 && this->isPass[i]==0){
            this->isPass[i] = 1;
            this->passPipe++;
            if(this->passPipe>this->passPipeMax){
                this->passPipeMax = this->passPipe;
            }
            return true;
        }
    }
    return false;
}

void Game::updateIspass()
{
    while (this->isPass.size() != this->pipeController.getDrawPipes().size())
    {
        if (this->isPass.size() > this->pipeController.getDrawPipes().size())
        {
            if (!this->isPass.empty())
            {
                this->isPass.erase(this->isPass.begin());
            }
        }
        else if(this->isPass.size() < this->pipeController.getDrawPipes().size()){
            this->isPass.push_back(0);
        }
    }
}

void Game::updateUI(int score){
    //把Game记录的数据传给GameUI
    this->ui.update(score,this->passPipeMax);
}

bool Game::checkCollide(Pipe closePipe){
    sf::FloatRect birdBounds = bird.getBounds();
    std::map<std::string,std::vector<sf::FloatRect>> pipeBounds = closePipe.getBounds();
    bool isCollide = false;
    if(birdBounds.findIntersection(pipeBounds["top"][0]).has_value()){
        isCollide = true;
    }
    else if(birdBounds.findIntersection(pipeBounds["top"][1]).has_value()){
        isCollide = true;
    }
    else if (birdBounds.findIntersection(pipeBounds["bottom"][0]).has_value())
    {
        isCollide = true;
    }
    else if (birdBounds.findIntersection(pipeBounds["bottom"][1]).has_value())
    {
        isCollide = true;
    }
    return isCollide;
}

Pipe Game::getClosePipe()
{
    std::vector<Pipe> pipes = pipeController.getDrawPipes();
    int index = 0;
    for(int i=0;i<this->isPass.size();i++){
        float dis = pipes[i].TopPipeCap.getPosition().x+settings.PipeCapWidth -(bird.player.getPosition().x-bird.shapeHalfW);
        if(isPass[i]==0 && dis>0){
            index = i;
            break;
        }
    }
    return pipes[index];
}

bool Game::isCollide(){
    Pipe ClosePipe = this->getClosePipe();
    float dis = ClosePipe.TopPipeCap.getPosition().x - (bird.player.getPosition().x+bird.shapeHalfW);
    if(dis > 50.f){
        return false;
    }
    bool checkResult = this->checkCollide(ClosePipe);
    if(checkResult == false)return false;
    return true;
}

void Game::Run()
{
    while (window.isOpen())
    {
        sf::Time dt = clock.restart();
        float deltaTime = dt.asSeconds();
        this->handleEvent();
        window.clear(settings.backgroundColor);

        switch (this->currentState)
        {
        case GameState::Playing:
        {
            Playing(deltaTime);
            break;
        };
        case GameState::Paused:
        {
            break;
        }
        case GameState::GameOver:
        {
            GameOver(deltaTime);
            break;
        }
        }

        this->Draw();
        window.display();
    }
}

void Game::GameOver(float deltaTime){
    bird.update(deltaTime);
    bird.deadEffect(deltaTime);
    if(!imEffect.getPlayDone()){
        imEffect.playEffect(bird.getBounds(),deltaTime);
    }
    CollisionSoundTimer(deltaTime);

}

void Game::Playing(float deltaTime)
{
    bird.update(deltaTime);
    bird.animate();
    pipeController.updatePipes(deltaTime);
    scene.sceneMove(deltaTime);
    updateIspass();
    bool isCollide = this->isCollide();
    if (isCollide == true)
    {
        this->currentState = GameState::GameOver;
        //计算bird的fallTime
        bird.velocity = settings.deadJumpSpeed;
        float skyH = settings.windowHeight*(1-settings.groundHeight);
        float birdY = skyH -bird.player.getGlobalBounds().position.y+bird.player.getGlobalBounds().size.y;
        float fallV = std::pow(2*birdY*settings.gravity-bird.velocity*bird.velocity,0.5);
        bird.fallTime = (fallV - bird.velocity)/(settings.gravity*1.0);
        bird.deadMoveDis = bird.deadMoveDisMax*0.6+0.4*bird.deadMoveDisMax*(birdY/skyH);
        bird.DeadMoveX = true;

        imEffect.setPlayDone(false);
        isSoundPlay = true;
        bird.jumpSound.stop();
        playCollisionSound();
    }
    bool isPass = PassPipeCheck();

    if (isPass)
    {
        updateUI(this->passPipe);
        playGetScoreSound();
    }
    if (isCollide)
    {
        bird.dead();
    }
}