#pragma once

#include "ContextManager.h"


struct SaveFileData {
	int16_t level;
	int16_t soundLevel;
	int16_t mouseSensitivity;
};

class MRContextManager : public ContextManager
{

public:
	//static MRContextManager& instance();
	MRContextManager();
	~MRContextManager() = default;

	void initMappings() override;
	bool saveGame(SaveFileData& saveFileData);
	bool loadGame(SaveFileData& saveFileData);



};

																																																																																																																	