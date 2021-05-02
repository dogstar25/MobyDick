#pragma once
class Trigger
{

public:
	Trigger() {};
	virtual bool hasMetCriteria() { return false; };
	virtual void execute() {};

protected:

};

