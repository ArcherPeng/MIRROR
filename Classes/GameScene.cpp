//
//  GameScene.cpp
//  SomeBirds
//
//  Created by ArcherPeng on 15/4/29.
//
//
// Magic. Do not touch.
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
    _isGameOver = false;
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
    _bgUP = nullptr;
    _sampleNum = 0.0f;
    _blurRadius = 0.0f;
    _gLProgramState = nullptr;
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
    
    _bgUP = Sprite::create("res/ui/bgup.png");
    _bgUP->setPosition(bgSp->getContentSize().width/2,bgSp->getContentSize().height/2);
    bgSp->addChild(_bgUP);
    _bgUP->setLocalZOrder(10);
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
    

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/back.wav",true);
#else
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/back.mp3",true);
#endif
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
    
    auto listenerkeyPad = EventListenerKeyboard::create();
    listenerkeyPad->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listenerkeyPad, this);
    
    this->getScheduler()->scheduleUpdate(this,0,false);
//    void update(float dt);
    return true;
}
void GameScene::onKeyReleased(EventKeyboard::KeyCode keycode, cocos2d::Event *event)
{
    if (keycode == EventKeyboard::KeyCode::KEY_BACKSPACE)  //返回
    {
        this->pauseGame();
    }
    else if (keycode == EventKeyboard::KeyCode::KEY_MENU)
    {
        
    }
}
//void GameScene::blurSchluder(float dt)
//{
//    _blurRadius += (25*0.8)/20;
//    _sampleNum += (11*0.6)/20;
////    getGLProgramState()->setUniformFloat("blurRadius", _blurRadius);
////    getGLProgramState()->setUniformFloat("sampleNum", 7.0f);
//    for (auto bird:_birds)
//    {
//        bird->getBirdSprite()->getGLProgramState()->setUniformFloat("blurRadius", _blurRadius);
//        bird->getBirdSprite()->getGLProgramState()->setUniformFloat("sampleNum", _sampleNum);
//    }
//    for (auto life:_life)
//    {
//        life->getGLProgramState()->setUniformFloat("blurRadius", _blurRadius);
//        life->getGLProgramState()->setUniformFloat("sampleNum", _sampleNum);
//    }
//    _bgUP->getGLProgramState()->setUniformFloat("blurRadius", _blurRadius);
//    _bgUP->getGLProgramState()->setUniformFloat("sampleNum", _sampleNum);
//    for (auto enemy:_enemy)
//    {
//        enemy->getGLProgramState()->setUniformFloat("blurRadius", _blurRadius);
//        enemy->getGLProgramState()->setUniformFloat("sampleNum", _sampleNum);
//    }
//}
//void GameScene::blurBackSchluder(float dt)
//{
//    _blurRadius -= (25*0.8)/60;
//    _sampleNum -= (11*0.6)/60;
//}
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
                    auto big = ScaleTo::create(0.7,1.5);
                    auto fadeOut = FadeOut::create(0.7);
                    _birds.at(_birds.size()-1)->setStopMove(true);
                    _birds.at(_birds.size()-1)->getBirdSprite()->runAction(Spawn::create(big,fadeOut, NULL));
                    _birds.at(_birds.size()-1)->runAction(Sequence::create(DelayTime::create(0.7),RemoveSelf::create(), NULL));
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
        
        auto bg = this->_spriteBg->getParent();
        bg->stopAllActions();
