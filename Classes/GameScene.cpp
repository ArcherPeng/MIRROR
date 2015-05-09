//
//  GameScene.cpp
//  SomeBirds
//
//  Created by ArcherPeng on 15/4/29.
//
//

#include "GameScene.h"
//#include "cocos-ext.h"
#include "SimpleAudioEngine.h"
#include "InfoLayer.h"
#include <vector>

Scene * GameScene::createScene()
{
    auto scene = Scene::create();
    auto layer = GameScene::create();
    scene->addChild(layer);
    return scene;
}
GameScene::GameScene()
{
    _pauseBtn = nullptr;
    _musicBtn = nullptr;
    isMusicBtnOut=false;
    _gameTimes = 0;
    _jianCePinLv = 0;
    _playTime = 0;
    _socre = 0;
    _totalScore = 0;
    _nextTimeCreateEnemy = 1.5f;
    _highScore = 0.0f;
    _timeLabel = nullptr;
    _spriteBg = nullptr;
};
GameScene::~GameScene(){};
bool GameScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    this->setName("GameScene");
//    std::vector<int> abc;
    auto visableSize = Director::getInstance()->getVisibleSize();
    _timeLabel = Label::createWithSystemFont("0.00s", "Arial", 140);
    _timeLabel->setAnchorPoint(Vec2(0, 1));
    _timeLabel->setTextColor(Color4B(252,233,219,255));
    _timeLabel->setPosition(15,visableSize.height - 10);
    this->addChild(_timeLabel);
    _timeLabel->setLocalZOrder(10);
    
    auto bgSp = Sprite::create("res/ui/005.png");
    bgSp->setPosition(visableSize.width/2,visableSize.height/2);
    this->addChild(bgSp);
    auto bgSp1 = Sprite::create("res/ui/005.png");
    bgSp1->setPosition(-bgSp->getContentSize().width/2,bgSp->getContentSize().height/2);
    bgSp->addChild(bgSp1);
    bgSp1 = Sprite::create("res/ui/005.png");
    bgSp1->setPosition(bgSp->getContentSize().width+bgSp->getContentSize().width/2,bgSp->getContentSize().height/2);
    bgSp->addChild(bgSp1);
    bgSp1 = Sprite::create("res/ui/bgup.png");
    bgSp1->setPosition(bgSp->getContentSize().width/2,bgSp->getContentSize().height/2);
    bgSp->addChild(bgSp1);
    bgSp1->setLocalZOrder(10);
    _pauseBtn= Button::create("res/ui/pausebtn.png");
    _pauseBtn->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - _pauseBtn->getContentSize().width/2, Director::getInstance()->getVisibleSize().height-_pauseBtn->getContentSize().height/2));
    _pauseBtn->setTag(103);
    _pauseBtn->addTouchEventListener(CC_CALLBACK_2(GameScene::btnSetCallback, this));
    this->addChild(_pauseBtn);
    
    _musicBtn = Button::create("res/ui/musicbtn.png");
    _musicBtn->setPosition(Vec2(Director::getInstance()->getVisibleSize().width+50, Director::getInstance()->getVisibleSize().height-65));
    _musicBtn->setTag(102);
    _musicBtn->addTouchEventListener(CC_CALLBACK_2(GameScene::btnSetCallback, this));
    this->addChild(_musicBtn);
    _musicBtn->setLocalZOrder(10);
    bool isMusicOn = UserDefault::getInstance()->getBoolForKey("isMusicOn",true);
    if (!isMusicOn)
    {
        _musicBtn->loadTextureNormal("res/ui/musicbtndis.png");
        
        CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
        CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);
    }
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/back.mp3",true); //NOTE: 在WP8平台，不支持Mp3格式，请转化为wav
    this->_spriteBg = Sprite::create("res/ui/006.png");
    this->_spriteBg->setPosition(Vec2(bgSp->getContentSize().width/2, bgSp->getContentSize().height/2));
    bgSp->addChild(this->_spriteBg);
    auto bird = BirdBase::createBird(1);
    bird->setPosition(this->_spriteBg->getContentSize().width/2+100,this->_spriteBg->getContentSize().height/2);
    this->_spriteBg->addChild(bird);
    _birds.pushBack(bird);
    bird = BirdBase::createBird(2);
    bird->setPosition(this->_spriteBg->getContentSize().width/2-100,this->_spriteBg->getContentSize().height/2);
    this->_spriteBg->addChild(bird);
    _birds.pushBack(bird);
