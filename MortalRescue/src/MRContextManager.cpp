#include "MRContextManager.h"
#include "SoundManager.h"
#include "Util.h"
#include "game.h"
#include "GameConstants.h"
#include <filesystem>

extern std::unique_ptr<Game> game;

MRContextManager::MRContextManager()
	: ContextManager()
{
	auto saveFileData = std::make_shared<GameSaveFileData>();

	//Get the saved values from gameData file or create the gameData file for the first time
	if (std::filesystem::exists(m_saveGamePath + GAME_FILENAME)) {
	//if (util::fileExists(m_saveGamePath + GAME_FILENAME)) {
		loadGame(saveFileData.get());
	}
	else {
		saveFileData->level = "1";
		saveFileData->mouseSensitivity = 50;
		saveFileData->soundLevel = 50;
		saveGame(saveFileData.get());
	}

}

bool MRContextManager::loadGame(BaseSaveFileData* saveFileData)
{
	std::ifstream m_gameFile(m_saveGamePath + GAME_FILENAME, std::ofstream::in);

	m_gameFile.read(reinterpret_cast<char*>(saveFileData), sizeof(&saveFileData));

	m_gameFile.close();
	if (!m_gameFile) {
		std::cout << "Error reading game file" << std::endl;
		return false;
	}

	//set values for the settings
	m_userSettings.mouseSensitivity = saveFileData->mouseSensitivity;
	m_userSettings.soundLevel = saveFileData->soundLevel;

	return true;

}

bool MRContextManager::saveGame(BaseSaveFileData* saveFileData)
{

	std::ofstream m_gameFile(m_saveGamePath + GAME_FILENAME, std::ofstream::out);

	m_gameFile.write(reinterpret_cast<char*>(saveFileData), sizeof(&saveFileData));

	m_gameFile.close();
	if (!m_gameFile) {
		std::cout << "Error writing game file" << std::endl;
		return false;
	}

	setMouseSensitivity(saveFileData->mouseSensitivity);
	setSoundVolume(saveFileData->soundLevel);

	return true;
}


