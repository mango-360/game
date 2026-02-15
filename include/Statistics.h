#pragma once 

#include "defines.h"
#include "TextField.h"

class Statistics
{
public:
	Statistics();
	~Statistics();
	void init();
	void update();
	void draw();
	void destroy();
private:
	TextField m_position;
};