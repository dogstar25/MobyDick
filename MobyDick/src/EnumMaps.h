#ifndef ENUM_MAPS_H
#define ENUM_MAPS_H

#include "Globals.h"

class EnumMap {

public:
	EnumMap();
	~EnumMap();

	static EnumMap& instance();
	size_t toEnum(std::string name);

private:
	std::map<std::string, size_t> m_enumMap;

};

#endif

