//
//  IOSiAP_Bridge.h
//  LongChengDaRen
//
//  Created by 白白 on 14-11-11.
//
//

#ifndef __LongChengDaRen__IOSiAP_Bridge__
#define __LongChengDaRen__IOSiAP_Bridge__

#include "IOSiAP.h"
#include "cocos2d.h"
class IOSiAP_Bridge : public IOSiAPDelegate, public cocos2d::Ref
{
public:
    IOSiAP_Bridge();
    ~IOSiAP_Bridge();
    IOSiAP *iap;
    int productID;
    void requestProducts(int);
    virtual void onRequestProductsFinish(void);
    virtual void onRequestProductsError(int code);
    virtual void onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event, int quantity);
    
    void registerHandler(int handler);
    void unregisterHandler();
private:
    int _scriptAttackHandle;
};
#endif /* defined(__LongChengDaRen__IOSiAP_Bridge__) */
