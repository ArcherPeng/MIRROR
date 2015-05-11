//
//  BirdBase.h
//  SomeBirds
//
//  Created by ArcherPeng on 15/4/29.
//
//

#ifndef __SomeBirds__BirdBase__
#define __SomeBirds__BirdBase__

#include "cocos2d.h"
USING_NS_CC;
class BirdBase:public Layer
{
    
protected:
    virtual bool init();
    bool _stopMove = false;
    Sprite * _birdBody;
    virtual Sprite * createBirdBody();
    
    virtual std::string getBirdTexture();
public:
    virtual void setStopMove(bool isStopMove);
    virtual bool getStopMove()const;
    static BirdBase * createBird(int type);
    CREATE_FUNC(BirdBase)
    BirdBase();
    ~BirdBase();
//    Sprite * getBirdSprite
    virtual void setPosition(const Vec2 &position);
    virtual void setPosition(float x , float y );
    virtual void setPosition(const Vec3 &position);
    virtual void setPosition3D(const Vec3 & position);
    virtual Vec3 getPosition3D()const;
    virtual void setPositionAsVec2(float x , float y );
    const Vec2& getPosition() const;
    Vec2 checkPosition(float x , float y,Vec2 vec );
    virtual int getBirdType();
    virtual Vec2 getXiangDuiVec2();
    virtual Vec2 getInitPosition(Vec2 pos);
    
};

class BirdOne:public BirdBase
{
    
protected:
    virtual std::string getBirdTexture();
public:
    CREATE_FUNC(BirdOne)
    BirdOne(){};
    ~BirdOne(){};
    virtual void setPositionAsVec2(float x , float y );
    virtual int getBirdType();
    virtual Vec2 getXiangDuiVec2();
    virtual Vec2 getInitPosition(Vec2 pos);
};
class BirdTwo:public BirdBase
{
    
protected:
    virtual std::string getBirdTexture();
public:
    CREATE_FUNC(BirdTwo)
    BirdTwo(){};
    ~BirdTwo(){};
    virtual void setPositionAsVec2(float x , float y );
    virtual int getBirdType();
    virtual Vec2 getXiangDuiVec2();
    virtual Vec2 getInitPosition(Vec2 pos);
};
class BirdThree:public BirdBase
{
    
protected:
    virtual std::string getBirdTexture();
public:
    CREATE_FUNC(BirdThree)
    BirdThree(){};
    ~BirdThree(){};
    virtual void setPositionAsVec2(float x , float y );
    virtual int getBirdType();
    virtual Vec2 getXiangDuiVec2();
    virtual Vec2 getInitPosition(Vec2 pos);
};
class BirdFour:public BirdBase
{
    
protected:
    virtual std::string getBirdTexture();
public:
    CREATE_FUNC(BirdFour)
    BirdFour(){};
    ~BirdFour(){};
    virtual void setPositionAsVec2(float x , float y );
    virtual int getBirdType();
    virtual Vec2 getXiangDuiVec2();
    virtual Vec2 getInitPosition(Vec2 pos);
};
#endif /* defined(__SomeBirds__BirdBase__) */
