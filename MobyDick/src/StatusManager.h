#pragma once
#include <map>
#include <string>

//typedef  int (StatusManager::* AccessorFunc)();

class StatusItem
{

public:
	void adjust(float adjustValue) { *m_value += adjustValue; }
	float* get() { return m_value; }
	void set(float* value) { m_value = value; }

private:
	float* m_value;

};

class StatusManager
{
	friend class HudManager;

public:
	StatusManager() = default;
	~StatusManager() = default;

	float level() { return m_level; }
	void setLevel(float level) { m_level = level; }

	std::map<std::string, StatusItem> hudValueMap() { return m_hudValueMap; }
	//void setHudMapping(std::string hudItemId, float* statusValuePtr);


	virtual void initHudMappings();

protected:
	//std::map<std::string, float*> m_hudValueMap;
	std::map<std::string, StatusItem> m_hudValueMap;

private:
	float m_level;
};


