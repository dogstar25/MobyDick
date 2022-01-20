#pragma once
#include <map>
#include <string>

//typedef  int (StatusManager::* AccessorFunc)();

class StatusItem
{

public:
	StatusItem(float value, float maxValue) :
		m_value(value),
		m_originalValue(value),
		m_maximumValue(maxValue) {}
	StatusItem() = default;
	void adjust(float adjustValue);
	float value() { return m_value; }
	void setValue(float newValue) { m_value = newValue; }

	void reset();

private:
	float m_value;
	float m_originalValue;
	float m_maximumValue;

};

class StatusManager
{
public:
	StatusManager();
	~StatusManager() = default;

	std::map<std::string, StatusItem> valueMap() { return m_valueMap; }
	float getValue(std::string valudId);
	void adjustValue(std::string valudId, float adjustmentValue);
	void setValue(std::string valudId, float newValue);

	virtual void initMappings();

protected:
	std::map<std::string, StatusItem> m_valueMap;

};


