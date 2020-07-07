#include "TextComponent.h"

#include "json/json.h"

#include "TransformComponent.h"
#include "RenderComponent.h"
#include "../GameObjectManager.h"
#include "../Renderer.h"
#include "../DynamicTextManager.h"
#include "../GameConfig.h"
#include "../texture.h"

TextComponent::TextComponent()
{

}



TextComponent::TextComponent(std::string gameObjectId)
{

	m_parentGameObjectId = gameObjectId;
	m_textureId = "TX_" + gameObjectId;

	if (gameObjectId.rfind("DEBUG_", 0) == 0)
	{
		m_isDebugText = true;
		m_debugId = gameObjectId;
		m_isDynamic = true;
		m_textValue = "default";
		m_fontId = "FONT_ARIAL_REG";
		m_fontSize = 12;
		
	}
	else
	{
		Json::Value definitionJSON = GameObjectManager::instance().getDefinition(gameObjectId)->definitionJSON();
		Json::Value textComponentJSON = definitionJSON["textComponent"];

		m_isDebugText = false;
		m_isDynamic = textComponentJSON["dynamic"].asBool();
		m_fontId = textComponentJSON["font"].asString();
		m_textValue = textComponentJSON["value"].asString();
		m_fontSize = textComponentJSON["fontSize"].asInt();

	}

}




void TextComponent::construct()
{

	

}


TextComponent::~TextComponent()
{

}

void TextComponent::update()
{

	std::string textureId = "TX_" + m_parentGameObjectId;

	if (TextureManager::instance().hasTexture(textureId))
	{
		//If we have already set the texture, dont have to do it again
		if (!m_refRenderComponent->texture())
		{
			m_refRenderComponent->setTexture(TextureManager::instance().getTexture(textureId));
			m_refTransformComponent->setSize(m_refRenderComponent->texture()->surface->w, m_refRenderComponent->texture()->surface->h);
		}

	}
	else
	{
		m_refRenderComponent->setTexture(generateTextTexture());
	}

	if (m_isDynamic == true)
	{
		m_refRenderComponent->setTexture(updateDynamicTextTexture());
	}


}

std::shared_ptr<Texture> TextComponent::generateTextTexture()
{

	std::shared_ptr<Texture> texture = std::make_shared<Texture>();;

	/*
	TextObjects have their textures generated here, not in the TextureManager.init().
	Generate the texture and add it to the TextureManager so that it can be shared with
	other objects if needed.
	*/
	SDL_Surface* tempSurface;

	std::string fontFile = TextureManager::instance().getFont(m_fontId);

	//FIXME:Add a vector of TTF_Font* fontObject's with maybe 6 different font sizes. OPen them in the contructor
	//and leave them open for regenerating text textures. Then close them in the deconstructor.
	//This should save loads of time
	TTF_Font* fontObject = TTF_OpenFont(fontFile.c_str(), m_fontSize);
	tempSurface = TTF_RenderText_Blended(fontObject, m_textValue.c_str(), m_refRenderComponent->color());
	TTF_CloseFont(fontObject);

	//Set the size of the textObject now that its texture has been generated
	m_refTransformComponent->setSize(tempSurface->w, tempSurface->h);

	texture->sdlTexture = Renderer::instance().createTextureFromSurface(tempSurface);
	texture->surface = tempSurface;

	//Add it to the textureManager but first free any texture that may already be there in case this is
	//being generated for a dynamic text object
	TextureManager::instance().addOrReplaceTexture(m_textureId, texture);

	return 	texture;

}

std::shared_ptr<Texture> TextComponent::updateDynamicTextTexture()
{

	TextItem* newText;
	std::shared_ptr<Texture> texture;
	SDL_Surface* surface;

	newText = DynamicTextManager::instance().getTextItem(m_parentGameObjectId);

	//check the clock and see if enough time as gone by
	std::chrono::steady_clock::time_point now_time = std::chrono::steady_clock::now();
	std::chrono::duration<double> time_diff = now_time - newText->time_snapshot;

	//use a timer to slow down the re-generating of dynamic text because its time consuming
	if (newText->hasChanged == true && time_diff.count() > GameConfig::instance().dynamicTextRefreshDelay())
	{
		//update the timestamp
		newText->time_snapshot = now_time;

		//Build new texture
		m_textValue = newText->textValue;
		texture = generateTextTexture();
		newText->hasChanged = false;

	}
	else
	{

		texture = TextureManager::instance().getTexture(m_textureId);

		//TODO:set a flag at game object level so that the TextureManager::render(TextObject* gameObject) doesnt have to 
		// do SDL_QueryTexture
	}

	return texture;

}

void TextComponent::setDependencyReferences(
	std::shared_ptr<TransformComponent> transformComponent,
	std::shared_ptr<RenderComponent> renderComponent)
{

	m_refTransformComponent = transformComponent;
	m_refRenderComponent = renderComponent;

}




