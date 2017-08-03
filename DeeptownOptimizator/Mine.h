#pragma once
class Mine
{
	const char speed;
	const char level;
	short height;
public:
	Mine(char pSpeed, char pLevel);
	void setHeight(short h);
	short getHeight();
	~Mine();
};

