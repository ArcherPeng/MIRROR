//
//  InfoLayer.cpp
//  SomeBirds
//
//  Created by ArcherPeng on 15/5/4.
//
//

#include "InfoLayer.h"
bool InfoLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }
    _step = 0;
    auto visiableSize = Director::getInstance()->getVisibleSize();
    auto colorLayer = LayerColor::create(Color4B(255, 255, 255, 210), visiableSize.width,visiableSize.height);
    colorLayer->setAnchorPoint(Vec2(0, 0));
    colorLayer->setPosition(Vec2::ZERO);
    colorLayer->setTag(100);
    colorLayer->setName("colorLayer");
    this->addChild(colorLayer);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(InfoLayer::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(InfoLayer::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(InfoLayer::onTouchEnded, this);
    listener->setSwallowTouches(true);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    Label * labelName = Label::createWithSystemFont("ЯOЯЯIM\n   镜  像", "Arial", 100);
    labelName->setTextColor(Color4B::BLACK);
    labelName->setPosition(Vec2(visiableSize.width/2, visiableSize.height/4*3+50));
    this->addChild(labelName);
    _infoLabels.pushBack(labelName);
    Label * labelArcher = Label::createWithSystemFont("DESIGNED BY", "Arial", 60);
    labelArcher->setTextColor(Color4B::BLACK);
    labelArcher->setPosition(Vec2(visiableSize.width/2, visiableSize.height/5*3));
    this->addChild(labelArcher);
    _infoLabels.pushBack(labelArcher);
    labelArcher = Label::createWithSystemFont("ArcherPeng", "Arial", 60);
    labelArcher->setTextColor(Color4B::BLACK);
    labelArcher->setPosition(Vec2(visiableSize.width/2, visiableSize.height/5*3-70));
    this->addChild(labelArcher);
    _infoLabels.pushBack(labelArcher);
    labelArcher = Label::createWithSystemFont("(寒江孤叶丶)", "Arial", 60);
    labelArcher->setTextColor(Color4B::BLACK);
    labelArcher->setPosition(Vec2(visiableSize.width/2, visiableSize.height/5*3-140));
    this->addChild(labelArcher);
    _infoLabels.pushBack(labelArcher);
    labelArcher = Label::createWithSystemFont("Contact me --> QQ:446569365", "Arial", 30);
    labelArcher->setTextColor(Color4B::BLACK);
    labelArcher->setAnchorPoint(Vec2(1, 0));
    labelArcher->setPosition(Vec2(visiableSize.width-5, 5));
    this->addChild(labelArcher);
    _infoLabels.pushBack(labelArcher);
    auto children = this->getChildren();
    for (auto node :children)
    {
        node->setOpacity(0);
    }
    return true;
}
void InfoLayer::onEnter()
{
    Layer::onEnter();
    auto children = this->getChildren();
    for (auto node :children)
    {
        if (node->getTag() == 100) {
            node->runAction(FadeTo::create(0.5, 210));
        }
        else
        {
            
            node->runAction(FadeIn::create(0.5));
        }
    }
}
bool InfoLayer::onTouchBegan(Touch* touch, Event*event)
{
    return true;
}
void InfoLayer::onTouchMoved(Touch* touch, Event*event)
{
    
}
void InfoLayer::onTouchEnded(Touch* touch, Event*event)
{
    for (auto node :_infoLabels)
    {
        node->runAction(Sequence::create(FadeOut::create(0.5),RemoveSelf::create(), NULL));
    }
    for (auto node :_thankLabels)
    {
        node->runAction(Sequence::create(FadeOut::create(0.5),RemoveSelf::create(), NULL));
    }
    _infoLabels.clear();
    _thankLabels.clear();
    if (_step == 0)
    {
        auto visiableSize = Director::getInstance()->getVisibleSize();
        Label * labelName = Label::createWithSystemFont("Special Thank", "Arial", 100);
        labelName->setTextColor(Color4B::BLACK);
        labelName->setPosition(Vec2(visiableSize.width/2, visiableSize.height/4*3+50));
        this->addChild(labelName);
        _thankLabels.pushBack(labelName);
        Label * labelArcher = Label::createWithSystemFont("lvyile,Schrodinger,断翼丶,田里千,\n虾米,大橙子", "Arial", 60);
        labelArcher->setTextColor(Color4B::BLACK);
        labelArcher->setAnchorPoint(Vec2(0.5,1));
        labelArcher->setPosition(Vec2(visiableSize.width/2, visiableSize.height/5*3));
        labelArcher->setHorizontalAlignment(TextHAlignment::CENTER);
        labelArcher->setDimensions(visiableSize.width - 100, 0);
        this->addChild(labelArcher);
        _thankLabels.pushBack(labelArcher);
        
        for (auto node :_thankLabels)
        {
            node->setOpacity(0);
            node->runAction(Sequence::create(DelayTime::create(0.5),FadeIn::create(0.8), NULL));
        }
    }
    
    if (_step == 1)
    {
        this->getChildByName("colorLayer")->runAction(Sequence::create(FadeOut::create(0.5),RemoveSelf::create(), NULL));
        this->runAction(Sequence::create(DelayTime::create(0.5),RemoveSelf::create(),NULL));
    }
    ++_step;
}
