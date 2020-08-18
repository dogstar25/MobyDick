#include "ContactEventMap.h"

ContactEventMap& ContactEventMap::instance()
{
	static ContactEventMap singletonInstance;
	return singletonInstance;
}


ContactEventMap::ContactEventMap()
{
	

}


ContactEventMap::~ContactEventMap()
{

}