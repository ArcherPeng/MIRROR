//
//  InfoLayer.h
//  SomeBirds
//
//  Created by ArcherPeng on 15/5/4.
//
//

#ifndef __SomeBirds__InfoLayer__
#define __SomeBirds__InfoLayer__

#include "cocos2d.h"
USING_NS_CC;
class InfoLayer:public Layer
{
protected:
    Vector<Label*> _infoLabels;
    Vector<Label*> _thankLabels;
    int _step;
    virtual bool init();
    bool onTouchBegan(Touch* touch, Event*event);
    void onTouchMoved(Touch* touch, Event*event);
    void onTouchEnded(Touch* touch, Event*event);
public:
    void onEnter();

    static cocos2d::Scene* createScene();
    
    CREATE_FUNC(InfoLayer);
};

#endif /* defined(__SomeBirds__InfoLayer__) */
