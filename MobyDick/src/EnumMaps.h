#ifndef ENUM_MAPS_H
#define ENUM_MAPS_H

#include "Globals.h"

class EnumMap {

public:
	EnumMap();
	~EnumMap();

	static EnumMap& instance();
	const int toEnum(std::string name);

private:
	std::map<std::string, int> m_enumMap;

};

#endif

