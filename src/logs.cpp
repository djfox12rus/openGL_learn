#include "../headers/logs.h"


int LOGS::InitLogFile()
{
	std::string str;
	Settings::SettingsFileHandler file;
	str = file.ReadPathToLogsFolder();
	str.append(get_current_date());
	str.append("__log.txt");
	LOG_STREAM().open(str, std::fstream::out | std::fstream::app);

	if (LOG_STREAM().is_open()) {
		LOG_STREAM() << "Log started at " << get_current_date() << "." << std::endl;
		CAN_LOG() = true;
		return 0;
	}
	return -1;
}

void LOGS::SaveLogFile()
{
	LOG_STREAM() << "Log ended at " << get_current_date() << "." << std::endl;
	LOG_STREAM().close();
}

const char * LOGS::get_current_date()
{
	static char buffer[80];
	time_t seconds = time(NULL);
	tm timeinfo;
	localtime_s(&timeinfo, &seconds);
	char* format = "%d_%m_%Y__%Hh_%Mm_%Ss";
	strftime(buffer, 80, format, &timeinfo);
	return buffer;
}