//        CCLOG("bg->getRotation() %f",bg->getRotation());
        auto rotateAction = RotateBy::create(2.5, 360-bg->getRotation());
        auto easeOutAction = EaseIn::create(rotateAction, 3);
        auto scaleAction = ScaleTo::create(2.5, 1);
        auto easeBigAction = EaseIn::create(scaleAction, 3);
        bg->runAction(Spawn::create(easeOutAction,easeBigAction, NULL));
        for (auto enemy:_enemy)
        {
//            int randNum = rand()%4;
//            if (randNum==0)
//            {
//                enemy->runAction(Sequence::create(MoveBy::create(2, Vec2(750 - enemy->getPositionX(),0)),RemoveSelf::create(), NULL));
//            }
//            else if (randNum==1)
//            {
//                enemy->runAction(Sequence::create(MoveBy::create(2, Vec2(enemy->getPositionX() + 20,0)),RemoveSelf::create(), NULL));
//            }
//            else if (randNum==2)
//            {
//                enemy->runAction(Sequence::create(MoveBy::create(2, Vec2(0,750 - enemy->getPositionY())),RemoveSelf::create(), NULL));
//            }
//            else if (randNum==3)
//            {
//                enemy->runAction(Sequence::create(MoveBy::create(2, Vec2(0,20 + enemy->getPositionY())),RemoveSelf::create(), NULL));
//            }
            auto big = ScaleTo::create(0.7, 2);
            auto fadeOut = FadeOut::create(0.7);
            enemy->runAction(Sequence::create(Spawn::create(big,fadeOut, NULL),RemoveSelf::create(), NULL));
        }
        _enemy.clear();
        for (auto life:_life)
        {
            life->runAction(Sequence::create(FadeOut::create(2),RemoveSelf::create(), NULL));
        }
        _life.clear();
        //其实下边这个没啥用。死了可能还大于2个白块
        if (_birds.size()>3)
        {
            _birds.at(3)->runAction(Sequence::create(FadeOut::create(2),RemoveSelf::create(), NULL));
            _birds.erase(3);
        }
        if (_birds.size()>2)
        {
            _birds.at(2)->runAction(Sequence::create(FadeOut::create(2),RemoveSelf::create(), NULL));
            _birds.erase(2);
        }
        auto big = ScaleTo::create(0.8,1.5);
        auto fadeOut = FadeOut::create(0.8);
        _birds.at(0)->getBirdSprite()->runAction(Sequence::create(Spawn::create(big,fadeOut, NULL),CallFunc::create([&](){
            _birds.at(0)->setPosition(Vec2(50, 50));
            _birds.at(0)->getBirdSprite()->setOpacity(255);
            _birds.at(0)->getBirdSprite()->setScale(1);
        }), NULL));
        _birds.at(0)->runAction(Sequence::create(DelayTime::create(0.8),MoveTo::create(1.2, Vec2(this->_spriteBg->getContentSize().width/2+100,this->_spriteBg->getContentSize().height/2)), NULL));
        
        
        auto big1 = ScaleTo::create(0.8,1.5);
        auto fadeOut1 = FadeOut::create(0.8);
        _birds.at(1)->getBirdSprite()->runAction(Sequence::create(Spawn::create(big1,fadeOut1, NULL),CallFunc::create([&](){
            _birds.at(1)->setPosition(Vec2(680, 680));
            _birds.at(1)->getBirdSprite()->setOpacity(255);
            _birds.at(1)->getBirdSprite()->setScale(1);
        }), NULL));
        _birds.at(1)->runAction(Sequence::create(DelayTime::create(0.8),MoveTo::create(1.2, Vec2(this->_spriteBg->getContentSize().width/2-100,this->_spriteBg->getContentSize().height/2)), NULL));
//        _birds.at(1)->runAction(Sequence::create(FadeOut::create(0.8),MoveTo::create(0.01, Vec2(680, 680)),FadeIn::create(0.01),MoveTo::create(1.2, Vec2(this->_spriteBg->getContentSize().width/2-100,this->_spriteBg->getContentSize().height/2)), NULL));
        
        _isGameOver = false;
        _pauseBtn->runAction(Sequence::create(DelayTime::create(2),FadeIn::create(1), NULL));
        this->getChildByName("hightScoreLabel")->runAction(Sequence::create(DelayTime::create(0.5),FadeOut::create(1),RemoveSelf::create(), NULL));
        this->getChildByName("info")->runAction(Sequence::create(DelayTime::create(0.5),FadeOut::create(1),RemoveSelf::create(), NULL));
        this->getChildByName("btnSet")->runAction(Sequence::create(DelayTime::create(0.5),FadeOut::create(1),RemoveSelf::create(), NULL));
        this->getChildByName("gameover")->runAction(Sequence::create(DelayTime::create(0.5),FadeOut::create(1),RemoveSelf::create(), NULL));
        this->getChildByName("restart")->runAction(Sequence::create(DelayTime::create(0.5),FadeOut::create(1),RemoveSelf::create(), NULL));
        _timeLabel->runAction(Sequence::create(DelayTime::create(0.5),FadeOut::create(1),CallFunc::create([&](){
            _timeLabel->setString("0.00s");
            _timeLabel->setPosition(15,Director::getInstance()->getVisibleSize().height - 10);
            _timeLabel->setAnchorPoint(Vec2(0, 1));
            _timeLabel->setTextColor(Color4B(252,233,219,255));
            _timeLabel->setLocalZOrder(10);
            _timeLabel->runAction(Sequence::create(DelayTime::create(1),FadeIn::create(1), NULL));
        }), NULL));
        _musicBtn->runAction(MoveTo::create(0.15, Vec2(Director::getInstance()->getVisibleSize().width+50, Director::getInstance()->getVisibleSize().height-65)));
        isMusicBtnOut = false;
        dynamic_cast<Button*>(this->getChildByName("btnSet"))->runAction(Sequence::create(FadeOut::create(2),RemoveSelf::create(), NULL));
        _gameTimes = 0;
        _jianCePinLv = 0;
        _playTime = 0;
        _socre = 0;
        _totalScore = 0;
        _nextTimeCreateEnemy = 1.5f;
        
        
        

