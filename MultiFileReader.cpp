#include "MultiFileReader.h"

MultiFileReader::MultiFileReader(std::string dir, int pageSize)
	: filenames(this->listDirectory(dir))
	, PAGE_SIZE(pageSize)
	, DIRECTORY(dir)
{
}

bool MultiFileReader::nextPage(Page& page, float minPageFullness)
{
	page.clear();

	for (int i = 0; i < this->PAGE_SIZE; i++) {
		bool keepBlanks = false;
		std::string line;
		if (!this->nextLine(line, keepBlanks)) {
			break;
		}
		page.emplace_back(line);
	}

	return page.size() >= this->PAGE_SIZE * minPageFullness;
}

bool MultiFileReader::nextLine(std::string& line, bool keepBlanks)
{
	if (keepBlanks) {
		return this->nextLine(line);
	}

	bool ok;
	while ((ok = nextLine(line)) && line == "");
	return ok;
}
bool MultiFileReader::nextLine(std::string& line)
{
	bool ok;
	if (ok = std::getline(this->infile, line)) {
		return ok;
	}
	this->nextFile();
	return std::getline(this->infile, line);
}

bool MultiFileReader::nextFile()
{

	if (this->filenames.size() == 0) {
		return false;
	}

	this->infile.close();
	this->infile.open(
			  this->DIRECTORY
			+ Util::separator()
			+ this->filenames.back());
	this->filenames.pop_back();

	return this->infile.good();
}

std::vector<std::string> MultiFileReader::listDirectory(std::string dirName)
{
	auto filenames = std::vector<std::string>();
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(dirName.c_str())) == NULL) {
		throw "Could not open directory";
	}

	while ((ent = readdir(dir)) != NULL) {
		if (ent->d_name[0] == '.') {
			continue;
		}
		filenames.emplace_back(ent->d_name);
	}
	closedir (dir);

	std::sort(filenames.rbegin(), filenames.rend()); // yes, reverse order
	return filenames;
}

void MultiFileReader::rewind()
{
	filenames = this->listDirectory(this->DIRECTORY);
	this->nextFile();
}
