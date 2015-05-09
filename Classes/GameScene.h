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
    Button * _musicBtn;
    Button * _pauseBtn;
    bool isMusicBtnOut;
    int _gameTimes;
    int _jianCePinLv;
    float _playTime;
    float _highScore;
    int _socre;
    int _totalScore;
    float _nextTimeCreateEnemy;
    Label * _timeLabel ;
    Sprite * _spriteBg;
    Vector<BirdBase *> _birds;
    Vector<Sprite * > _enemy;
    Vector<Sprite * > _life;
    
    bool onTouchBegan(Touch* touch, Event*event);
    void onTouchMoved(Touch* touch, Event*event);
    void onTouchEnded(Touch* touch, Event*event);
    void btnCallback(Ref*,Widget::TouchEventType);
    void btnSetCallback(Ref*,Widget::TouchEventType);
    virtual bool init();
    void onEnter();
    void createEnemy();
    void createLife();
    float getFlyTimeByScore();
    bool checkPengZhuang();
    int getGameLevelByScore();
    void gameOver();
    void addLife(int num);
    std::vector<int> getSuiJiShu(int num,int baoliu);
    void createFuncCallback();
public:
    void pauseGame();
    void update(float dt);
    CREATE_FUNC(GameScene)
    static Scene * createScene();
    GameScene();
    ~GameScene();
};


#endif /* defined(__SomeBirds__GameScene__) */
