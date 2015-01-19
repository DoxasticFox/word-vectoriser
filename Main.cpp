#include "NSphere.h"
#include "Corpus.h"
#include "Learner.h"
#include <iostream>

int main (int argc, char** argv) {
	std::string dir;
	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " corpusdirectory" << std::endl;
		return 0;
	} else {
		dir = argv[1];
	}

	std::cout.precision(20);

	int pageSize = 50;
	auto c = Corpus(dir, pageSize);
	//Corpus::WordList words = c.computeMostFrequentWords();
	//c.writeDictionary(words);

	Corpus::Lexicon lexicon = c.readDictionary(10000);

	unsigned contextSize = 6;
	unsigned dimensionality = 200;
	auto l = Learner(lexicon, contextSize, dimensionality);

	std::cout << std::endl;
	while (true) {
		std::string s;
		std::string t;
		s = "but";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "and";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "so";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "if";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "that";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "list";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "up";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "thousands";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "mr";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "mrs";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "december";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "wednesday";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "thursday";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "friday";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "would";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "will";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "a";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "an";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "the";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "they";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "we";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "i";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "are";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "is";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "be";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "to";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "from";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "buy";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "$";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "muslim";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "u.s.";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "iraq";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "iran";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "syria";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "america";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "australia";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "japan";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "korea";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "government";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = "and";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = ".";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;
		s = ",";
		t = l.computeNearestWord(s);
		std::cout << s << " " << t << " " << l.computeDistance(s, t) << std::endl;

		Corpus::Page page;
		c.getPage(page);
		l.learnFromPage(page);
		std::cout << std::endl;
	}

	return 0;

	//unsigned a, b, x, y;
	//NSphere ns = NSphere(100);
	//ns.addRandomUnitPoints(5000);

	//ns.computeFurthestPoints(a, b);
	//ns.computeNearestPoints(x, y);

	//a = 0;
	//b = 1;
	//ns.setWeight(a, b, 0.5);
	//ns.setWeight(0, 10, 0.5);
	//ns.setWeight(0, 11, 0.5);
	//ns.setWeight(0, 12, 0.5);
	//ns.setWeight(1, 10, 0.5);
	//ns.setWeight(1, 11, 0.5);
	//ns.setWeight(1, 12, 0.5);

	//std::cout << "To reduce:   " << std::fixed << ns.computeDistance(a, b)  << std::endl;
	//std::cout << "Goal (est.): " << std::fixed << ns.computeDistance(x, y)  << std::endl;
	//std::cout << "Energy:      " << ns.computeEnergy() << std::endl << std::endl;

	////for (int i = 0; i < 10; i++)
		//ns.solve();

	////ns.computeFurthestPoints(a, b);
	//ns.computeNearestPoints(x, y);

	//std::cout << std::endl;
	//std::cout << "Reduced?:    " << std::fixed << ns.computeDistance(a, b)  << std::endl;
	//std::cout << "Goal:        " << std::fixed << ns.computeDistance(x, y)  << std::endl;
	//std::cout << "Energy:      " << ns.computeEnergy() << std::endl;

	//std::cout << std::endl;
	//std::cout << "Distance: " << ns.computeDistance(0, 1) << std::endl;
	//std::cout << "Distance: " << ns.computeDistance(0, 2) << std::endl;
	//std::cout << "Distance: " << ns.computeDistance(1, 2) << std::endl;

	//std::cout << std::endl;
	//std::cout << "Pairwise Energy: " << ns.computeEnergyInPoints(0, 1) << std::endl;
	//std::cout << "Pairwise Energy: " << ns.computeEnergyInPoints(0, 2) << std::endl;
	//std::cout << "Pairwise Energy: " << ns.computeEnergyInPoints(1, 2) << std::endl;

	//std::cout << std::endl;
	//std::cout << "Pointwise Energy: " << ns.computeEnergyDueToPoint(0) << std::endl;
	//std::cout << "Pointwise Energy: " << ns.computeEnergyDueToPoint(1) << std::endl;
	//std::cout << "Pointwise Energy: " << ns.computeEnergyDueToPoint(2) << std::endl;
	
	//return 0;
}
