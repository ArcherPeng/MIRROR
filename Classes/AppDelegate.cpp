#include "AppDelegate.h"
#include "SimpleAudioEngine.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include "HelloWorldScene.h"
#endif
#include "GameScene.h"
USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// If you want to use packages manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    srand(static_cast<unsigned>(time(nullptr)));
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Some Birds");
        director->setOpenGLView(glview);
    }
//    glview->setDesignResolutionSize(640, 960, ResolutionPolicy::FIXED_WIDTH);
    glview->setDesignResolutionSize(768, 1152, ResolutionPolicy::FIXED_WIDTH);

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    register_all_packages();

    // create a scene. it's an autorelease object
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) || (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    auto scene = HelloWorld::createScene();
#else
    auto scene = GameScene::createScene();
#endif

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    auto runingScene = Director::getInstance()->getRunningScene();
    auto runingLayer = runingScene->getChildByName("GameScene");
    if (runingLayer &&!dynamic_cast<GameScene*>(runingLayer)->isGameOver())
    {
        dynamic_cast<GameScene*>(runingLayer)->pauseGame();
    }
    else
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    }
    Director::getInstance()->stopAnimation();
    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    auto runingScene = Director::getInstance()->getRunningScene();
    auto runingLayer = runingScene->getChildByName("GameScene");
    if (runingLayer&&!dynamic_cast<GameScene*>(runingLayer)->isGameOver())
    {
    }
    else
    {
        CocosDenshion::SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
