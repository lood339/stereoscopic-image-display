#include <conio.h>
#include "FileIO.h"


FileIO::FileIO()
{
	m_currentIndex = 0;
	m_os = NULL;
}


FileIO::~FileIO()
{
	if(m_os)
	{
		delete m_os, m_os = NULL;
	}
}

bool FileIO::loadConfigFile( const string &fileName)
{
	m_files.clear();
	ifstream ifile(fileName.c_str(), ios_base::in);
	int num = 0;
	if(!ifile)
	{
		_cprintf("can't open file %s, please open a new config file.\n", fileName.c_str());
		return false;	
	}
	ifile>>num;
	assert(num > 0);
	assert(ifile.good());
	for(int i = 0; i<num; ++i)
	{
		string name;
		ifile>>name;
		m_files.push_back(name);
	}
	assert(ifile.good());

	m_resultFileName = fileName.substr(0, fileName.length()-4) + string("_result.txt");
	m_os = new ofstream(m_resultFileName.c_str(), ios_base::out);
	assert(m_os);

	return true;
}

bool FileIO::hasNextFile()
{
	unsigned int nextIndex = m_currentIndex + 1;
	if(nextIndex < m_files.size())
	{
		m_currentIndex = nextIndex;
		return true;
	}
	else
	{
		return false;
	}
}

string FileIO::currentFileName()
{
	if(m_currentIndex < m_files.size())
	{
		return m_files[m_currentIndex];
	}
	return string("");
}

ofstream * FileIO::getOfStream()
{
	return m_os;
}

void FileIO::closeOutFileStream()
{
	m_os->close();
	_cprintf("save to file %s\n", m_resultFileName.c_str());
}
