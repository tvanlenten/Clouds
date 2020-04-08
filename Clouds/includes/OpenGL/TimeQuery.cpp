#include "TimeQuery.h"
#include "OpenGLDebug.h"

TimeQuery::TimeQuery()
{
	_queryID[0] = 0;
	_deltaTime = 0.0f;
}

TimeQuery::~TimeQuery()
{
	if (_queryID[0]) glDeleteQueries(2, _queryID);
}

void TimeQuery::start()
{
	if (_queryID[0]) glDeleteQueries(2, _queryID);
	glGenQueries(2, _queryID);
	glQueryCounter(_queryID[0], GL_TIMESTAMP);
}

void TimeQuery::stop()
{
	glQueryCounter(_queryID[1], GL_TIMESTAMP);
	GLint stopTimerAvailable = 0;
	while (!stopTimerAvailable) {
		glGetQueryObjectiv(_queryID[1],
			GL_QUERY_RESULT_AVAILABLE,
			&stopTimerAvailable);
	}
	glGetQueryObjectui64v(_queryID[0], GL_QUERY_RESULT, &_startTime);
	glGetQueryObjectui64v(_queryID[1], GL_QUERY_RESULT, &_stopTime);
	_deltaTime = (_stopTime - _startTime) / 1000000.0;
}

double TimeQuery::getDeltaTime()
{
	return _deltaTime;
}

double TimeQuery::getFPS()
{
	return 1000.0 / _deltaTime;
}