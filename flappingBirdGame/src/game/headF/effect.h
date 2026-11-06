#include <sfml/Graphics.hpp>
#include "GameSetting.h"
class Bird;
class Game;
class impactEffect{
    friend class Bird;
    friend class Game;
    protected:
    float totalTime;
    float currentTime = 0.f;
    float frameDuration;
    float timer = 0;

    float targetW;
    float targetH;
    float frameW;
    float frameH;

    int frameQuntity;
    int currentFrame = 0;

    bool playDone = true;

    sf::Sprite effect;
    public:
    impactEffect(GameSettings s);
    protected:
    void playEffect(sf::FloatRect tBounds,float deltaTime);
    static sf::Texture& getEffectTex();
    bool getPlayDone();
    void setPlayDone(bool p);
    sf::Sprite& getDrawObj()const;
};