#ifndef FILE_IO_H
#define FILE_IO_H 1

// output stream

#include "ImageModel.h"
#include <vector>
#include <string>
#include <fstream>

using namespace std;

// file input and out for multiple image settings
class FileIO
{
public:
	FileIO();
	~FileIO();

	bool loadConfigFile( const string &fileName);
	bool hasNextFile();
	string currentFileName();
	ofstream * getOfStream();
	void closeOutFileStream();

private:
	vector<string> m_files;  
	unsigned int   m_currentIndex;
	ofstream       *m_os;
	string         m_resultFileName;

};

#endif
