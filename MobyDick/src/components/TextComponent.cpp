#include "TextComponent.h"

#include "json/json.h"

#include "TransformComponent.h"
#include "RenderComponent.h"
#include "../GameObjectManager.h"
#include "../Renderer.h"
#include "../DynamicTextManager.h"
#include "../GameConfig.h"
#include "../texture.h"
#include "../GameObject.h"

TextComponent::TextComponent()
{

}



TextComponent::TextComponent(std::string gameObjectId, Json::Value definitionJSON)
{

	//gameObjectId can be dynamic because of the debug text so it must be passed in
	m_textureId = "TX_" + gameObjectId;
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

	//test
	std::string fontFile = TextureManager::instance().getFont(m_fontId);
	m_fontObject = TTF_OpenFont(fontFile.c_str(), m_fontSize);


}




void TextComponent::construct()
{

	

}


TextComponent::~TextComponent()
{
	TTF_CloseFont(m_fontObject);
}

void TextComponent::update(std::shared_ptr<GameObject>gameObject)
{
	//convenience reference to outside component(s)
	auto& renderComponent =
		std::static_pointer_cast<RenderComponent>(gameObject->components()[RENDER_COMPONENT]);
	auto& transformComponent =
		std::static_pointer_cast<TransformComponent>(gameObject->components()[TRANSFORM_COMPONENT]);

	std::string textureId = "TX_" + m_gameObjectId;

	if (TextureManager::instance().hasTexture(textureId))
	{
		//If we have already set the texture, dont have to do it again
		if (renderComponent->texture())
		{
			renderComponent->setTexture(TextureManager::instance().getTexture(textureId));
			transformComponent->setSize(renderComponent->texture()->surface->w, renderComponent->texture()->surface->h);
		}

	}
	else
	{
		renderComponent->setTexture(generateTextTexture(gameObject));
	}

	if (m_isDynamic == true)
	{
		renderComponent->setTexture(updateDynamicTextTexture(gameObject));
	}


}

std::shared_ptr<Texture> TextComponent::generateTextTexture(std::shared_ptr<GameObject>gameObject)
{
	//convenience reference to outside component(s)
	auto& renderComponent =
		std::static_pointer_cast<RenderComponent>(gameObject->components()[RENDER_COMPONENT]);
	auto& transformComponent =
		std::static_pointer_cast<TransformComponent>(gameObject->components()[TRANSFORM_COMPONENT]);

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
	//m_fontObject = TTF_OpenFont(fontFile.c_str(), m_fontSize);
	tempSurface = TTF_RenderText_Blended(m_fontObject, m_textValue.c_str(), renderComponent->color());
	//TTF_CloseFont(m_fontObject);

	//Set the size of the textObject now that its texture has been generated
	transformComponent->setSize(tempSurface->w, tempSurface->h);
	transformComponent->setPosition(
		transformComponent->originalPosition().x + tempSurface->w/2,
		transformComponent->originalPosition().y + tempSurface->h/2);

	texture->sdlTexture = Renderer::instance().createTextureFromSurface(tempSurface);
	texture->surface = tempSurface;

	//Add it to the textureManager but first free any texture that may already be there in case this is
	//being generated for a dynamic text object
	TextureManager::instance().addOrReplaceTexture(m_textureId, texture);

	return 	texture;

}

std::shared_ptr<Texture> TextComponent::updateDynamicTextTexture(std::shared_ptr<GameObject>gameObject)
{

	TextItem* newText;
	std::shared_ptr<Texture> texture;
	SDL_Surface* surface;

	newText = DynamicTextManager::instance().getTextItem(m_gameObjectId);

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
		texture = generateTextTexture(gameObject);
		newText->hasChanged = false;

	/*	m_refTransformComponent->setPosition(
			(m_refTransformComponent->size().x / 2),
			(m_refTransformComponent->size().y / 2)
		);*/


	}
	else
	{

		texture = TextureManager::instance().getTexture(m_textureId);

		//TODO:set a flag at game object level so that the TextureManager::render(TextObject* gameObject) doesnt have to 
		// do SDL_QueryTexture
	}

	return texture;

}






