//
//  fighter.cpp
//  easyFighter
//
//  Created by SKY on 13-8-9.
//
//

#include "Fighter.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;
bool touchFlag;
bool dongFlag=true;

CCScene* Fighter::scene()
{
    CCScene*scene=CCScene::create();
    Fighter*layer=Fighter::create();
    scene->addChild(layer);
    return scene;
}

bool Fighter::init()
{
    if (!CCLayer::init())
    {
        return false;
    }
    else
    {
        //初始化各变量
        winSize=CCDirector::sharedDirector()->getWinSize();
        diTime=3;
        diMi=0.8f;
        huai=0;
        miedi=0;
        zidanTime=0.2;
        zidanda=1.2f;
        zidanArray = CCArray::create();
        zidanArray->retain();
        diArray = CCArray::create();
        diArray->retain();
        //添加我的飞机和背景
        CCSprite *back=CCSprite::create("back.jpg");
        back->setPosition(ccp(winSize.width/2, winSize.height/2));
        addChild(back,-1,120);
        
        scoreLabel=CCLabelAtlas::create("0", "fps_images.png", 16, 32, '.');
        scoreLabel->setPosition(ccp(30, winSize.height-70));
        scoreLabel->setScale(1.5);
        addChild(scoreLabel);
        
        //创建两张背景图，用于交替出现
        CCSprite *back2=CCSprite::create("back2.jpg");
        back2->setPosition(ccp(winSize.width/2, winSize.height*1.5));
        addChild(back2,-1,121);
        
        fighter=CCSprite::create("fighter.png");
        fighter->setPosition(ccp(winSize.width/2, 50));
        addChild(fighter,0,110);
        
        //创建一个空精灵用来播放动画
        
        //----------------------------------
        
        //预处理音乐
        SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("back.caf");
        SimpleAudioEngine::sharedEngine()->preloadEffect("zidan.caf");
        SimpleAudioEngine::sharedEngine()->preloadEffect("baozha.caf");
        
        SimpleAudioEngine::sharedEngine()->playBackgroundMusic("back.caf", true);
        
        schedule(schedule_selector(Fighter::addZidan), zidanTime);
        schedule(schedule_selector(Fighter::pengzhuang), 0.1);
        schedule(schedule_selector(Fighter::addDiji), diMi);
        schedule(schedule_selector(Fighter::zidanPengzhuang));
        return true;
    }
}

void Fighter::onEnter()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, false);
    this->CCSprite::onEnter();
}

void Fighter::onExit()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    this->CCSprite::onExit();
}

bool Fighter::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint point=pTouch->getLocation();
    CCLog("触摸坐标是：%g,%g",point.x,point.y);
    if (fighter->boundingBox().containsPoint(point))
    {
        touchFlag=true;
    }
    else
    {
        return false;
    }
    return true;
}

void Fighter::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
    CCPoint mpoint=pTouch->getLocation();
    if (touchFlag)
    {
        fighter->setPosition(mpoint);
    }
}
//添加敌机
void Fighter::addDiji()
{
    int a=arc4random()%((int)(winSize.width-1))+1;
    int b=arc4random()%((int)(winSize.width+50))-25;
    diji=CCSprite::create("diji.png");
    diji->setPosition(ccp(a, winSize.height));
    diArray->addObject(diji);
    this->addChild(diji);
    CCMoveTo*moveTo=CCMoveTo::create(diTime, ccp(b, 0));
    CCCallFuncN *funcn=CCCallFuncN::create(this, callfuncN_selector(Fighter::removeDiji));
    CCSequence *seq=CCSequence::create(moveTo,funcn);
    diji->runAction(seq);
}
//添加子弹
void Fighter::addZidan()
{
    zidan=CCSprite::create("bullet.png");
    zidanArray->addObject(zidan);
    zidan->setPosition(fighter->getPosition());
    this->addChild(zidan);
    zidan->setScale(zidanda);
    CCMoveTo *moveTo=CCMoveTo::create(zidanTime, ccp(fighter->getPositionX(), winSize.height));
    CCCallFuncN *funcn=CCCallFuncN::create(this, callfuncN_selector(Fighter::removeZidan));
    CCSequence *seq=CCSequence::create(moveTo,funcn);
    zidan->runAction(seq);
    SimpleAudioEngine::sharedEngine()->playEffect("zidan.caf",false);
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.2f);
}
//移除子弹
void Fighter::removeZidan(cocos2d::CCSprite *sp)
{
    zidanArray->removeObject(sp);
    sp->removeFromParentAndCleanup(true);
}
//移除敌机
void Fighter::removeDiji(cocos2d::CCSprite *spdi)
{
    diArray->removeObject(spdi);
    spdi->removeFromParentAndCleanup(true);
}