//    bird = BirdBase::createBird(3);
//    bird->setPosition(this->_spriteBg->getContentSize().width/2,this->_spriteBg->getContentSize().height/2+100);
//    this->_spriteBg->addChild(bird);
//    _birds.pushBack(bird);
//    bird = BirdBase::createBird(4);
//    bird->setPosition(this->_spriteBg->getContentSize().width/2,this->_spriteBg->getContentSize().height/2-100);
//    this->_spriteBg->addChild(bird);
//    _birds.pushBack(bird);
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    this->getScheduler()->scheduleUpdate(this,0,false);
//    void update(float dt);
    return true;
}
void GameScene::onEnter()
{
    Layer::onEnter();
    _highScore = UserDefault::getInstance()->getFloatForKey("HighScore",0);
}
bool GameScene::checkPengZhuang()
{
    bool pengZhuangFlag = true;
    int lifeNum = 0;
    do {
        //TODO 检测碰撞
        for(auto bird : _birds)
        {
            for(auto enemy : _enemy)
            {
                bool b = (bird->getPosition().x-enemy->getPosition().x)*(bird->getPosition().x-enemy->getPosition().x)+(bird->getPosition().y-enemy->getPosition().y)*(bird->getPosition().y-enemy->getPosition().y) < 1600;
                if(b)//bird->getBoundingBox().intersectsRect(enemy->getBoundingBox())
                {
                    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/d.wav",false);
                    for(auto enemy : _enemy)
                    {
                        enemy->stopAllActions();
                        enemy->setLocalZOrder(5);
                        auto big = ScaleTo::create(0.7, 4);
                        auto fadeOut = FadeOut::create(0.7);
                        enemy->runAction(Sequence::create(Spawn::create(big,fadeOut, NULL),RemoveSelf::create(), NULL));
                        
                        
//                        enemy->removeFromParentAndCleanup(true);
                        
                    }
//                    _enemy.clear();
                    
                    if (_birds.size()<3)
                    {
                        for(auto enemy : _enemy)
                        {
                            enemy->stopAllActions();
                        }
                        for(auto life : _life)
                        {
                            life->stopAllActions();
                        }
                        
                        this->getScheduler()->unscheduleUpdate(this);
                        Director::getInstance()->getEventDispatcher()->removeEventListenersForTarget(this);
                        this->gameOver();
//                        this->addLife(lifeNum);
                        return pengZhuangFlag;
                    }
                    auto big = ScaleTo::create(0.7,1);
                    auto fadeOut = FadeOut::create(0.7);
                    _birds.at(_birds.size()-1)->setStopMove(true);
                    _birds.at(_birds.size()-1)->runAction(Sequence::create(Spawn::create(big,fadeOut, NULL),RemoveSelf::create(), NULL));
//                    _birds.at(_birds.size()-1)->removeFromParentAndCleanup(true);
                    
                    _birds.popBack();
                    _enemy.clear();
                    
                    this->addLife(lifeNum);
                    return pengZhuangFlag;
                }
            }
            for (int i = 0; i<_life.size(); ++i)
            {
                auto life = _life.at(i);
                bool b = (bird->getPosition().x-life->getPosition().x)*(bird->getPosition().x-life->getPosition().x)+(bird->getPosition().y-life->getPosition().y)*(bird->getPosition().y-life->getPosition().y) < 1600;
                if(b)//bird->getBoundingBox().intersectsRect(life->getBoundingBox())
                {
                    _life.erase(i);
//                    life->removeFromParentAndCleanup(true);
                    auto big = ScaleTo::create(0.5, 3);
                    auto fadeOut = FadeOut::create(0.5);
                    life->setLocalZOrder(5);
                    life->runAction(Sequence::create(Spawn::create(big,fadeOut, NULL),RemoveSelf::create(), NULL));
                    ++lifeNum;
                    CCLOG("lifeNum :%d",lifeNum);
                    --i;
                    this->addLife(lifeNum);
                    return pengZhuangFlag;
                }
            }
        }
        pengZhuangFlag = false;
    } while (0);
    this->addLife(lifeNum);
    return pengZhuangFlag;
}
void GameScene::btnCallback(Ref* ref,Widget::TouchEventType eventType)
{
    if (eventType == Widget::TouchEventType::ENDED)
    {
        CCLOG("11");
        auto bg = this->_spriteBg->getParent();
        auto rotateAction = RotateBy::create(2.5, 360);
        auto easeOutAction = EaseIn::create(rotateAction, 3);
        auto scaleAction = ScaleTo::create(2.5, 1);
        auto easeBigAction = EaseIn::create(scaleAction, 3);
        bg->runAction(Spawn::create(easeOutAction,easeBigAction, NULL));
        
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/gamestart.wav");//gamestart.wav
        this->runAction(Sequence::create(DelayTime::create(2),CallFunc::create([](){
            auto scene = GameScene::createScene();
            auto actionScene = TransitionCrossFade::create(0.5, scene);
            Director::getInstance()->replaceScene(actionScene);
        }),NULL));
        
    }

}
void GameScene::addLife(int num)
{
    if (num>0)
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/levelup.mp3");
    }
    if (num == 0 || _birds.size()>= 4)
    {
        return;
    }
    int type[4]={0,0,0,0};
    for (int i = 0; i<_birds.size(); ++i)
    {
        type[_birds.at(i)->getBirdType()-1] = 1;
    }
    for (int i = 0; i<num; ++i)
    {
        if (_birds.size()>= 4)
        {
            break;
        }
        for (int j = 0; j<4; ++j)
        {
            if (type[j]==0)
            {
                auto bird = BirdBase::createBird(j+1);
                auto vec = _birds.at(0)->getXiangDuiVec2();
                bird->setPosition(bird->getInitPosition(vec));
                this->_spriteBg->addChild(bird);
                _birds.pushBack(bird);
                break;
            }
        }
    }
}

