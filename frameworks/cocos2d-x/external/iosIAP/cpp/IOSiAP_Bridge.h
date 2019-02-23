//
//  IOSiAP_Bridge.h
//  LongChengDaRen
//
//  Created by 白白 on 14-11-11.
//
//

#ifndef __LongChengDaRen__IOSiAP_Bridge__
#define __LongChengDaRen__IOSiAP_Bridge__

#import "IOSiAP.h"
class IOSiAP_Bridge : public IOSiAPDelegate
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
};
#endif /* defined(__LongChengDaRen__IOSiAP_Bridge__) */
