#ifndef  __NOTIFICATION_DIRECTOR_H__
#define  __NOTIFICATION_DIRECTOR_H__

#include "cocos2d.h"
#include <map>
#include <string>

typedef struct _NotificationItem
{
	std::string noteKey;
	void* object;
}NotificationItem;


class INotificationDelegate;
class NotificationDirector :public cocos2d::CCObject
{
public:
	virtual ~NotificationDirector();
	static NotificationDirector* getInstance();

	void registerNotification(const char* noteKey, INotificationDelegate* delegate );
	void removeNotification(const char* noteKey, INotificationDelegate* delegate);
	void removeNotification(const char* noteKey);

	void post(const char* noteKey, void* object, bool immediate = true);

	void step(float dt);

private:
	void doPost(NotificationItem& item);
	static NotificationDirector* _instance;
	NotificationDirector();
	std::multimap<std::string, INotificationDelegate*> _delegateMap;
	std::vector<NotificationItem>_itemList;
    std::vector<NotificationItem>_itemListBackupOnLock;
    bool _itemPostLock;
};


#endif