#include "HelloWorldScene.h"
#include "GameScene.h"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto bgSp = Sprite::create("res/ui/beginbg.png");
    bgSp->setPosition(visibleSize.width/2,visibleSize.height/2);
    this->addChild(bgSp);
    return true;
}
void HelloWorld::onEnter()
{
    Layer::onEnter();
    auto scene = GameScene::createScene();
    auto actionScene = TransitionCrossFade::create(0.5, scene);
    Director::getInstance()->replaceScene(actionScene);
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
