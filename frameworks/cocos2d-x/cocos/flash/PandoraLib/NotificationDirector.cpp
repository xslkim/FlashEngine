#include "NotificationDirector.h"
#include "Notifications.h"


using namespace cocos2d;

NotificationDirector* NotificationDirector::_instance = 0;

NotificationDirector* NotificationDirector::getInstance()
{	
	if( _instance == 0)
	{
		_instance = new NotificationDirector();
	}
	return _instance;
}

NotificationDirector::NotificationDirector()
{
	_itemPostLock = false;
}

NotificationDirector::~NotificationDirector()
{
	CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(NotificationDirector::step), this);
}


void NotificationDirector::step(float dt)
{
	std::vector<NotificationItem>::iterator it = _itemList.begin();
	while(it != _itemList.end())
	{
		doPost(*it);
		_itemList.erase(it);
		it = _itemList.begin();
	}
    
    for(int i = 0; i < _itemListBackupOnLock.size(); ++i )
    {
        _itemList.push_back( _itemListBackupOnLock[i] );
    }
    _itemListBackupOnLock.clear();
}

void NotificationDirector::registerNotification(const char* noteKey, INotificationDelegate* delegate )
{
	static bool bInit = false;
	if(!bInit)
	{
		CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(NotificationDirector::step), this, 0, false);
		bInit = true;
	}

	std::multimap<std::string, INotificationDelegate*>::iterator pos;
	for (pos = _delegateMap.lower_bound(noteKey);
		pos != _delegateMap.upper_bound(noteKey); ++pos)
	{
		if((*pos).second == delegate )
		{
			//_delegateMap.erase(pos);
			return;
		}
	}
	_delegateMap.insert( std::map<std::string, INotificationDelegate*>::value_type(noteKey, delegate) );
}

void NotificationDirector::removeNotification(const char* noteKey)
{
	std::string itemKey = noteKey;

	std::multimap<std::string, INotificationDelegate*>::iterator pos = _delegateMap.find(noteKey);

	while (pos != _delegateMap.end())
	{
		_delegateMap.erase(pos);
		pos = _delegateMap.find(noteKey);
	}

// 	std::multimap<std::string, INotificationDelegate*>::iterator pos = _delegateMap.begin();
// 
// 	for (; pos != _delegateMap.end();) 
// 	{
// 		if (pos->first == itemKey) 
// 		{
// 			_delegateMap.erase(pos);
// 		}
// 		else 
// 		{
// 			++pos;
// 		}
// 	}
}

void NotificationDirector::removeNotification(const char* noteKey, INotificationDelegate* delegate)
{
	std::string itemKey = noteKey;

	std::multimap<std::string, INotificationDelegate*>::iterator pos;
	for (pos = _delegateMap.lower_bound(itemKey);
		pos != _delegateMap.upper_bound(itemKey); ++pos)
	{
		if((*pos).second == delegate )
		{
			_delegateMap.erase(pos);
			break;
		}
	}
}

void NotificationDirector::post(const char* noteKey, void* object, bool immediate)
{
	NotificationItem item;
	item.noteKey = noteKey;
	item.object = object;

	if(immediate)
	{
		doPost(item);
	}	
	else
	{
        if( _itemPostLock )
        {
            _itemListBackupOnLock.push_back(item);
        }
        else 
        {
            _itemList.push_back(item);
        }
	}
	
}

void NotificationDirector::doPost(NotificationItem& item)
{
    _itemPostLock = true;
    
	std::vector<INotificationDelegate*>tempDelegateArray;
	std::multimap<std::string, INotificationDelegate*>::iterator pos;

	for (pos = _delegateMap.lower_bound(item.noteKey);
		pos != _delegateMap.upper_bound(item.noteKey); ++pos)
	{
		tempDelegateArray.push_back((*pos).second);
	}

	for(std::vector<INotificationDelegate*>::iterator it = tempDelegateArray.begin(); it != tempDelegateArray.end(); ++it)
	{
		(*it)->onHandleNotification(item.noteKey, item.object);
	}
    _itemPostLock = false;
}
