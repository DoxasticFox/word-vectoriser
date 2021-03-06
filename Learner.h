#ifndef LEARNER_H
#define LEARNER_H

#include "NSphere.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <unordered_map>
#include <vector>
#include <limits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

class Learner {
public:
	typedef std::string                            Word;
	typedef std::vector<Word>                      WordList;
	typedef std::vector<Word>                      Lexicon;
	typedef std::unordered_map<Word, unsigned>     WordToWordIndexMap;
	typedef WordList                               Sentence;
	typedef std::vector<Sentence>                  Page;
	typedef std::vector<unsigned>                  Context;
	typedef std::vector<Context>                   Contexts;
	typedef std::unordered_map<unsigned, Contexts> WordIndexToContextsMap;

	Learner(const Learner::WordList& lexicon
			, unsigned contextSize
			, unsigned dimensionality);
	void learnFromPage(const Learner::Page& page);
	float computeDistance(Word w1, Word w2);
	Word  computeNearestWord(Word w);
	void  exportnSphere();
private:
	int n;
	Learner::WordToWordIndexMap wordToWordIndexMap;
	NSphere nSphere;

	unsigned getWordIndex(const std::string& word);
	Context getContext(const Sentence& sentence, unsigned wordIndex);
	float   computeDistance(const Learner::Context& C1, const Learner::Context& C2);
	float   computeDistance(const Learner::Contexts& C1s, const Learner::Contexts& C2s);
	const std::string NIL_WORD = "NIL";
	const unsigned CONTEXT_SIZE;
	const unsigned DIMENSIONALITY;

};

#endif
