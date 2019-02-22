#ifndef __NOTIFICATION_H__
#define __NOTIFICATION_H__
#include <string>

class INotificationDelegate
{
public:
	virtual void onHandleNotification(std::string noteKey, void* object) = 0;
};

// Login notification
#define NOTE_GAME_LOGIN_SUCCEED			"game login succeed"
#define NOTE_GAME_LOGIN_TIMEOUT			"game login timeout"
#define NOTE_GAME_LOGIN_ULOCKED			"game login user locked"

// Network notification
#define NOTE_GAME_NETWORK_ERROR			"game network error"



#define NOTE_GAME_CHANGE_LAYER			"game scene change layer"

#define NOTE_GET_META_DATA_ERROR		"get meta data error"
#define NOTE_GET_UPDATE_DATA_ERROR		"get update data error"
#define NOTE_DOWNLOAD_FILE_ERROR		"download file error"
#define NOTE_CLIENT_UPDATE				"client update"










#endif // __HELLOWORLD_SCENE_H__
