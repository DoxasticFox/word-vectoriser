#include "Corpus.h"

Corpus::Corpus(std::string dir, int pageSize) : DIRECTORY(dir)
{
	this->reader = new MultiFileReader(dir, pageSize);
}

Corpus::~Corpus()
{
	delete this->reader;
}

bool Corpus::getPage(Corpus::Page& page)
{
	bool ok;
	MultiFileReader::Page readerPage;
	ok = this->reader->nextPage(readerPage);
	page = this->readerPageToCorpusPage(readerPage);
	return ok;
}

Corpus::Page Corpus::readerPageToCorpusPage(const MultiFileReader::Page& readerPage)
{
	auto corpusPage = Corpus::Page();
	for (auto i = readerPage.begin(); i != readerPage.end(); ++i) {
		auto s = Util::split(*i, ' ');
		for (auto w = s.begin(); w != s.end(); ++w) {
			std::transform(w->begin(), w->end(), w->begin(), ::tolower);
		}
		corpusPage.emplace_back(s);
	}
	return corpusPage;
}

void Corpus::rewind() {
	this->reader->rewind();
}

Corpus::WordList Corpus::readDictionary(unsigned n)
{
	std::ifstream infile(this->DIRECTORY + Util::separator() + ".dictionary");
	std::string line;
	auto wordList = Corpus::WordList();
	while (std::getline(infile, line) && n-- > 0) {
		wordList.emplace_back(line);
	}
	return wordList;
}

void Corpus::writeDictionary(WordList& dictionary)
{
	std::ofstream outfile(this->DIRECTORY + Util::separator() + ".dictionary");
	for (auto i = dictionary.begin(); i != dictionary.end(); ++i) {
		outfile << *i << "\n";
	}
}

Corpus::WordList Corpus::computeMostFrequentWords()
{
	/* Tally words */
	auto wordCounts = std::unordered_map<Corpus::Word, unsigned>();

	auto page = Corpus::Page();
	while (this->getPage(page)) {
		for (auto s = page.begin(); s != page.end(); ++s) {
			for (auto w = s->begin(); w != s->end(); ++w) {
				auto wordCount = wordCounts.find(*w);
				if (wordCount == wordCounts.end()) {
					wordCounts[*w] = 1;
				} else {
					wordCounts[*w]++;
				}
			}
		}
	}

	/* Sort words by frequency */
	typedef std::pair<Corpus::Word, unsigned> WordCount;

	struct CountCmp {
		bool operator()(const WordCount &lhs, const WordCount &rhs) {
			return lhs.second > rhs.second;
		}
	};

	auto wordCountsVector = std::vector<WordCount>(
			  wordCounts.begin()
			, wordCounts.end());
	std::sort(wordCountsVector.begin(), wordCountsVector.end(), CountCmp());

	/* Extract most frequent words */
	auto words = Corpus::WordList();
	for (auto i = wordCountsVector.begin(); i != wordCountsVector.end(); ++i) {
		words.emplace_back(i->first);
	}
	
	/* Finish up */
	this->reader->rewind();
	return words;
}
