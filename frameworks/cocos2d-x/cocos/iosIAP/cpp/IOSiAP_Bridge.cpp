//
//  IOSiAP_Bridge.cpp
//  LongChengDaRen
//
//  Created by 白白 on 14-11-11.
//
//

#include "IOSiAP_Bridge.h"

USING_NS_CC;

IOSiAP_Bridge::IOSiAP_Bridge()
{
    iap = new IOSiAP();
    _scriptAttackHandle = 0;
    iap->delegate = this;
}

IOSiAP_Bridge::~IOSiAP_Bridge()
{
    delete iap;
}

void IOSiAP_Bridge:: requestProducts(int id)
{
    productID = id;
    std::vector<std::string> product;
    product.push_back("com.u1xs.icefireworld.60jinlong");
    product.push_back("com.u1xs.icefireworld.300jinlong");
    product.push_back("com.u1xs.icefireworld.980jinlong");
    product.push_back("com.u1xs.icefireworld.1980jinlong");
    product.push_back("com.u1xs.icefireworld.3280jinlong");
    product.push_back("com.u1xs.icefireworld.6480jinlong");
    product.push_back("com.u1xs.icefireworld.yueka");
    product.push_back("com.u1xs.icefireworld.yongjiuyueka");
	//把需要付费的道具的所有product id都放到容器里面传进去
    iap->requestProducts(product);
}

void IOSiAP_Bridge::onRequestProductsFinish(void)
{
    std::string identifier = "";
    switch (productID) {
        case 6:
            identifier = "com.u1xs.icefireworld.60jinlong";
            break;
        case 30:
            identifier = "com.u1xs.icefireworld.300jinlong";
            break;
        case 98:
            identifier = "com.u1xs.icefireworld.980jinlong";
            break;
        case 198:
            identifier = "com.u1xs.icefireworld.1980jinlong";
            break;
        case 328:
            identifier = "com.u1xs.icefireworld.3280jinlong";
            break;
        case 648:
            identifier = "com.u1xs.icefireworld.6480jinlong";
            break;
        case 25:
            identifier = "com.u1xs.icefireworld.yueka";
            break;
        case 50:
            identifier = "com.u1xs.icefireworld.yongjiuyueka";
            break;
        default:
            break;
    }

    //必须在onRequestProductsFinish后才能去请求iAP产品数据。
    IOSProduct *product = iap->iOSProductByIdentifier(identifier);
    // 然后可以发起付款请求。,第一个参数是由iOSProductByIdentifier获取的IOSProduct实例，第二个参数是购买数量
    iap->paymentWithProduct(product, 1);
}

void IOSiAP_Bridge::onRequestProductsError(int code)
{
    //这里requestProducts出错了，不能进行后面的所有操作。
    log("付款失败");
}

void IOSiAP_Bridge::onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event, int quantity)
{
    
    if (event == IOSIAP_PAYMENT_PURCHAED) {
        //付款成功了，可以吧金币发给玩家了。
		//根据传入的参数就能知道购买的是哪种类型的金币
        switch (productID) {
            case 6:
            {
                CommonScriptData data(_scriptAttackHandle, "success", this);
                
                ScriptEvent event(kCommonEvent,(void*)&data);
                ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
                break;
            }
            case 30:
            {
                CommonScriptData data(_scriptAttackHandle, "success", this);
                
                ScriptEvent event(kCommonEvent,(void*)&data);
                ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
                break;
            }
            case 98:                
            {
                CommonScriptData data(_scriptAttackHandle, "success", this);
                
                ScriptEvent event(kCommonEvent,(void*)&data);
                ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
                break;
            }
            case 198:
               
            {
                CommonScriptData data(_scriptAttackHandle, "success", this);
                
                ScriptEvent event(kCommonEvent,(void*)&data);
                ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
                break;
            }
            case 328:
                
            {
                CommonScriptData data(_scriptAttackHandle, "success", this);
                
                ScriptEvent event(kCommonEvent,(void*)&data);
                ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
                break;
            }
            case 648:
                
            {
                CommonScriptData data(_scriptAttackHandle, "success", this);
                
                ScriptEvent event(kCommonEvent,(void*)&data);
                ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
                break;
            }
            case 25:
                
            {
                CommonScriptData data(_scriptAttackHandle, "success", this);
                
                ScriptEvent event(kCommonEvent,(void*)&data);
                ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
                break;
            }
            case 50:
                
            {
                CommonScriptData data(_scriptAttackHandle, "success", this);
                
                ScriptEvent event(kCommonEvent,(void*)&data);
                ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
                break;
            }
            default:
                break;
        }        
        log("付款成功");
        
    }
    else if(IOSIAP_PAYMENT_FAILED == event)
    {
        CommonScriptData data(_scriptAttackHandle, "failed", this);
        
        ScriptEvent event(kCommonEvent,(void*)&data);
        ScriptEngineManager::getInstance()->getScriptEngine()->sendEvent(&event);
    }
    else if (event == IOSIAP_PAYMENT_REMOVED)
    {
        
    }
    //其他状态依情况处理掉。
}

void IOSiAP_Bridge::registerHandler(int handler)
{
    unregisterHandler();
    _scriptAttackHandle = handler;
}
void IOSiAP_Bridge::unregisterHandler()
{
    if (_scriptAttackHandle)
    {
        ScriptEngineManager::getInstance()->getScriptEngine()->removeScriptHandler(_scriptAttackHandle);
        _scriptAttackHandle = 0;
    }
}
