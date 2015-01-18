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
	void exportnSphere();
private:
	int n;
	const std::string NIL_WORD = "NIL";
	const unsigned CONTEXT_SIZE;
	const unsigned DIMENSIONALITY;
	Learner::WordToWordIndexMap wordToWordIndexMap;
	NSphere nSphere;

	unsigned getWordIndex(const std::string& word);
	Context getContext(const Sentence& sentence, unsigned wordIndex);
	void addContextToWordIndex(Learner::WordIndexToContextsMap& map, const Context& c, unsigned wIdx);
	void decWeight(unsigned wIdx1, unsigned wIdx2, float dec=1.0);
	float computeDecrement(Contexts& C1s, Contexts& C2s);
	int getRunLength(Contexts& Cs, int i);
};

#endif
