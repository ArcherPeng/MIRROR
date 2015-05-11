//
//  BirdBase.cpp
//  SomeBirds
//
//  Created by ArcherPeng on 15/4/29.
//
//

#include "BirdBase.h"

////////////////////////////////////////////////////////////////////////
//  BirdBase基类
////////////////////////////////////////////////////////////////////////
BirdBase::BirdBase()
{
    _stopMove = false;
    _birdBody = nullptr;
};
BirdBase::~BirdBase(){};
BirdBase * BirdBase::createBird(int type)
{

    switch (type) {
        case 1:
            return BirdOne::create();
            break;
        case 2:
            return BirdTwo::create();
            break;
        case 3:
            return BirdThree::create();
            break;
        case 4:
            return BirdFour::create();
            break;
        default:
            return BirdBase::create();
            break;
    }
}
Sprite* BirdBase::getBirdSprite()const
{
    return _birdBody;
}
int BirdBase::getBirdType()
{
    return 1;
}
bool BirdBase::init()
{
    if (!Layer::init())
    {
        return false;
    }
    this->_birdBody = this->createBirdBody();
    this->addChild(_birdBody);
    _birdBody->setPosition(Vec2(_birdBody->getContentSize().width/2, _birdBody->getContentSize().height/2));
    this->setContentSize(_birdBody->getContentSize());
    return true;
}

Sprite * BirdBase::createBirdBody()
{
    return Sprite::create(this->getBirdTexture());
}
void BirdBase::setPosition(const Vec2 &position)
{
    this->setPosition(position.x, position.y);
}
void BirdBase::setPosition(float x , float y )
{
    Layer::setPosition(x-_birdBody->getContentSize().width/2,y-_birdBody->getContentSize().height/2);
}
void BirdBase::setPosition(const Vec3 &position)
{
    this->setPosition(position.x, position.y);
}
Vec3 BirdBase::getPosition3D()const
{
    auto vecP = Layer::getPosition();
    float x = vecP.x+_birdBody->getContentSize().width/2;
    float y = vecP.y+_birdBody->getContentSize().height/2;
    return Vec3(x, y,0);
}
void BirdBase::setPosition3D(const Vec3 & position)
{
//    CCLOG("setPosition3D  %f  %f",position.x,position.y);
    this->setPosition(position);
}
const Vec2& BirdBase::getPosition() const
{
    auto vecP = Layer::getPosition();
    float x = vecP.x+_birdBody->getContentSize().width/2;
    float y = vecP.y+_birdBody->getContentSize().height/2;
    return *(new Vec2(x, y));
}
void BirdBase::setPositionAsVec2(float x , float y )
{
    if(getStopMove())
    {
        return;
    }
    auto vecPosition = this->getPosition();
    this->setPosition(this->checkPosition(vecPosition.x + x ,vecPosition.y + y ,vecPosition));
}
Vec2 BirdBase::checkPosition(float x , float y ,Vec2 vec)
{
    if (x-this->_birdBody->getContentSize().width/2<0 + 20)
    {
        x =this->_birdBody->getContentSize().width/2 + 20;
    }
    if (x+this->_birdBody->getContentSize().width/2>730 - 20)
    {
        x =730 - this->_birdBody->getContentSize().width/2 - 20;
    }
    if (y-this->_birdBody->getContentSize().height/2<0+20)
    {
        y =this->_birdBody->getContentSize().height/2 + 20;
    }
    if (y+this->_birdBody->getContentSize().width/2>730 - 20)
    {
        y =730 - this->_birdBody->getContentSize().height/2 - 20;
    }
    if(x <150 &&y <150)
    {
        if(Vec2(150,150).distance(Vec2(x,y))>130 - this->_birdBody->getContentSize().height*1.3/2)
        {
            return vec;
        }
    }
    else if(x >580 &&y <150)
    {
        if(Vec2(580,150).distance(Vec2(x,y))>130 - this->_birdBody->getContentSize().height*1.3/2)
        {
            return vec;
        }
    }
    else if(x <150 &&y >580)
    {
        if(Vec2(150,580).distance(Vec2(x,y))>130 - this->_birdBody->getContentSize().height*1.3/2)
        {
            return vec;
        }
    }
    else if(x >580 &&y >580)
    {
        if(Vec2(580,580).distance(Vec2(x,y))>130 - this->_birdBody->getContentSize().height*1.3/2)
        {
            return vec;
        }
    }
    
    return Vec2(x, y);
}
void BirdBase::setStopMove(bool isStopMove)
{
    this->_stopMove = isStopMove;
}
bool BirdBase::getStopMove() const
{
    return this->_stopMove;
}
std::string BirdBase::getBirdTexture()
{
    return "res/bird/bird_1.png";
}
Vec2 BirdBase::getXiangDuiVec2()
{
    Vec2 vec;
    vec.x = this->getPosition().x-365;
    vec.y = this->getPosition().y-365;
    return vec;
}
Vec2 BirdBase::getInitPosition(Vec2 pos)
{
    pos.x+= 365;
    pos.y+= 365;
    return pos;
}

