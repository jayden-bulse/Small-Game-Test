#include "GameSetting.h"
#include "Bird.h"
#include "PipeAPipeController.h"
#include "GameUI.h"
#include<map>
#include<vector>
#include<string>
#include "Scene.h"
#include "effect.h"
#include <SFML/Audio.hpp>

enum class GameState{Rest,Playing,Paused,GameOver};

class Game
{
public:
    explicit Game(GameSettings s);

protected:
    GameSettings settings;
    sf::Clock clock;
    Bird bird;
    PipeController pipeController;
    sf::RectangleShape ground;
    sf::RenderWindow window;
    GameState currentState;
    GameUI ui;
    Scene scene;
    impactEffect imEffect;

    sf::SoundBuffer hitSoundbuffer;
    sf::Sound hitSound;
    float collisionSoundLength;
    float collisionSoundTimer = 0;
    bool isSoundPlay = false;

    // 单次触发判断变量
    bool spacePrev = false;
    // 游戏进行时通过的管道数量
    unsigned int passPipe = 0;
    unsigned int mostPassPipe = 0;
    unsigned int passPipeMax = 0;
    //聚焦管
    std::vector<int> isPass;

    sf::SoundBuffer scoreSoundBuffer;
    sf::Sound scoreSound;

    std::optional<sf::Keyboard::Key> getPressSfCode(const sf::Event& event);
    void handleEvent();
    void Draw();
    bool checkCollide(Pipe closePipe);
    bool isCollide();
    Pipe getClosePipe();
    void updateUI(int score);
    void updateIspass();
    bool PassPipeCheck(); 
    void restart();
    void Playing(float deltaTime);
    void GameOver(float deltaTime);
    void playCollisionSound();
    void CollisionSoundTimer(float deltaTime);
    void playGetScoreSound();

    void unRestHandle();
    void RestHandle();

    void RestDraw();
    void unRestDraw();
public:
    void Run();
};
