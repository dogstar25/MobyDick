#pragma once

#include "StatusManager.h"

class MRStatusManager : public StatusManager
{

public:
	//static MRStatusManager& instance();
	MRStatusManager() = default;
	~MRStatusManager() = default;

	float lives() { return m_lives; }
	void setLives(float lives) { m_lives = lives; }
	float scrapCount() { return m_scrapCount; }
	void setScrapCount(float scrapCount) { m_scrapCount = scrapCount; }

	void initHudMappings() override;

private:

	float m_lives;
	float m_weaponLevel;
	float m_scrapCount;

};

																																																																																																																	