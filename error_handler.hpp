#pragma once

#include "game.h"


//////////////////// Error Type ////////////////////

typedef struct Error
{
	int id;
	static int current_id;

	const char *error;
	const char *message;

	Error(const char *error, const char *message);
} _ErrorT;

int Error::current_id = 0;

Error::Error(const char *error, const char *message)
{
	id = current_id++;
	this->error = error;
	this->message = message;
}




//////////////////// Error_handler ////////////////////

class Error_handler
{
public:
	void report(_ErrorT error)			{ reports.push_back(error); }
	std::vector<_ErrorT> getReports()	{ return reports; }

private:
	std::vector<_ErrorT> reports;
};