#include "HudItemFactory.h"

#include "HudStatusText.h"
#include "HudStatusSeries.h"

HudItemFactory& HudItemFactory::instance()
{
    static HudItemFactory singletonInstance;
    return singletonInstance;

}

std::shared_ptr<HudItem> HudItemFactory::create(HudItemTypes type, std::string labelId, std::string statusId, std::string statusValueId, 
    float labelPadding, Scene* parentScene)
{

    std::shared_ptr<HudItem> hudItem{};

    if (type == HudItemTypes::STATUS_SINGLE) {

        hudItem = std::make_shared<HudStatusText>(labelId, statusId, statusValueId, labelPadding, parentScene);
        
    }
    else if (type == HudItemTypes::STATUS_SERIES) {

        hudItem = std::make_shared<HudStatusSeries>(labelId, statusId, statusValueId, labelPadding, parentScene);
    }


    return hudItem;
}