void GameScene::btnSetCallback(Ref* ref,Widget::TouchEventType eventType)
{
    if (eventType == Widget::TouchEventType::ENDED)
    {
        auto btn = dynamic_cast<Button *>(ref);
        int tag = btn->getTag();
        if (tag==100)//关于
        {
            auto layer = InfoLayer::create();
            layer->setPosition(Vec2::ZERO);
            layer->setLocalZOrder(20);
            this->addChild(layer);
        }
        else if(tag == 101)//设置
        {
            CCLOG("btnSetCallback  tag 100");
            if(isMusicBtnOut)
            {
                _musicBtn->runAction(MoveTo::create(0.15, Vec2(Director::getInstance()->getVisibleSize().width+50, btn->getPosition().y)));
                isMusicBtnOut = false;
            }
            else
            {
                auto move1 = MoveTo::create(0.15, Vec2(btn->getPosition().x-125, btn->getPosition().y));
                auto move2 = MoveTo::create(0.05, Vec2(btn->getPosition().x-115, btn->getPosition().y));
                _musicBtn->runAction(Sequence::create(move1,move2, NULL));
                isMusicBtnOut = true;
            }
        }
        else if(tag == 102)//音量
        {
            bool isMusicOn = UserDefault::getInstance()->getBoolForKey("isMusicOn",true);
            if (isMusicOn)
            {
                _musicBtn->loadTextureNormal("res/ui/musicbtndis.png");
                UserDefault::getInstance()->setBoolForKey("isMusicOn",false);
                CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0);
                CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(0);
            }
            else
            {
                _musicBtn->loadTextureNormal("res/ui/musicbtn.png");
                UserDefault::getInstance()->setBoolForKey("isMusicOn",true);
                CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(1);
                CocosDenshion::SimpleAudioEngine::getInstance()->setEffectsVolume(1);
            }
        }
        else if(tag == 103)//暂停
        {
            this->pauseGame();
            
        }
        else if(tag == 104)//继续
        {
            this->resume();
            _pauseBtn->setTouchEnabled(true);
            auto layer = this->getChildByName("PauseLayer");
            if (layer)
            {
                layer->removeFromParentAndCleanup(true);
            }
            for (Sprite * enemy : _enemy)
            {
                enemy->resume();
            }
            for (Sprite * life : _life)
            {
                life->resume();
            }
        }
//        CCLOG("btnSetCallback");
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/d.wav", false);
    }
}
void GameScene::pauseGame()
{
    this->pause();
    _pauseBtn->setTouchEnabled(false);
    auto colorLayer = LayerColor::create(Color4B(0, 0, 0, 120), Director::getInstance()->getVisibleSize().width,Director::getInstance()->getVisibleSize().height);
    colorLayer->setAnchorPoint(Vec2(0, 0));
    colorLayer->setPosition(Vec2::ZERO);
    colorLayer->setLocalZOrder(100);
    this->addChild(colorLayer);
    colorLayer->setName("PauseLayer");
    auto resumeBtn= Button::create("res/ui/resumebtn.png");
    resumeBtn->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2, Director::getInstance()->getVisibleSize().height/2));
    resumeBtn->setTag(104);
    resumeBtn->addTouchEventListener(CC_CALLBACK_2(GameScene::btnSetCallback, this));
    colorLayer->addChild(resumeBtn);
    auto pauseLabel = Label::createWithSystemFont("PAUSE", "Arial", 140);
    pauseLabel->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2, Director::getInstance()->getVisibleSize().height/4*3));
    colorLayer->addChild(pauseLabel);
    for (Sprite * enemy : _enemy)
    {
        enemy->pause();
    }
    for (Sprite * life : _life)
    {
        life->pause();
    }
}
void GameScene::createFuncCallback()
{
    auto btn = Button::create("res/ui/restart.png");
    btn->addTouchEventListener(CC_CALLBACK_2(GameScene::btnCallback, this));
    btn->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2, Director::getInstance()->getVisibleSize().height/4));
    this->addChild(btn);
    btn->setOpacity(0);
    btn->runAction(FadeIn::create(0.8));
    btn->setLocalZOrder(10);
    
    auto gameOverSp = Sprite::create("res/ui/gameover.png");
    gameOverSp->setPosition(Director::getInstance()->getVisibleSize().width/2, Director::getInstance()->getVisibleSize().height/2-50);
    gameOverSp->setOpacity(0);
    gameOverSp->runAction(FadeIn::create(0.8));
    gameOverSp->setLocalZOrder(10);
    this->addChild(gameOverSp);
    
    auto btnSet = Button::create("res/ui/setting.png");
    btnSet->addTouchEventListener(CC_CALLBACK_2(GameScene::btnSetCallback, this));
    btnSet->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 65, Director::getInstance()->getVisibleSize().height-65));
    btnSet->setTag(101);
    this->addChild(btnSet);
    btnSet->setOpacity(0);
    btnSet->runAction(FadeIn::create(0.8));
    btnSet->setLocalZOrder(10);
    
    _timeLabel->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height/2-40));
    _timeLabel->setOpacity(0);
    _timeLabel->setTextColor(Color4B(37,162,51,255));
    _timeLabel->setAnchorPoint(Vec2(0.5,0.5));
    _timeLabel->runAction(FadeIn::create(0.8));
    
    auto btnInfo = Button::create("res/ui/info.png");
    btnInfo->addTouchEventListener(CC_CALLBACK_2(GameScene::btnSetCallback, this));
    btnInfo->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 50, 50));
    this->addChild(btnInfo);
    btnInfo->setTag(100);
    btnInfo->setOpacity(0);
    btnInfo->runAction(FadeIn::create(0.8));
    btnInfo->setLocalZOrder(10);

    Label* hightScoreLabel = nullptr;
    if (_playTime>_highScore)
    {
        UserDefault::getInstance()->setFloatForKey("HighScore", _playTime);
        _highScore=_playTime;
        hightScoreLabel = Label::createWithSystemFont("Congratulations! \nNEW HIGH SCORE!", "Arial", 70);
        hightScoreLabel->setTextColor(Color4B(205,85,85,255));
        hightScoreLabel->setHorizontalAlignment(TextHAlignment::CENTER);
        hightScoreLabel->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height/2+30));
        hightScoreLabel->setAnchorPoint(Vec2(0.5, 0));
    }
    else
    {
        hightScoreLabel = Label::createWithSystemFont(StringUtils::format("HIGH SCORE: %0.2fs",_highScore), "Arial", 40);
        hightScoreLabel->setTextColor(Color4B(255,255,255,255));
        hightScoreLabel->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height/2-120));
        hightScoreLabel->setAnchorPoint(Vec2(0.5, 1));
    }
    hightScoreLabel->setOpacity(0);
    hightScoreLabel->runAction(FadeIn::create(0.8));
    this->addChild(hightScoreLabel);
    hightScoreLabel->setLocalZOrder(10);
    
}
void GameScene::gameOver()
{
    auto bg = this->_spriteBg->getParent();
    auto rotateAction = RotateBy::create(8, -120);
    auto easeOutAction = EaseOut::create(rotateAction, 1.6);
    auto scaleAction = ScaleTo::create(8, 3);
    auto easeBigAction = EaseOut::create(scaleAction, 1.6);
    bg->runAction(Spawn::create(easeOutAction,easeBigAction, NULL));
    bg->runAction(Sequence::create(DelayTime::create(1.5),CallFunc::create(CC_CALLBACK_0(GameScene::createFuncCallback, this)),NULL));
    _timeLabel->runAction(FadeOut::create(0.8));
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/die.mp3");//gamestart.wav
    CCLOG("GameOver");
    
    _pauseBtn->runAction(FadeOut::create(0.8));
    _pauseBtn->setTouchEnabled(false);
}
float GameScene::getFlyTimeByScore()
{
    if (_gameTimes<4)
    {
        return 3.0f;
    }
    else if (_gameTimes<12)
    {
        return 2.5f;
    }
    else if (_gameTimes<20)
    {
        return 2.0f;
    }
    else
    {
        return 1.5f;
    }
}
int GameScene::getGameLevelByScore()
{
    if (_gameTimes<4)
    {
        return 1;
    }
    else if (_gameTimes<8)
    {
        return 2;
    }
    else if (_gameTimes<8)
    {
        return 3;
    }
    else if (_gameTimes<12)
    {
        return 4;
    }
    else if (_gameTimes<16)
    {
        return 5;
    }
    else if (_gameTimes<20)
    {
        return 6;
    }
    else if (_gameTimes<24)
    {
        return 7;
    }
    else if (_gameTimes<28)
    {
        return 8;
    }
    else if (_gameTimes<32)
    {
        return 9;
    }
    
    else
    {
        return 10;
    }
}
std::vector<int> GameScene::getSuiJiShu(int num,int baoliu)
{
    int arr[22];
    for (int i = 0,j=0 ; i<23; ++i)
    {
        if (i!=baoliu) {
            arr[j]=i+1;
            ++j;
        }
    }
    for (int i = 0; i<50; ++i)
    {
        int m=rand()%22;
        int n=rand()%22;
        
        int t=arr[m];
        arr[m]=arr[n];
        arr[n]=t;
    }
    std::vector<int> vec;
    vec.push_back(baoliu);
    for (int i = 0; i<num-1; ++i)
    {
        vec.push_back(arr[i]);
    }
    return vec;
}