//        _timeLabel 
//        _spriteBg = nullptr;
        
        CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/gamestart.wav");//gamestart.wav
        this->runAction(Sequence::create(DelayTime::create(2.5),CallFunc::create([&](){
//            auto scene = GameScene::createScene();
//            auto actionScene = TransitionCrossFade::create(0.5, scene);
//            Director::getInstance()->replaceScene(actionScene);
            _pauseBtn->setTouchEnabled(true);
            this->_spriteBg->getParent()->setRotation(0);
            
            auto listener = EventListenerTouchOneByOne::create();
            listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
            listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
            listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
            Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

            this->getScheduler()->scheduleUpdate(this,0,false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/back.wav",true);
#else
            CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/back.mp3",true);
#endif
        }),NULL));
        dynamic_cast<Button*>(ref)->setTouchEnabled(false);
    }

}
void GameScene::addLife(int num)
{
    if (num>0)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/levelup.wav");
#else
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/levelup.mp3");
#endif
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
            
            this->getChildByName("PauseLayer")->getChildByTag(104)->setVisible(false);
            auto countDown = Label::createWithSystemFont("3","Arial",140);
            countDown->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2,Director::getInstance()->getVisibleSize().height/2));
            countDown->setName("countDown");

            countDown->runAction(Spawn::create(FadeOut::create(0.7f),ScaleTo::create(0.7f, 2), NULL));
            this->getChildByName("PauseLayer")->addChild(countDown);
            this->runAction(Sequence::create(DelayTime::create(0.7f),CallFunc::create([&](){
                
                auto countDownLabel =dynamic_cast<Label*>(this->getChildByName("PauseLayer")->getChildByName("countDown"));
                countDownLabel->setString("2");
                countDownLabel->stopAllActions();
                countDownLabel->setScale(1);
                countDownLabel->setOpacity(255);
                countDownLabel->runAction(Spawn::create(FadeOut::create(0.7f),ScaleTo::create(0.7f, 2), NULL));
                
                
            }),DelayTime::create(0.7f),CallFunc::create([&](){
                auto countDownLabel =dynamic_cast<Label*>(this->getChildByName("PauseLayer")->getChildByName("countDown"));
                countDownLabel->setString("1");
                countDownLabel->setScale(1);
                countDownLabel->setOpacity(255);
                countDownLabel->stopAllActions();
                countDownLabel->runAction(Spawn::create(FadeOut::create(0.7f),ScaleTo::create(0.7f, 2), NULL));
                
            }), DelayTime::create(0.7f),CallFunc::create([&](){
                
                auto countDownLabel =dynamic_cast<Label*>(this->getChildByName("PauseLayer")->getChildByName("countDown"));
                countDownLabel->removeFromParentAndCleanup(true);
                CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
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
            }),NULL));
            
        }
        else if(tag == 105)//暂停
        {
            Director::getInstance()->end();
            
        }
//        CCLOG("btnSetCallback");
//        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("music/d.wav", false);
    }
}
void GameScene::pauseGame()
{
    CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    auto closeBtn= Button::create("res/ui/closebtn.png");
    closeBtn->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2, Director::getInstance()->getVisibleSize().height/4));
    closeBtn->setTag(105);
    closeBtn->addTouchEventListener(CC_CALLBACK_2(GameScene::btnSetCallback, this));
    colorLayer->addChild(closeBtn);
