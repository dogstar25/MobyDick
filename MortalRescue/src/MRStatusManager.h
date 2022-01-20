#pragma once

#include "StatusManager.h"

class MRStatusManager : public StatusManager
{

public:
	//static MRStatusManager& instance();
	MRStatusManager();
	~MRStatusManager() = default;

	void initMappings() override;

};

																																																																																																																	