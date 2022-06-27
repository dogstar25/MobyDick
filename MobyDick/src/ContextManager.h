#pragma once
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class BaseSaveFileData {
public:
	int16_t soundLevel;
	int16_t mouseSensitivity;
};


class StatusItem
{

public:
	StatusItem(int id, float value=0, float minValue=0, float maxValue=0) :
		m_id(id),
		m_value(value),
		m_initialValue(value), 
		m_minValue(minValue),
		m_maxValue(maxValue) {}
	StatusItem() = default;
	void adjust(float adjustValue);
	void reset() { m_value = m_initialValue; }

	int id() { return m_id; }
	float value() { return m_value; }
	float initialvalue() { return m_initialValue; }
	float minValue() { return m_minValue; }
	float maxValue() { return m_maxValue; }

	void setValue(float newValue) { m_value = newValue; }
	void setInitialValue(float initialValue) { m_initialValue = initialValue; }
	void setMinValue(float minValue) { m_minValue = minValue; }
	void setMaxValue(float maxValue) { m_maxValue = maxValue; }

	bool isAtMax() { return (m_value >= m_maxValue); }

private:
	int m_id{};
	bool m_resetOnLevelLoad{};
	float m_value{};
	float m_initialValue{};
	float m_minValue{};
	float m_maxValue{};


};

const std::string GAME_FILENAME = "\\game.dat";

struct UserSettings {

	int soundLevel{};
	int mouseSensitivity{};

};

class ContextManager
{
public:
	ContextManager();
	~ContextManager() {};

	std::map<int, StatusItem> statusValueMap() { return m_statusValueMap; }
	float getStatusItemValue(int id);
	void adjustStatusItemValue(int id, float adjustmentValue);
	void setStatusItemValue(int id, float newValue);
	StatusItem& getStatusItem(int id);
	void clearStatusItems();

	void setMouseSensitivity(int mouseSensitivity);
	void setSoundVolume(int volume);

	//These are the 1 - 100 GUI slider config values
	int getMouseSensitivityForGui();
	int getSoundVolumeForGui();

	//These are the values to be used for the actual configuration
	float getMouseSensitivity();
	int getSoundVolume();
	void addStatusItem(int id, StatusItem& statusItem);

protected:
	virtual bool saveGame(BaseSaveFileData& saveFileData) = 0;
	virtual bool loadGame(BaseSaveFileData& saveFileData) = 0;

	std::map<int, StatusItem> m_statusValueMap;
	UserSettings m_userSettings{};

	std::string m_saveGamePath{};


};


