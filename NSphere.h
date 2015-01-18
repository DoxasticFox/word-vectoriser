#ifndef NSPHERE_H
#define NSPHERE_H

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <iostream> /* Only so that `solve` can echo energy rises */
#include <limits>
#include <sstream>
#include <string>

class NSphere {
/* IMPORTANT: In the interests of speed, this code has been written missing many
 * basic safety features like rangechecks and exceptions. Be careful using it!
 */
private:
	static const unsigned MAX_POINTS = 35000;
	static const unsigned MAX_WEIGHTS = (MAX_POINTS / 2) *
		(MAX_POINTS + 1); // = 1 + 2 + 3 + ... + MAX_POINTS

	unsigned dimensionality;
	unsigned numPoints;
	float**  points;
	float*   weights;
public:
	            NSphere(unsigned dimensionality=3);
	float       computeDistance(float* p, float* q);
	float       computeDistance(unsigned i, unsigned j);
	float       computeSquaredDistance(float* p, float* q);
	float       computeSquaredDistance(unsigned i, unsigned j);
	float       computeEnergy();
	float       computeEnergyDueToPoint(unsigned  i);
	float       computeEnergyInPoints(unsigned  i, unsigned j);
	float       computeLength(float* p);
	float       computeLength(unsigned i);
	float       getWeight(unsigned i, unsigned j);
	float       randUniform(float range);
	float       getDefaultWeight();
	float*      getPoint(unsigned i);
	float*      getPointClone(unsigned i);
	std::string pointToString(float* p);
	std::string pointToString(unsigned i);
	std::string pointsToString();
	std::string pointsToPyDict();
	unsigned    computeNearestPoint(float* p);
	unsigned    computeNearestPoint(unsigned    i);
	unsigned    computeFurthestPoint(float* p);
	unsigned    computeFurthestPoint(unsigned    i);
	unsigned    getNumPoints();
	unsigned    getDimensionality();
	unsigned    getMaxPoints();
	unsigned    addPoint(float* p);
	unsigned    addRandomUnitPoint();
	void        addRandomUnitPoints(unsigned i);
	void        computeNearestPoints(unsigned &i, unsigned &j);
	void        computeFurthestPoints(unsigned &i, unsigned &j);
	void        copyPoint(float* source, float* dest);
	void        normalisePoint(float* p);
	void        normalisePoint(unsigned  i);
	void        perturbPoint(float* p, float d);
	void        scalarMultiplication(float* p, float scalar);
	void        setPoint(float* p, unsigned i);
	void        setWeight(unsigned i, unsigned j, float weight);
	void        solve();
	float       solveStep(float inc);
private:
	float*      makeRandomUnitPoint();
	void        weightIndexesToWeightArrayIndex(
					unsigned &index,
					unsigned i,
					unsigned j);
};

#endif
