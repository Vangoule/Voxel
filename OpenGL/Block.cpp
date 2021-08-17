#include "Block.h"


Block::Block(void)
{
	activeState = true;
}


Block::~Block(void)
{
}


bool Block::IsActive()
{
	return activeState;
}

void Block::SetActive(bool active)
{
	activeState = active;
}