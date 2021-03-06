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

void Learner::learnFromPage(const Learner::Page& page)
{
	this->exportnSphere();
	
	/* Compute contexts of each word in the page */
	std::cout << "Learner::learnFromPage: Computing contexts..." << std::endl;
	auto wordIndexToContextsMap = Learner::WordIndexToContextsMap();

	for (auto s = page.begin(); s != page.end(); ++s) {          // s = sentence
		unsigned j = 0;
		for (auto w = s->begin(); w != s->end(); ++w, ++j) {     // w = word
			// figure out the `wordIndex` that belongs to the `word`
			unsigned wordIndex = this->getWordIndex(*w);

			// add an empty list of contexts for `wordIndex` if none exist yet
			auto contexts = wordIndexToContextsMap.find(wordIndex);
			if (contexts == wordIndexToContextsMap.end()) {
				wordIndexToContextsMap.emplace(wordIndex, Learner::Contexts());
			}

			// add a `context` to one of the lists in `wordIndexToContextMap`
			wordIndexToContextsMap[wordIndex].emplace_back(this->getContext(
						  *s
						, j ));
		}
	}

	/* Compute the distances between the words based on the contexts and update
	 * `this->nSphere`
	 */
	std::cout << "Learner::learnFromPage: Computing distances..." << std::endl;
	for (auto i  = wordIndexToContextsMap.begin();
			  i != wordIndexToContextsMap.end();
			  i++) {
		for (auto j = std::next(i); j != wordIndexToContextsMap.end(); j++) {
			unsigned wordIndex1 = i->first;
			unsigned wordIndex2 = j->first;
			Learner::Contexts contexts1 = i->second;
			Learner::Contexts contexts2 = j->second;
			float distance = this->computeDistance(contexts1, contexts1);

			this->nSphere.setWeight(wordIndex1, wordIndex2, distance);
		}
	}

	/* Improve the model */
	std::cout << "Learner::learnFromPage: Solving n-sphere..." << std::endl;
	//for (int i = 0; i < 20; i++)
		this->nSphere.solve();
	//std::cout << std::endl;
}

/*
 * Group average distance
 */
float Learner::computeDistance(
		  const Learner::Contexts& C1s
		, const Learner::Contexts& C2s)
{
	if (C1s.size() == 0 || C2s.size() == 0) {
		return this->nSphere.getDefaultWeight();
	}

	float distance = 0.0;
	for (int i = 0; i < C1s.size(); i++) {
		for (int j = 0; j < C2s.size(); j++) {
			distance += this->computeDistance(C1s[i], C2s[j]);
		}
	}
	return distance / (C1s.size() * C2s.size());
}

float Learner::computeDistance(
		  const Learner::Context& C1
		, const Learner::Context& C2)
{
	float distance = 0.0;
	int skipped = 0;
	for (int i = 0; i < this->CONTEXT_SIZE; i++) {
		unsigned nilWordIndex = this->getWordIndex(NIL_WORD);
		if (C1[i] == nilWordIndex || C2[i] == nilWordIndex) {
			skipped++;
			continue;
		}
		distance += this->nSphere.computeSquaredDistance(C1[i], C2[i]);
	}

	/* Finish up */
	if (this->CONTEXT_SIZE - skipped == 0) {   // Only to avoid division by zero
		return this->nSphere.getDefaultWeight();
	}
	if (distance >= std::numeric_limits<float>::epsilon()) {//Skipping nil words
		return this->nSphere.getDefaultWeight();            //might cause
	}                                                       //distance == 0.0
	distance /= this->CONTEXT_SIZE - skipped;
	return sqrt(distance);
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
	if (w == this->NIL_WORD) {
		return this->NIL_WORD;
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

	/* shouldn't ever happen */
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
