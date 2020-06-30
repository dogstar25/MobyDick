#ifndef ATTACHMENTS_COMPONENT_H
#define ATTACHMENTS_COMPONENT_H

#include <vector>
#include "Component.h"

class AttachmentsComponent :   public Component
{
public:
	AttachmentsComponent();
	~AttachmentsComponent();

	void update() override;


private:
	//std::vector<GameObjectAttachment> m_attachments;






};

#endif