void Fighter::pengzhuang()
{
    //战斗机碰撞检测
    CCRect rect=fighter->boundingBox();
    float i=rect.origin.x;
    float j=rect.origin.y;
    float w=rect.size.width;
    float h=rect.size.height;
    CCLog("(%g,%g),(%g,%g)",i,j,w,h);
    for (int i=0; i<diArray->count(); i++)
    {
        CCSprite *diTem=(CCSprite*)diArray->objectAtIndex(i);
        if (fighter->boundingBox().intersectsRect(diTem->boundingBox()))
        {
            diTem->stopAllActions();
            diArray->removeObject(diTem);
            diTem->removeFromParentAndCleanup(true);
            huai++;
            CCLog("我的战斗机爆炸%d次了！",huai);
            CCTexture2D *texture=CCTextureCache::sharedTextureCache()->addImage("mebaozha.png");
            CCAnimation *animation=CCAnimation::create();
            for (int i=0; i<6; i++)
            {
                CCRect rect=CCRectMake(96*i, 0, 96, 96);
                CCSpriteFrame *frame=CCSpriteFrame::createWithTexture(texture, rect);
                animation->addSpriteFrame(frame);
            }
            animation->setLoops(1);
            animation->setDelayPerUnit(0.1f);
            animation->setRestoreOriginalFrame(true);
            CCAnimate* baoAnimate=CCAnimate::create(animation);
            CCSprite *playDong=CCSprite::create();
            playDong->setPosition(fighter->getPosition());
            addChild(playDong);
            CCCallFuncN *calln=CCCallFuncN::create(this, callfuncN_selector(Fighter::removePlayDong));
            CCSequence *se=CCSequence::create(baoAnimate,calln);
            playDong->runAction(se);
//            unscheduleAllSelectors();
        }
    }
}

//检测子弹碰撞
void Fighter::zidanPengzhuang()
{
    if (dongFlag)
    {
        for (int i=0; i<zidanArray->count(); i++)
        {
            CCSprite *zidanTem=(CCSprite*)zidanArray->objectAtIndex(i);
            CCRect zidanRect=zidanTem->boundingBox();
            for (int j=0; j<diArray->count(); j++)
            {
                CCSprite *dijiTem=(CCSprite*)diArray->objectAtIndex(j);
                CCRect dijiRect=dijiTem->boundingBox();
                if (zidanRect.intersectsRect(dijiRect))
                {
                    miedi++;
                    score=score+100;
                    CCString *str=CCString::createWithFormat("%d",score);
                    scoreLabel->setString(str->getCString());
                    SimpleAudioEngine::sharedEngine()->playEffect("baozha.caf",false);
                    SimpleAudioEngine::sharedEngine()->setEffectsVolume(1.2f);
                    CCLog("摧毁敌机%d个！",miedi);
                    zidanTem->stopAllActions();
                    dijiTem->stopAllActions();
                    zidanArray->removeObject(zidanTem);
                    diArray->removeObject(dijiTem);
                    zidanTem->removeFromParentAndCleanup(true);
                    dijiTem->removeFromParentAndCleanup(true);
                    //创建动画 敌机爆炸时用----------------
                    CCTexture2D *texture=CCTextureCache::sharedTextureCache()->addImage("mebaozha.png");
                    CCAnimation *animation=CCAnimation::create();
                    for (int i=0; i<6; i++)
                    {
                        CCRect rect=CCRectMake(96*i, 0, 96, 96);
                        CCSpriteFrame *frame=CCSpriteFrame::createWithTexture(texture, rect);
                        animation->addSpriteFrame(frame);
                    }
                    animation->setLoops(1);
                    animation->setDelayPerUnit(0.08f);
                    animation->setRestoreOriginalFrame(true);
                    CCAnimate* baoAnimate=CCAnimate::create(animation);
                    CCSprite *playDong=CCSprite::create();
                    playDong->setPosition(dijiRect.origin);
                    addChild(playDong);
                    CCCallFuncN *calln=CCCallFuncN::create(this, callfuncN_selector(Fighter::removePlayDong));
                    CCSequence *se=CCSequence::create(baoAnimate,calln);
                    playDong->runAction(se);
                                              
                }
            }
        }

    }
    //检测地图
    CCSprite *backtem=(CCSprite*)this->getChildByTag(120);
    float bacpoint=backtem->getPositionY();
    
    if (bacpoint<=-winSize.height/2)
    {
        backtem->setPosition(ccp(winSize.width/2, winSize.height*1.5-5));
    }
    else
    {
        backtem->setPosition(ccpAdd(backtem->getPosition(), ccp(0, -3)));
    }
    
    CCSprite *backtem2=(CCSprite*)this->getChildByTag(121);
    float bacpoint2=backtem2->getPositionY();
    if (bacpoint2<=-winSize.height/2)
    {
        backtem2->setPosition(ccp(winSize.width/2, winSize.height*1.5-5));
    }
    else
    {
        backtem2->setPosition(ccpAdd(backtem2->getPosition(), ccp(0, -3)));
    }
}
void Fighter::removePlayDong(cocos2d::CCSprite *sp)
{
    sp->removeFromParentAndCleanup(true);
}