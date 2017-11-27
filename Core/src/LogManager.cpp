#include "LogManager.h"
#include <iomanip>
#include <ctime>
#include <chrono>

// TODO: Remove
#pragma warning(disable : 4996)

CLogManager::CLogManager()
	: m_file(DEFAULT_FILE_NAME)
{ 

}


void CLogManager::StartUp()
{

	if (!m_file.is_open())
	{
		std::cout << "Error - CLogManager: Unable to open / create log file" << std::endl;
		return;
	}

	m_file << "------------------------------------------------------ EROS LOG ------------------------------------------------------" << '\n';
	
}

void CLogManager::WriteLine(std::string& msg, bool newline /*= true*/)
{
	using namespace std::chrono;


	time_point<system_clock, microseconds> cur;
	cur = time_point_cast<microseconds>(system_clock::now());

	//auto cur = system_clock::now();
	auto time = system_clock::to_time_t(cur);

	m_file << "[" << std::put_time(std::localtime(&time), "%T") << "] ";
	
	m_file << "[" << time << "] - ";

	m_file << msg;
	if (newline) m_file << '\n';
}


void CLogManager::ShutDown()
{
}

CLogManager::~CLogManager()
{
	if (m_file.is_open())
	{
		m_file.close();
	}
}