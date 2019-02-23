#ifndef  __PVP_MANAGER__H__
#define  __PVP_MANAGER__H__

#include "cocos2d.h"
#include <map>
#include "network/HttpClient.h"

NS_CC_BEGIN

class PvpManager : public Ref
{
public:
	static PvpManager* getInstance();
	void setComputeReady();
	//1: ready to computer  2: computing 3: data is ready
	int getManagerState();
	std::string getBattleData();
	std::string getComputerData();
	void startCompute(const char* battleData);
	void setComputerResult(const char* result);
	void registerStartBattleHandler(int handler);
	void unregisterStartBattleHandler();

	std::string bakFunction(const char* str);
    
	void registerHandler(int handler); 
	void unregisterHandler();
    
    void registerHandler1(int handler);
    void unregisterHandler1();
    
    void registerHandler2(int handler);
    void unregisterHandler2();
    
    
    std::string bakFunction1(const char* str);
    std::string bakFunction2(const char* str);
    std::string bakFunction3(const char* str);

    
    char* backupFun(Node* arg);
    char* backupFun1(Node* arg);
    
    char* backupFunction(Node* arg, const char* str);
    char* backupFunction1(Node* arg, const char* str);


	void talkingDataResumeSetAccount();
    
private:
	PvpManager();
	static PvpManager* _instance;
	int _startBattleHandler;
	int _handler;
    int _handler1;
    int _handler2;

};
NS_CC_END
#endif