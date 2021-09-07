#include "DrawBatch.h"

#include <memory>

#include "GLRenderer.h"
#include "../game.h"

extern std::unique_ptr<Game> game;

DrawBatch::DrawBatch(int ObjectType)
{

	
	m_glDrawer = static_cast<GLRenderer*>(game->renderer())->spriteDrawer();



}
