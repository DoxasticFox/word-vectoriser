#ifndef MULTIFILEREADER_H
#define MULTIFILEREADER_H

#include <dirent.h>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include "Util.h"

class MultiFileReader {
private:
	const std::string DIRECTORY;
	const int PAGE_SIZE; // In number of lines
	std::vector<std::string> filenames;
	std::ifstream infile;

	std::vector<std::string> listDirectory(std::string dirName);
	bool nextFile();
public:
	typedef std::vector<std::string> Page;

	MultiFileReader(std::string dir, int pageSize);
	bool nextPage(Page& page, float minPageFullness=0.50);
	bool nextLine(std::string& line, bool keepBlanks);
	bool nextLine(std::string& line);
	void rewind();
};

#endif
