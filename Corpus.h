#ifndef CORPUS_H
#define CORPUS_H

#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include "MultiFileReader.h"
#include "Util.h"

class Corpus {
public:
	typedef std::string                            Word;
	typedef std::vector<Word>                      WordList;
	typedef std::vector<Word>                      Lexicon;
	typedef WordList                               Sentence;
	typedef std::vector<Sentence>                  Page;
private:
	const std::string DIRECTORY;
	MultiFileReader* reader;

	std::vector<std::string> listDirectory(std::string& dirName);
	Corpus::Page readerPageToCorpusPage(const MultiFileReader::Page& readerPage);
public:
	Corpus(std::string dir, int pageSize);
	~Corpus();
	WordList readDictionary(unsigned n);
	WordList computeMostFrequentWords();
	void writeDictionary(WordList& dictionary);
	bool getPage(Corpus::Page& page);
	void rewind();
};

#endif
