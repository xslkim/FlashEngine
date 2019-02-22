#ifndef  __DATA_LOGIC_MGR__
#define  __DATA_LOGIC_MGR__

#include <string>

class DOMDocument;
class DataLogicMgr
{
public:
	DataLogicMgr();
	~DataLogicMgr();
	static DataLogicMgr* getInstance();
	void initCmdLine(const char* comLine);
	bool isPlay();
	bool isLoadXml(){ return _isLoadXml; }

	void loadScreen(std::string path);
	bool loadXml(std::string path);
	bool loadFpk(std::string pathAndName);

	std::string getPath(){ return _path; }
	std::string getFpkPath();
	std::string getPlistName(){ return _plistName; }
	//const FlashCCSprite* getSprite( std::string name ){ return _document->getSprite(name); }
	//const DOMSymbolItem* getSymbolItem( const char* name){ return _document->getSymbolItem(name); }
	DOMDocument* getDocument(){ return _document; }
	int getMaxAnimination();
	int getScreenHeight();
	int getScreenWidth();

private:
	static DataLogicMgr* _instance;
	bool _isPlay;
	bool _isLoadXml;
	DOMDocument* _document;
	std::string _path;
	std::string _outFolder;
	std::string _plistName;
};

DataLogicMgr* getDataLogicMgr();

#endif