void GameScene::createEnemy()
{
    
    _enemy.clear();
    ++_gameTimes;
    float flyTime = getFlyTimeByScore();
    _nextTimeCreateEnemy = _playTime + flyTime + 0.5;
    int gameLevel = this->getGameLevelByScore();
    int enemyNum = gameLevel+1;
    if (gameLevel>5)
    {
        enemyNum += 1;
    }
    if (gameLevel>8)
    {
        enemyNum += 1;
    }
//    CCLOG("enemyNum : %d",enemyNum);
    std::vector<int> feiChuFangXiang;
    
    feiChuFangXiang.push_back(rand()%4);
    if(gameLevel>3)
    {
        feiChuFangXiang.push_back(rand()%4);
    }
    if(gameLevel>5)
    {
        feiChuFangXiang.push_back(rand()%4);
    }
    if(gameLevel>8)
    {
        feiChuFangXiang.push_back(rand()%4);
    }
    
    std::vector<int> suiJiShuXVec = getSuiJiShu(enemyNum,(_birds.at(rand()%_birds.size())->getPosition().x/30));
    std::vector<int> suiJiShuYVec = getSuiJiShu(enemyNum,(_birds.at(rand()%_birds.size())->getPosition().y/30));
    for (int i = 0; i<enemyNum; ++i)
    {
        //创建敌人
        int fangXiang = feiChuFangXiang[rand()%feiChuFangXiang.size()];
        //方向 0 → 1 ↑ 2 ← 3 ↓
        auto sp = Sprite::create("res/enemy/007.png");
        sp->setName("Enemy");
        MoveBy* moveBy ;
        
        float vec2x = suiJiShuXVec[i]*30+20;
        float vec2y = suiJiShuYVec[i]*30+20;
//        if(i == 0)
//        {
//            auto vec = _birds.at(rand()%_birds.size())->getPosition();
//            vec2x = vec.x;
//            vec2y = vec.y;
//        }
        if (fangXiang == 0)
        {
            vec2x = -15;
            moveBy = MoveBy::create(flyTime, Vec2(760,0));
        }
        else if(fangXiang == 1)
        {
            vec2y = 730+15;
            moveBy = MoveBy::create(flyTime, Vec2(0,-760));
        }
        else if(fangXiang == 2)
        {
            vec2x = 730+15;
            moveBy = MoveBy::create(flyTime, Vec2(-760,0));
        }
        else
        {
            vec2y = -15;
            moveBy = MoveBy::create(flyTime, Vec2(0,760));
        }
//        auto callFunc = CallFunc::create([&]()
//                                         {
////                                             sp->removeFromParentAndCleanup(true);
//                                             _enemyAndLife.eraseObject(sp);
//                                         });
        _enemy.pushBack(sp);
        this->_spriteBg->addChild(sp);
        sp->setPosition(vec2x,vec2y);
        sp->runAction(Sequence::create(moveBy,RemoveSelf::create(), NULL));
    }
    createLife();
    
}
void GameScene::createLife()
{
    _life.clear();
    if ((rand()%4)==1)// 4 分之 1几率   rand()%4 <1
    {

        auto sp = Sprite::create("res/enemy/008.png");
        sp->setName("Life");
        float vec2x = rand()%650+15+25;
        float vec2y = rand()%650+15+25;

        _life.pushBack(sp);
        this->_spriteBg->addChild(sp);
        sp->setPosition(vec2x,vec2y);
        auto fadeInAction = FadeIn::create(0.8);
        auto delAction = DelayTime::create(1.5);
        auto shanAction = Blink::create(1.5, 5);
        auto call = CallFuncN::create([&](Node * node)
                                     {
                                         _life.eraseObject(dynamic_cast<Sprite *>(node));
                                     });
        auto removeSelf = RemoveSelf::create();
        sp->runAction(Sequence::create(fadeInAction,delAction,shanAction,call,removeSelf, NULL));
    }
}
void GameScene::update(float dt)
{
    _playTime += dt; //计算游戏时间
    
    ++_jianCePinLv;
    if (_jianCePinLv<2)
    {
        return;
    }
    _jianCePinLv = 0;
    _timeLabel->setString(StringUtils::format("%.2fs",_playTime));
    if (_playTime >= _nextTimeCreateEnemy)//根据时间创建敌人
    {
        this->createEnemy();//创建敌人
        
    }
    this->checkPengZhuang();//检测碰撞
    
    
    _totalScore = _socre + static_cast<int>(_playTime);
    
}
bool GameScene::onTouchBegan(Touch* touch, Event*event)
{
    return true;
}
void GameScene::onTouchMoved(Touch* touch, Event*event)
{
    auto vecPre = touch->getPreviousLocation();
    auto vecNow = touch->getLocation();
    float x = vecNow.x - vecPre.x;
    float y = vecNow.y - vecPre.y;
    for (BirdBase * bird : this->_birds)
    {
        bird->setPositionAsVec2(x, y);
    }
}
void GameScene::onTouchEnded(Touch* touch, Event*event)
{
    
}