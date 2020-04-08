#pragma once
#include <glad/glad.h>

class TimeQuery
{
public:
	TimeQuery();
	~TimeQuery();

	void start();
	void stop();

	//milliseconds
	double getDeltaTime();
	double getFPS();
private:
	double _deltaTime;
	unsigned int _queryID[2];
	GLuint64 _startTime;
	GLuint64 _stopTime;
};