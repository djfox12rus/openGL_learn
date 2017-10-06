#include "../headers/settings.h"

Settings::SettingsFileHandler::SettingsFileHandler() :file_stream() {
	file_stream.open("settings.ini", std::fstream::in);
	
}

Settings::SettingsFileHandler::~SettingsFileHandler()
{
	file_stream.close();
}

uint16_t Settings::SettingsFileHandler::ReadGLVersion()
{
	if (!file_stream.is_open())
		return 330;

	file_stream.seekg(file_stream.beg);
	char line[100];
	bool found = false;
	while (!file_stream.eof()) {
		file_stream.getline(line, 100);
		if (!strcmp(line, "[GLVersion]")) {
			file_stream.getline(line, 100);
			found = true;
			break;
		}
	}
	if (found) {
		uint16_t out = strtoul(line, nullptr, 0);
		return out;
	}
	return 330;
}

int Settings::SettingsFileHandler::ReadWidthHeightTitle(int & _w, int & _h, char**_t)
{
	if (!file_stream.is_open())
		return -1;
	char line[100];
	bool found = false;
	char* eq;
	while (!file_stream.eof()) {
		file_stream.getline(line, 100);
		if (!strcmp(line,"[Window]")) {
			file_stream.getline(line, 100);
			eq = strrchr(line, '=');
			eq++;
			_w = strtoul(eq, &eq, 0);		
			file_stream.getline(line, 100);
			eq = strrchr(line, '=');
			eq++;
			_h = strtoul(eq, &eq, 0);
			file_stream.getline(line, 100);
			eq = strrchr(line, '=');
			eq++;
			strcpy_s(*_t,100, eq);			
			found = true;
			break;
		}
	}
	if (!found)return -1;
	return 0;
}

int Settings::SettingsFileHandler::ReadShadersPaths(const char * _shader_name, char **_vertexPath, char **_fragmentPath)
{
	if (!file_stream.is_open())
		return -1;
	char line[100];
	bool found = false;
	while (!file_stream.eof()) {
		file_stream.getline(line, 100);
		if (!strcmp(line, _shader_name)) {
			file_stream.getline(*_vertexPath, 100);
			file_stream.getline(*_fragmentPath, 100);
			found = true;
			break;
		}
	}
	if (!found)return -1;
	return 0;
}
