#pragma once

#include "ContextManager.h"

class GameSaveFileData : public BaseSaveFileData {
	public:
	short level;
};

class BBContextManager : public ContextManager
{

public:
	//static MRContextManager& instance();
	BBContextManager();

	bool saveGame(BaseSaveFileData* saveFileData) override;
	bool loadGame(BaseSaveFileData* saveFileData) override;
	int getCurrentLevel();



};

																																																																																																																	