#pragma once
#ifndef LOGS_H
#define LOGS_H
#include "settings.h"
namespace LOGS {
	inline bool & CAN_LOG() {
		static bool _test = false;
		return _test;
	}

	inline std::ofstream& LOG_STREAM() {
		static std::ofstream LOG;
		return LOG;
	}

	int InitLogFile();

	void SaveLogFile();

	const char* get_current_date();


}

#endif // !LOGS_H
