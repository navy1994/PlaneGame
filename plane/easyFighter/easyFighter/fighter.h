//
//  fighter.h
//  easyFighter
//
//  Created by SKY on 13-8-9.
//
//

#ifndef __easyFighter__fighter__
#define __easyFighter__fighter__

#include <iostream>
#include "cocos2d.h"
using namespace cocos2d;
class Fighter:public CCLayer
{
public:
    
    CCSprite*fighter;//我的战斗机
    CCSprite*diji;//敌机
    CCSprite *zidan;//子弹
    CCSize winSize;
    CCArray *diArray;//敌机数组
    CCArray *zidanArray;//子弹数组
    float diTime;//控制敌机速度
    float diMi;//控制敌机密度
    float zidanTime;//控制子弹速度
    int huai;//记录我的战斗机损毁次数
    int miedi;//记录摧毁敌机数量
    int score;//统计得分
    float zidanda;
    CCLabelAtlas *scoreLabel;
public:
    static CCScene*scene();
    bool init();
    CREATE_FUNC(Fighter);
    
    virtual bool ccTouchBegan(CCTouch *pTouch,CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch,CCEvent *pEvent);
    void onEnter();
    void onExit();
    //自己的方法
    void zidanPengzhuang();
    void pengzhuang();
    void addDiji();
    void addZidan();
    void removeZidan(CCSprite*sp);
    void removeDiji(CCSprite *spdi);
    void removePlayDong(CCSprite *sp);//用来移除播放动画的临时精灵
};
#endif /* defined(__easyFighter__fighter__) */
