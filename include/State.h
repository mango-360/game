#pragma once

class State
{
public:
	State();
	~State();

	virtual void init();
	virtual void run();
	virtual void destroy();
};