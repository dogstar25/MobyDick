#ifndef ENUM_MAPS_H
#define ENUM_MAPS_H

#include "Util.h"
#include "BaseConstants.h"
#include "particleEffects/BaseParticleEffects.h"

class EnumMap {

public:
	EnumMap();
	~EnumMap();

	static EnumMap& instance();
	const int toEnum(std::string name);
	void addEnumItem(std::string id, int enumValue) {

		m_enumMap[id] = enumValue;
	}

private:
	std::map<std::string, int> m_enumMap;

};

#endif

