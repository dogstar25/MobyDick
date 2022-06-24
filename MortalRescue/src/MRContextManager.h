#pragma once

#include "ContextManager.h"

class GameSaveFileData : public BaseSaveFileData {
	public:
	int16_t level;
};

class MRContextManager : public ContextManager
{

public:
	//static MRContextManager& instance();
	MRContextManager();

	bool saveGame(BaseSaveFileData& saveFileData) override;
	bool loadGame(BaseSaveFileData& saveFileData) override;



};

																																																																																																																	