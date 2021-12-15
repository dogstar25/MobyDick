#include "TextComponent.h"

#include <memory>

#include "../game.h"
#include "../DynamicTextManager.h"
#include "../GameConfig.h"
#include "../GameObject.h"

extern std::unique_ptr<Game> game;

TextComponent::TextComponent(std::string gameObjectId, Json::Value definitionJSON)
{

	//gameObjectId can be dynamic because of the debug text so it must be passed in
	m_gameObjectId = gameObjectId;

	Json::Value textComponentJSON = definitionJSON["textComponent"];
	m_isDebugText = false;
	m_isDynamic = textComponentJSON["dynamic"].asBool();
	m_fontId = textComponentJSON["font"].asString();
	m_textValue = textComponentJSON["value"].asString();
	m_fontSize = textComponentJSON["fontSize"].asInt();

	if (m_textValue.empty())
	{
		m_textValue = "Default Text";
	}

	std::string fontFile = TextureManager::instance().getFont(m_fontId);
	m_fontObject = TTF_OpenFont(fontFile.c_str(), m_fontSize);

	m_refreshTimer = Timer(GameConfig::instance().dynamicTextRefreshDelay());


	if (m_isDynamic == true) {
		m_dynamicCount++;
		std::stringstream uniqueTextureId;
		uniqueTextureId << "TX_" << gameObjectId << m_dynamicCount;

		m_textureId = uniqueTextureId.str();

	}
	else {
		m_textureId = "TX_" + gameObjectId;

	}

}

void TextComponent::construct()
{
	

}

TextComponent::~TextComponent()
{
	TTF_CloseFont(m_fontObject);
}

void TextComponent::update()
{
	//convenience reference to outside component(s)
	const auto& renderComponent = parent()->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	const auto& transformComponent = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

	//std::string textureId = "TX_" + m_gameObjectId;
	//std::string textureId = "TX_" + parent()->name();

	if (TextureManager::instance().hasTexture(m_textureId))
	{
		//If we have already set the texture, dont have to do it again
		//if (renderComponent->texture())
		//{
			renderComponent->setTexture(TextureManager::instance().getTexture(m_textureId));
			transformComponent->setSize((float)renderComponent->texture()->surface->w, (float)renderComponent->texture()->surface->h);

		//}

	}
	else
	{
		renderComponent->setTexture(generateTextTexture());
	}

	if (m_isDynamic == true)
	{
		renderComponent->setTexture(updateDynamicTextTexture());

	}

}

std::shared_ptr<Texture> TextComponent::generateTextTexture()
{
	//convenience reference to outside component(s)
	const auto& renderComponent = parent()->getComponent<RenderComponent>(ComponentTypes::RENDER_COMPONENT);
	const auto& transformComponent = parent()->getComponent<TransformComponent>(ComponentTypes::TRANSFORM_COMPONENT);

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
	//tempSurface = TTF_RenderText_Blended(m_fontObject, m_textValue.c_str(), renderComponent->color());
	tempSurface = TTF_RenderText_Blended(m_fontObject, m_textValue.c_str(), renderComponent->color());

	//Set the size of the textObject now that its texture has been generated
	transformComponent->setSize((float)tempSurface->w, (float)tempSurface->h);
	transformComponent->setPosition(
		transformComponent->originalPosition().x + tempSurface->w/2,
		transformComponent->originalPosition().y + tempSurface->h/2);

	//Also set the size of the texture itself
	SDL_Rect quad = { 0 , 0, (float)tempSurface->w, (float)tempSurface->h };
	texture->textureAtlasQuad = std::move(quad);

	texture->sdlTexture = game->renderer()->createTextureFromSurface(tempSurface);
	//texture->surface = SDL_ConvertSurface(tempSurface, SDL_PixelFormat, 0);
	texture->surface = tempSurface;

	//For openGL, text items are always at texture index 1
	if (GameConfig::instance().rendererType() == RendererType::OPENGL) {
		texture->gLTextureId = 2;
	}

	//Add it to the textureManager but first free any texture that may already be there in case this is
	//being generated for a dynamic text object
	TextureManager::instance().addOrReplaceTexture(m_textureId, texture);

	return 	texture;

}

std::shared_ptr<Texture> TextComponent::updateDynamicTextTexture()
{

	TextItem* newText;
	std::shared_ptr<Texture> texture;

	newText = DynamicTextManager::instance().getTextItem(m_gameObjectId);

	//use a timer to slow down the re-generating of dynamic text because its time consuming
	//if (newText->hasChanged == true && m_refreshTimer.hasMetTargetDuration())
	if ( m_refreshTimer.hasMetTargetDuration())
	{

		//Build new texture
		m_textValue = newText->textValue;
		texture = generateTextTexture();
		newText->hasChanged = false;
	}
	else
	{

		texture = TextureManager::instance().getTexture(m_textureId);
	}

	return texture;

}






