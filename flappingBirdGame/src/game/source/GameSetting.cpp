#include "../headF/GameSetting.h"
#include <fstream>

using json = nlohmann::json;

std::vector<sf::IntRect> loadFrames(const std::string& jsonPath){
    std::vector<sf::IntRect> frames;

    //打开文件
    std::ifstream file(jsonPath);
    if(!file.is_open()){
        std::cerr<<"无法打开 Json文件："<<jsonPath<<std::endl;
        return frames;
    }

    //解析json
    json j;
    try{
        file>>j;
    }
    catch(const std::exception& e){
        std::cerr<<"JSON解析失败："<<e.what()<<std::endl;
        return frames;
    }

    if(!j.contains("frames")){
        std::cerr<<"Json 中找不到 frames 节点"<<std::endl;
        return frames;
    }
    auto framesData = j["frames"];

    //提取所有的文件名，并排序
    std::vector<std::string> keys;
    for(auto& el:framesData.items()){
        keys.push_back(el.key());
    }
    std::sort(keys.begin(),keys.end());

    //提取坐标数据
    for(const auto&key:keys){
        auto frameInfo = framesData[key]["frame"];

        int x = frameInfo["x"].get<int>();
        int y = frameInfo["y"].get<int>();
        int w = frameInfo["w"].get<int>();
        int h = frameInfo["h"].get<int>();
        frames.push_back(sf::IntRect({x,y},{w,h}));
    }
    // std::cout<<"成功读取了"<<frames.size()<<"帧数据"<<std::endl;
    return frames;
}