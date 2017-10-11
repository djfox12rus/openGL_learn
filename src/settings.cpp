#include "../headers/settings.h"

Settings::SettingsFileHandler::SettingsFileHandler() :file_stream() {
	file_stream.open("settings.ini", std::fstream::in);
	
}

Settings::SettingsFileHandler::~SettingsFileHandler()
{
	if (file_stream.is_open())
		file_stream.close();
}

uint16_t Settings::SettingsFileHandler::ReadGLVersion()
{
	if (!file_stream.is_open())
		return 330;	
	std::string str;
	bool found = false;
	while (!file_stream.eof()) {
		file_stream >> str;
		if (!str.compare("[GLVersion]")) {
			file_stream >> str;			
			found = true;
			break;
		}
	}
	if (found) {
		uint16_t out = strtoul(str.c_str(), nullptr, 0);
		return out;
	}
	return 330;
}

int Settings::SettingsFileHandler::ReadWidthHeightTitle(int & _w, int & _h, std::string* _t)
{
	if (!file_stream.is_open())
		return -1;
	
	std::string str;
	bool found = false;
	char* eq;
	while (!file_stream.eof()) {
		file_stream >> str;
		if (!str.compare("[Window]")) {
			file_stream >> str;	
			eq = &str[str.find("=")];
			eq++;
			_w = strtoul(eq, &eq, 0);		
			file_stream >> str;
			eq = &str[str.find("=")];
			eq++;
			_h = strtoul(eq, &eq, 0);
			if (_t != nullptr) {
				file_stream >> str;
				eq = &str[str.find("=")];
				eq++;
				_t->assign(eq);				
			}
			found = true;
			break;
		}
	}
	if (!found)return -1;
	return 0;
}

int Settings::SettingsFileHandler::ReadShadersPaths(const char * _shader_name, std::string &_vertexPath, std::string &_fragmentPath)
{
	if (!file_stream.is_open())
		return -1;	
	bool found = false;
	std::string str;
	while (!file_stream.eof()) {
		
		file_stream >> str;
		if (!str.compare(_shader_name)) {
			file_stream >> _vertexPath;
			file_stream >> _fragmentPath;			
			found = true;
			break;
		}
	}
	if (!found)return -1;
	return 0;
}

std::string Settings::SettingsFileHandler::ReadPathTo(const char * _model_name)
{

	if (!file_stream.is_open())
		return std::string();
	bool found = false;
	std::string str;
	while (!file_stream.eof()) {
		file_stream >> str;
		if (!str.compare(_model_name)) {
			file_stream >> str;
			found = true;
			break;
		}
	}
	if (found) {
		return str;
	}
	return std::string();
}

std::string Settings::SettingsFileHandler::ReadPathToLogsFolder()
{
	if (!file_stream.is_open())
		return std::string();
	bool found = false;
	std::string str;
	while (!file_stream.eof()) {
		file_stream >> str;	
		if (!str.compare("[LogsPath]")) {			
			file_stream >> str;
			found = true;
			break;
		}
	}
	if (found) {		
		return str;
		
	}
	return std::string();
}
