#pragma once
#ifndef SETINGS_H
#define SETINGS_H
#include "STD_commons.h"
namespace Settings {

	class SettingsFileHandler {
	private:
		std::ifstream file_stream;
	public:
		SettingsFileHandler();
		~SettingsFileHandler();

		inline std::ifstream& SETTINGS_STREAM() {
			return file_stream;
		}
		uint16_t ReadGLVersion();
		int ReadWidthHeightTitle(int& _w, int& _h,char**_t);

		int ReadShadersPaths(const char * _shader_name, char **, char **);

	};
}

#endif // !SETINGS_H
