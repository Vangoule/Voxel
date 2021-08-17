#pragma once
#include "BlockTypes.h"

class Block
{
public:
    Block();
    ~Block();

    bool IsActive();
    void SetActive(bool active);
	
private:
    bool activeState;

    BlockType blockType;
};