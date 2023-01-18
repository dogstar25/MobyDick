#include "BBContextManager.h"
#include "SoundManager.h"
#include "Util.h"
#include "game.h"
#include "GameConstants.h"
#include <filesystem>

extern std::unique_ptr<Game> game;

BBContextManager::BBContextManager()
	: ContextManager()
{
	auto saveFileData = std::make_shared<GameSaveFileData>();

	//Get the saved values from gameData file or create the gameData file for the first time
	if (std::filesystem::exists(m_saveGamePath + GAME_FILENAME)) {
	//if (util::fileExists(m_saveGamePath + GAME_FILENAME)) {
 		loadGame(saveFileData.get());
	}
	else {
		saveFileData->level = 0;
		saveFileData->mouseSensitivity = 50;
		saveFileData->soundLevel = 50;
		saveGame(saveFileData.get());
	}

}

int BBContextManager::getCurrentLevel()
{
	GameSaveFileData saveFileData{};
	std::ifstream m_gameFile(m_saveGamePath + GAME_FILENAME, std::ofstream::in);

	m_gameFile.read((char*)&saveFileData, sizeof(GameSaveFileData));

	m_gameFile.close();
	assert(m_gameFile && "Error reading game file");

	return saveFileData.level;

}

bool BBContextManager::loadGame(BaseSaveFileData* saveFileData)
{
	std::ifstream m_gameFile(m_saveGamePath + GAME_FILENAME, std::ofstream::in);

	m_gameFile.read( (char*)saveFileData, sizeof(GameSaveFileData));

	m_gameFile.close();
	assert(m_gameFile && "Error reading game file");

	//set values for the settings
	m_userSettings.mouseSensitivity = saveFileData->mouseSensitivity;
	m_userSettings.soundLevel = saveFileData->soundLevel;

	return true;

}

bool BBContextManager::saveGame(BaseSaveFileData* saveFileData)
{

	std::ofstream m_gameFile(m_saveGamePath + GAME_FILENAME, std::ofstream::out);

	m_gameFile.write(reinterpret_cast<char*>(saveFileData), sizeof(GameSaveFileData));

	m_gameFile.close();
	assert(m_gameFile && "Error reading game file");

	setMouseSensitivity(saveFileData->mouseSensitivity);
	setSoundVolume(saveFileData->soundLevel);

	return true;
}


