#include "TextComponent.h"


#include "../game.h"

extern std::unique_ptr<Game> game;

TextComponent::TextComponent(std::string gameObjectId, Json::Value componentJSON)
{

	m_isDebugText = false;
	m_isDynamic = componentJSON["dynamic"].asBool();
	m_fontId = componentJSON["font"].asString();
	m_textValue = componentJSON["value"].asString();
	m_fontSize = componentJSON["fontSize"].asInt();
	m_dynamicValueId = componentJSON["statusValueId"].asString();
	m_gameObjectId = gameObjectId;


	if (m_textValue.empty())
	{
		m_textValue = "Default Text";
	}

	std::string fontFile = TextureManager::instance().getFont(m_fontId);
	m_fontObject = TTF_OpenFont(fontFile.c_str(), m_fontSize);

	m_refreshTimer = Timer(GameConfig::instance().dynamicTextRefreshDelay());


	//
	//Problem here
	//
	if (m_isDynamic == true) {
		std::stringstream uniqueTextureId;
		uniqueTextureId << "TX_" << m_dynamicValueId;
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

	if (TextureManager::instance().hasTexture(m_textureId))
	{
		renderComponent->setTexture(TextureManager::instance().getTexture(m_textureId));
		transformComponent->setSize((float)renderComponent->texture()->surface->w, (float)renderComponent->texture()->surface->h);
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

	std::shared_ptr<Texture> texture = std::make_shared<Texture>();
	//If this is openGL, then set this textureIndex to the dynammically loaded type
	if (GameConfig::instance().rendererType() == RendererType::OPENGL) {

		texture->openglTextureIndex = GL_TextureIndexType::DYNAMICALLY_LOADED;
	}


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
	//TTF_RenderText_Blended_Wrapped() - for wrapping text - swesome

	//Set the size of the textObject now that its texture has been generated
	transformComponent->setSize((float)tempSurface->w, (float)tempSurface->h);
	transformComponent->setPosition(
		transformComponent->originalPosition().x + tempSurface->w/2,
		transformComponent->originalPosition().y + tempSurface->h/2);

	//Also set the size of the texture itself
	SDL_Rect quad = { 0 , 0, tempSurface->w, tempSurface->h };
	texture->textureAtlasQuad = std::move(quad);

	texture->sdlTexture = game->renderer()->createTextureFromSurface(tempSurface);
	//texture->surface = SDL_ConvertSurface(tempSurface, SDL_PixelFormat, 0);
	texture->surface = tempSurface;

	//Add it to the textureManager but first free any texture that may already be there in case this is
	//being generated for a dynamic text object
	TextureManager::instance().addOrReplaceTexture(m_textureId, texture);

	return 	texture;

}

std::shared_ptr<Texture> TextComponent::updateDynamicTextTexture()
{

	std::shared_ptr<Texture> texture;

	if ( m_refreshTimer.hasMetTargetDuration())
	{

		//Build new texture
		auto newTextValue = game->contextMananger()->getValue(m_dynamicValueId);
		std::stringstream ss;
		ss << static_cast<float>(newTextValue);
		m_textValue = ss.str();
		texture = generateTextTexture();
	}
	else
	{

		texture = TextureManager::instance().getTexture(m_textureId);
	}

	return texture;

}






