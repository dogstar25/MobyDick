#pragma once
#include "Component.h"
#include "../LevelManager.h"
#include "../BrainAABBCallback.h"

#include <json/json.h>
#include "glm/glm.hpp"
//#include "../GameObject.h"

#include <optional>

inline constexpr int DESTINATION_DISTANCE_TOLERANCE = 25;
inline constexpr int HASMOVED_DISTANCE_TOLERANCE = 2;
inline constexpr int NAVIGATION_STUCK_TIMER_DURATION = 2;

struct SeenObjectDetails {

    std::weak_ptr<GameObject> gameObject{};
    float distance{};
    b2Vec2 normal{};
    b2Fixture* fixture{};
};

class GameObject;

class BrainComponent : public Component
{

public:
    BrainComponent();
    BrainComponent(Json::Value componentJSON);
    ~BrainComponent();

    void update() override;
    void postInit() override;

    int state() { return m_currentState; }

    int sightSensorSize() { return m_sightSensorSize; }

    std::vector<SeenObjectDetails> seenObjects() { return m_seenObjects; }
    std::vector<std::weak_ptr<GameObject>> detectedObjects() { return m_detectedObjects; }

protected:

    int m_currentState{ BrainState::PATROL };
    std::bitset<32> m_detectObjectTraits{};
    int m_sightSensorSize{};
    std::vector<SeenObjectDetails> m_seenObjects;
    std::vector<std::weak_ptr<GameObject>> m_detectedObjects;

private:
    void _updateSensorInput();
    std::optional<SeenObjectDetails> _hasLineOfSight(BrainAABBFoundObject& detectedObject);
    Timer m_updateSensorInputTimer{0.25, true};

};