#endif


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
    btn->setName("restart");
    btn->addTouchEventListener(CC_CALLBACK_2(GameScene::btnCallback, this));
    btn->setPosition(Vec2(Director::getInstance()->getVisibleSize().width/2, Director::getInstance()->getVisibleSize().height/4));
    this->addChild(btn);
    btn->setOpacity(0);
    btn->runAction(FadeIn::create(0.8));
    btn->setLocalZOrder(10);
    
    auto gameOverSp = Sprite::create("res/ui/gameover.png");
    gameOverSp->setPosition(Director::getInstance()->getVisibleSize().width/2, Director::getInstance()->getVisibleSize().height/2-50);
    gameOverSp->setName("gameover");
    gameOverSp->setOpacity(0);
    gameOverSp->runAction(FadeIn::create(0.8));
    gameOverSp->setLocalZOrder(10);
    this->addChild(gameOverSp);
    
    auto btnSet = Button::create("res/ui/setting.png");
    btnSet->setName("setting");
    btnSet->addTouchEventListener(CC_CALLBACK_2(GameScene::btnSetCallback, this));
    btnSet->setPosition(Vec2(Director::getInstance()->getVisibleSize().width - 65, Director::getInstance()->getVisibleSize().height-65));
    btnSet->setName("btnSet");
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
    btnInfo->setName("info");

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
    hightScoreLabel->setName("hightScoreLabel");
    
}
bool GameScene::isGameOver()
{
    return _isGameOver;
}
//void GameScene::setGLProgramBlur(Node * node)
//{
////    node->set
//    Sprite * sp = dynamic_cast<Sprite*>(node);
//    GLchar * fragSource = (GLchar*) String::createWithContentsOfFile(
//                                                                     FileUtils::getInstance()->fullPathForFilename("shaders/example_Blur.fsh").c_str())->getCString();
//    auto program = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, fragSource);
//    
//    auto glProgramState = GLProgramState::getOrCreateWithGLProgram(program);
//    sp->setGLProgramState(glProgramState);
//    
//    auto size = sp->getTexture()->getContentSizeInPixels();
//    sp->getGLProgramState()->setUniformVec2("resolution", size);
//    sp->getGLProgramState()->setUniformFloat("blurRadius", 0);
//    sp->getGLProgramState()->setUniformFloat("sampleNum",0);
//}
//void GameScene::setGLProgramBack(Node * node)
//{
//    node->setGLProgramState(_gLProgramState);
//}
void GameScene::gameOver()
{
    _isGameOver = true;
    auto bg = this->_spriteBg->getParent();
    auto rotateAction = RotateBy::create(8, -120);
    auto easeOutAction = EaseOut::create(rotateAction, 1.6);
    auto scaleAction = ScaleTo::create(8, 3);
    auto easeBigAction = EaseOut::create(scaleAction, 1.6);
    bg->runAction(Spawn::create(easeOutAction,easeBigAction, NULL));
    bg->runAction(Sequence::create(DelayTime::create(1.5),CallFunc::create(CC_CALLBACK_0(GameScene::createFuncCallback, this)),NULL));
    _timeLabel->runAction(FadeOut::create(0.8));
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) //WP8平台不支持mp3格式，使用wav，分平台编译
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/die.wav");
#else
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("music/die.mp3");
#endif
    
    //GameScene::blurSchluder
//    for (auto bird:_birds)
//    {
//        this->setGLProgramBlur(bird->getBirdSprite());
//    }
//    for (auto life:_life)
//    {
//        this->setGLProgramBlur(life);
//    }
//    this->setGLProgramBlur(_bgUP);
//    for (auto enemy:_enemy)
//    {
//        this->setGLProgramBlur(enemy);
//    }
    
//    this->getScheduler()->schedule(schedule_selector(GameScene::blurSchluder),this,0.1, 20,0,false);
//    _gLProgramState = _bgUP->getGLProgramState();
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
//    CCLOG("num %d baoliu %d",num,baoliu);
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
        sp->runAction(Sequence::create(fadeInAction,delAction,shanAction,DelayTime::create(0.5),call,removeSelf, NULL));
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