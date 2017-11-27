#pragma once


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Manager.h"
#include "Singleton.h"
#include "Address.h"
#include "Packet.h"

#define DEFAULT_FILE_NAME "log.txt"

class CLogManager final
	: public IManager, public CSingleton<CLogManager>
{
public:
	CLogManager();
	~CLogManager() override;

	void StartUp() override;
	void ShutDown() override;

	void WriteLine(std::string& msg, bool newline = true);

private:

	std::ofstream m_file;


};


