#include "Learner.h"

Learner::Learner(
		  const Learner::WordList& lexicon
		, unsigned contextSize
		, unsigned dimensionality)
	: wordToWordIndexMap()
	, nSphere(dimensionality)
	, CONTEXT_SIZE(contextSize)
	, DIMENSIONALITY(dimensionality)
	, n(0)
{
	if (lexicon.size() > this->nSphere.getMaxPoints()) {
		throw "Lexicon size exceeds nSphere capacity";
	}
	if (this->CONTEXT_SIZE <= 0) {
		throw "Context size not non-zero";
	}
	if (this->CONTEXT_SIZE % 2 != 0) {
		throw "Context size not even";
	}

	for (auto i = lexicon.begin(); i != lexicon.end(); i++) {
		this->wordToWordIndexMap.emplace(
				  *i
				, this->nSphere.addRandomUnitPoint());
	}
	this->wordToWordIndexMap.emplace(
			  this->NIL_WORD
			, this->nSphere.getNumPoints());
}

void Learner::decWeight(unsigned wIdx1, unsigned wIdx2, float dec)
{
	if (dec <= std::numeric_limits<float>::epsilon()) {
		return;
	}
	float weight = this->nSphere.getWeight(wIdx1, wIdx2);
	weight *= pow(0.99, dec);
	this->nSphere.setWeight(wIdx1, wIdx2, weight);
}
void Learner::learnFromPage(const Learner::Page& page)
{
	this->exportnSphere();

	auto wordIndexToContextsMap = Learner::WordIndexToContextsMap();

	/* Compute contexts */
	std::cout << "Learner::learnFromPage: Computing contexts..." << std::endl;
	for (auto s = page.begin(); s != page.end(); ++s) {          // s = sentence
		unsigned i = 0; // position of `w` in sentence
		for (auto w = s->begin(); w != s->end(); ++w, ++i) {     // w = word
			unsigned wordIndex = this->getWordIndex(*w);
			Learner::Context c = this->getContext(*s, i++);
			this->addContextToWordIndex(wordIndexToContextsMap, c, wordIndex);
		}
	}

	/* Sort each list of contexts */
	std::cout << "Learner::learnFromPage: Sorting contexts..." << std::endl;
	for (auto i  = wordIndexToContextsMap.begin();
			  i != wordIndexToContextsMap.end();
			  i++) {
		std::sort(i->second.begin(), i->second.end());
	}

	/* update the mutual charges between points */
	std::cout << "Learner::learnFromPage: Updating charges..." << std::endl;
	for (auto i  = wordIndexToContextsMap.begin();
			  i != wordIndexToContextsMap.end();
			  i++) {
		for (auto j = std::next(i); j != wordIndexToContextsMap.end(); j++) {
			unsigned wordIndex1 = i->first;
			unsigned wordIndex2 = j->first;
			Learner::Contexts contexts1 = i->second;
			Learner::Contexts contexts2 = j->second;
			float dec = this->computeDecrement(contexts1, contexts2);
			this->decWeight(wordIndex1, wordIndex2, dec);
		}
	}
	
	/* Improve the model */
	std::cout << "Learner::learnFromPage: Solving n-sphere..." << std::endl;
	//for (int i = 0; i < 20; i++)
		this->nSphere.solve();
	//std::cout << std::endl;
}

float Learner::computeDecrement(Contexts& C1s, Contexts& C2s)
{
	int i = 0;
	int j = 0;
	float dec = 0.0;
	while (i < C1s.size() && j < C2s.size()) {
		if (C1s[i] < C2s[j]) {
			i++;
		} else if (C1s[i] > C2s[j]) {
			j++;
		} else {
			int runLengthC1s = this->getRunLength(C1s, i);
			int runLengthC2s = this->getRunLength(C2s, j);
			dec += sqrt(runLengthC1s * runLengthC2s);
			i += runLengthC1s;
			j += runLengthC2s;
		}
	}
	return dec;
}

int Learner::getRunLength(Contexts& Cs, int i)
{
	int result = 1;
	while (i + result < Cs.size()) {
		if (Cs[i] == Cs[i + result]) {
			result++;
		} else {
			break;
		}
	}
	return result;
}
float Learner::computeDistance(Word w1, Word w2)
{
	if (w1 == NIL_WORD) {
		return this->nSphere.getDefaultWeight();
	}
	if (w2 == NIL_WORD) {
		return this->nSphere.getDefaultWeight();
	}
	if (this->wordToWordIndexMap.find(w1) == this->wordToWordIndexMap.end()) {
		return this->nSphere.getDefaultWeight();
	}
	if (this->wordToWordIndexMap.find(w2) == this->wordToWordIndexMap.end()) {
		return this->nSphere.getDefaultWeight();
	}

	return this->nSphere.computeDistance(
			  this->getWordIndex(w1)
			, this->getWordIndex(w2));
}

/*
 * Computes the context of a word in a `sentence` positioned at `wordIndex`. The
 * context is given in terms of the indices stored in `this->nSphere`.
 */
Learner::Context Learner::getContext(
		  const Sentence& sentence
		, unsigned wordIndex)
{
	auto context = Learner::Context(this->CONTEXT_SIZE);
	/* Iterating through words in `sentence`... */
	for (int i  = (int)wordIndex - (int)this->CONTEXT_SIZE/2;
			 i <= (int)wordIndex + (int)this->CONTEXT_SIZE/2;
			 i++) {
		if (i == wordIndex) {
			continue;
		}

		if (i < 0 || i >= sentence.size()) {
			context.emplace_back(this->getWordIndex(this->NIL_WORD));
		} else {
			context.emplace_back(this->getWordIndex(sentence[i]));
		}
	}

	return context;
}

void Learner::addContextToWordIndex(
		  Learner::WordIndexToContextsMap& map
		, const Learner::Context& c
		, unsigned wIdx)
{
	if (map.find(wIdx) == map.end()) {
		map.emplace(wIdx, Learner::Contexts());
	}
	map[wIdx].emplace_back(c);
}

unsigned Learner::getWordIndex(const std::string& word)
{
	if (this->wordToWordIndexMap.find(word) == this->wordToWordIndexMap.end()) {
		return this->wordToWordIndexMap[this->NIL_WORD];
	} else {
		return this->wordToWordIndexMap[word];
	}
}

Learner::Word Learner::computeNearestWord(Word w)
{
	if (w == NIL_WORD) {
		return NIL_WORD;
	}
	if (this->wordToWordIndexMap.find(w) == this->wordToWordIndexMap.end()) {
		return NIL_WORD;
	}

	unsigned wordIndex = this->nSphere.computeNearestPoint(
			this->getWordIndex(w));

	/* Find word given its `wordIndex`. O(n) operation in wordToWordIndexMap's
	 * length. */
	for (auto i  = this->wordToWordIndexMap.begin();
			  i != this->wordToWordIndexMap.end();
			  ++i) {
		if (i->second == wordIndex) {
			return i->first;
		}
	}

	/* shouldn't happen */
	return "";
}

void Learner::exportnSphere() {
	std::ofstream pyDict;
	std::stringstream filename;
	filename << "/tmp/points/dump-";
	filename << std::setw(4) << std::setfill('0') << this->n++;
	filename << ".py";
	pyDict.open(filename.str());
	pyDict << this->nSphere.pointsToPyDict();
	pyDict.close();
}
