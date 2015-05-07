//
//  GameScene.h
//  SomeBirds
//
//  Created by ArcherPeng on 15/4/29.
//
//

#ifndef __SomeBirds__GameScene__
#define __SomeBirds__GameScene__

#include "cocos2d.h"
#include "BirdBase.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
using namespace ui;
class GameScene:public Layer
{
    
protected:
    Button * musicBtn;
    bool isMusicBtnOut=false;
    int _gameTimes = 0;
    int _jianCePinLv = 0;
    Label * _timeLabel ;
    virtual bool init();
    Sprite * _spriteBg;
    Vector<BirdBase *> _birds;
    bool onTouchBegan(Touch* touch, Event*event);
    void onTouchMoved(Touch* touch, Event*event);
    void onTouchEnded(Touch* touch, Event*event);
    void btnCallback(Ref*,Widget::TouchEventType);
    void btnSetCallback(Ref*,Widget::TouchEventType);
    float _playTime = 0;
    int _socre = 0;
    int _totalScore = 0;
    float _nextTimeCreateEnemy = 1.5f;
    
    void createEnemy();
    void createLife();
    float getFlyTimeByScore();
    bool checkPengZhuang();
    int getGameLevelByScore();
    Vector<Sprite * > _enemy;
    Vector<Sprite * > _life;
    void gameOver();
    void addLife(int num);
    std::vector<int> getSuiJiShu(int num,int baoliu);
    void createFuncCallback();
public:
    void update(float dt);
    CREATE_FUNC(GameScene)
    static Scene * createScene()
    {
        auto scene = Scene::create();
        auto layer = GameScene::create();
        scene->addChild(layer);
        return scene;
    }
    GameScene(){};
    ~GameScene(){};
};


#endif /* defined(__SomeBirds__GameScene__) */
