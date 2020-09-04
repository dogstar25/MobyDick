#ifndef CONTACT_EVENTS_MAP_H
#define CONTACT_EVENTS_MAP_H

#include <map>
#include <memory>
#include <string>
#include <assert.h>

class ContactEventMap
{
public:
	static ContactEventMap& instance();
	ContactEventMap();
	~ContactEventMap();

	std::shared_ptr<Action> getAction(std::string actionKey) {

		assert(m_actionMap.find(actionKey) != m_actionMap.end() && "ActionKey was not found in ActionMaps collection");

		return m_actionMap[actionKey];
	}

private:

	std::map<std::string, std::shared_ptr<Action>>m_actionMap;

};

#endif