////////////////////////////////////////////////////////////////////////
//  BirdOne
////////////////////////////////////////////////////////////////////////

std::string BirdOne::getBirdTexture()
{
    return "res/bird/bird_1.png";
}
int BirdOne::getBirdType()
{
    return 1;
}
void BirdOne::setPositionAsVec2(float x , float y )
{
    if(getStopMove())
    {
        return;
    }
    auto vecPosition = this->getPosition();
    this->setPosition(this->checkPosition(vecPosition.x + x ,vecPosition.y + y ,vecPosition));
}
Vec2 BirdOne::getXiangDuiVec2()
{
    Vec2 vec;
    vec.x = this->getPosition().x-365;
    vec.y = this->getPosition().y-365;
    
    return vec;
}
Vec2 BirdOne::getInitPosition(Vec2 pos)
{
    pos.x+= 365;
    pos.y+= 365;
    return pos;
}

////////////////////////////////////////////////////////////////////////
//  BirdTwo
////////////////////////////////////////////////////////////////////////

int BirdTwo::getBirdType()
{
    return 2;
}
std::string BirdTwo::getBirdTexture()
{
    return "res/bird/bird_2.png";
}
void BirdTwo::setPositionAsVec2(float x , float y )
{
    if(getStopMove())
    {
        return;
    }
    auto vecPosition = this->getPosition();
    this->setPosition(this->checkPosition(vecPosition.x + (-x) ,vecPosition.y + (-y) ,vecPosition));
}
Vec2 BirdTwo::getXiangDuiVec2()
{
    Vec2 vec;
    vec.x = -this->getPosition().x-365;
    vec.y = -this->getPosition().y-365;
    
    return vec;
}
Vec2 BirdTwo::getInitPosition(Vec2 pos)
{
    
    return Vec2(-pos.x+365,-pos.y+365);
}
////////////////////////////////////////////////////////////////////////
//  BirdThree
////////////////////////////////////////////////////////////////////////
int BirdThree::getBirdType()
{
    return 3;
}
std::string BirdThree::getBirdTexture()
{
    return "res/bird/bird_1.png";
}

void BirdThree::setPositionAsVec2(float x , float y )
{
    if(getStopMove())
    {
        return;
    }
    auto vecPosition = this->getPosition();
    float xx = y;
    float yy = x;
    this->setPosition(this->checkPosition(vecPosition.x + xx ,vecPosition.y + yy ,vecPosition));
}
Vec2 BirdThree::getXiangDuiVec2()
{
    Vec2 vec;
    vec.x = this->getPosition().y-365;
    vec.y = this->getPosition().x-365;
    
    return vec;
}
Vec2 BirdThree::getInitPosition(Vec2 pos)
{
    return Vec2(pos.y+365,pos.x+365);
}

////////////////////////////////////////////////////////////////////////
//  BirdTwo
////////////////////////////////////////////////////////////////////////
int BirdFour::getBirdType()
{
    return 4;
}

std::string BirdFour::getBirdTexture()
{
    return "res/bird/bird_2.png";
}
void BirdFour::setPositionAsVec2(float x , float y )
{
    if(getStopMove())
    {
        return;
    }
    auto vecPosition = this->getPosition();
    float xx = y;
    float yy = x;
    this->setPosition(this->checkPosition(vecPosition.x + (-xx) ,vecPosition.y + (-yy),vecPosition ));
}
Vec2 BirdFour::getXiangDuiVec2()
{
    Vec2 vec;
    vec.x = -this->getPosition().y-365;
    vec.y = -this->getPosition().x-365;
    
    return vec;
}
Vec2 BirdFour::getInitPosition(Vec2 pos)
{
    return Vec2(-pos.y+365,-pos.x+365);
}



















