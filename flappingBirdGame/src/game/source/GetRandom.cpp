#include"../headF/GetRandom.h"

float getRandom(){
    static std::mt19937 gen(std::random_device{}());
    static std::uniform_real_distribution<float> disFloat(0.f,1.f);
    return disFloat(gen);
}