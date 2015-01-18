#include "NSphere.h"

NSphere::NSphere(unsigned dimensionality)
{
	this->dimensionality = dimensionality;
	
	this->numPoints = 0;
	
	this->points = new float*[MAX_POINTS];
	#pragma omp parallel for schedule(static,256) num_threads(4)
	for (unsigned i = 0; i < this->MAX_POINTS; i++) {
		this->points[i] = new float[dimensionality];
	}
	
	this->weights = new float[MAX_WEIGHTS];
	#pragma omp parallel for schedule(static,256) num_threads(4)
	for (unsigned i = 0; i < MAX_WEIGHTS; i++) {
		this->weights[i] = this->getDefaultWeight();
	}
}
float NSphere::computeDistance(unsigned i, unsigned j)
{
	return this->computeDistance(this->points[i], this->points[j]);
}
float NSphere::computeDistance(float* p, float* q)
{
	float distance = this->computeSquaredDistance(p, q);
	distance = sqrt(distance);
	return distance;
}
float NSphere::computeSquaredDistance(unsigned i, unsigned j)
{
	return this->computeSquaredDistance(this->points[i], this->points[j]);
}
float NSphere::computeSquaredDistance(float* p, float* q)
{
	float distance = 0.0;
	for (int i = 0; i < this->dimensionality; i++) {
		float distanceI = p[i] - q[i];
		distance += distanceI * distanceI;
	}
	return distance;
}
float NSphere::computeEnergy()
{
	float energy = 0.0;
	for (int i = 0; i < this->numPoints; i++) {
		for (int j = i + 1; j < this->numPoints; j++) {
			energy += this->computeEnergyInPoints(i, j);
		}
	}
	return energy;
}
float NSphere::computeEnergyDueToPoint(unsigned i)
{
	float energy = 0.0;
	#pragma omp parallel for reduction(+:energy) schedule(static,256) num_threads(4)
	for (int i_ = 0; i_ < this->numPoints; i_++) {
		energy += this->computeEnergyInPoints(i, i_);
	}
	return energy;
}
float NSphere::computeEnergyInPoints(unsigned i, unsigned j)
{
	float distance = this->computeDistance(i, j);
	if (distance <= std::numeric_limits<float>::epsilon()) {
		return 0.0;
	}
	return - this->getWeight(i, j) / distance;
}
float NSphere::computeLength(unsigned i)
{
	return this->computeLength(this->points[i]);
}
float NSphere::computeLength(float* p)
{
	float length = 0.0;
	for (int i = 0; i < this->dimensionality; i++) {
		length += p[i] * p[i];
	}
	length = sqrt(length);
	return length;
}
float NSphere::getWeight(unsigned i, unsigned j)
{
	unsigned index;
	this->weightIndexesToWeightArrayIndex(index, i, j);
	return this->weights[index];
}
float NSphere::randUniform(float range)
{
	int rand = std::rand() - RAND_MAX / 2; // interval [-RAND_MAX/2, RAND_MAX/2]
	float rand_ = rand / (RAND_MAX / 2.0); // interval [-1, 1]
	return rand_ * range;
}
float NSphere::getDefaultWeight()
{
	return sqrt(2.0);
}
float* NSphere::getPoint(unsigned i)
{
	return this->points[i];
}
float* NSphere::getPointClone(unsigned i)
{
	float* point = new float[this->dimensionality];
	this->copyPoint(this->points[i], point);
	return point;
}
float* NSphere::makeRandomUnitPoint()
{
	float* point = new float[this->dimensionality];
	for (int i = 0; i < this->dimensionality; i++) {
		point[i] = this->randUniform(1.0);
	}
	this->normalisePoint(point);
	return point;
}
std::string NSphere::pointToString(float* p)
{
	if (this->dimensionality == 0) {
		return "[]";
	}

	typedef std::numeric_limits<float> fl;
	std::ostringstream ss;
	ss.precision(fl::digits10);

	ss << "[ ";
	for (int i = 0; i < this->dimensionality - 1; i++) {
		ss << std::fixed << p[i];
		ss << ",";
	}
	ss << std::fixed << p[this->dimensionality - 1];
	ss << "]";
	return ss.str();
}
std::string NSphere::pointToString(unsigned i)
{
	return this->pointToString(this->points[i]);
}
std::string NSphere::pointsToString()
{
	std::ostringstream ss;
	for (int i = 0; i < this->numPoints; i++) {
		ss << this->pointToString(i) << std::endl;
	}
	return ss.str();
}
std::string NSphere::pointsToPyDict()
{
	if (this->numPoints == 0) {
		return "";
	}

	std::ostringstream ss;
	ss << "{" << std::endl;
	for (int i = 0; i < this->numPoints - 1; i++) {
		ss << "\t";
		ss << "\"" << std::to_string(i) << "\": ";
		ss << this->pointToString(i);
		ss << "," << std::endl;
	}
	ss << "\"" << std::to_string(this->numPoints - 1) << "\": ";
	ss << this->pointToString(this->numPoints - 1);
	ss << "}";
	return ss.str();
}
unsigned NSphere::computeNearestPoint(unsigned i)
{
	return this->computeNearestPoint(this->points[i]);
}
unsigned NSphere::computeNearestPoint(float* p)
{
	float minDistance = std::numeric_limits<float>::max();
	unsigned minPoint;
	for (int i = 0; i < this->numPoints; i++) {
		if (p == this->points[i]) {
			continue;
		}
		float thisDistance = this->computeDistance(p, this->points[i]);
		if (thisDistance < minDistance) {
			minDistance = thisDistance;
			minPoint = i;
		}
	}
	return minPoint;
}
unsigned NSphere::computeFurthestPoint(unsigned i)
{
	return this->computeFurthestPoint(this->points[i]);
}
unsigned NSphere::computeFurthestPoint(float* p)
{
	float maxDistance = std::numeric_limits<float>::min();
	unsigned maxPoint;
	for (int i = 0; i < this->numPoints; i++) {
		float thisDistance = this->computeDistance(p, this->points[i]);
		if (thisDistance > maxDistance) {
			maxDistance = thisDistance;
			maxPoint = i;
		}
	}
	return maxPoint;
}
unsigned NSphere::getNumPoints()
{
	return this->numPoints;
}
unsigned NSphere::getDimensionality()
{
	return this->dimensionality;
}
unsigned NSphere::getMaxPoints()
{
	return this->MAX_POINTS;
}
unsigned NSphere::addPoint(float* p)
{
	this->setPoint(p, this->numPoints);
	return this->numPoints++;
}
unsigned NSphere::addRandomUnitPoint()
{
	float* point = this->makeRandomUnitPoint();
	unsigned index = this->addPoint(point);
	delete point;
	return index;
}
void NSphere::addRandomUnitPoints(unsigned i)
{
	for (int i_ = 0; i_ < i; i_++) {
		this->addRandomUnitPoint();
	}
}
void NSphere::computeNearestPoints(unsigned &i, unsigned &j)
{
	float minDistance = std::numeric_limits<float>::max();
	for (int i_ = 0; i_ < this->numPoints; i_++) {
		unsigned j_ = this->computeNearestPoint(i_);
		float thisDistance = this->computeDistance(i_, j_);
		if (thisDistance < minDistance) {
			minDistance = thisDistance;
			i = i_;
			j = j_;
		}
	}
}
void NSphere::computeFurthestPoints(unsigned &i, unsigned &j)
{
	float maxDistance = std::numeric_limits<float>::min();
	for (int i_ = 0; i_ < this->numPoints; i_++) {
		unsigned j_ = this->computeFurthestPoint(i_);
		float thisDistance = this->computeDistance(i_, j_);
		if (thisDistance > maxDistance) {
			maxDistance = thisDistance;
			i = i_;
			j = j_;
		}
	}
}
void NSphere::copyPoint(float* source, float* dest)
{
	std::memcpy(dest, source, this->dimensionality * sizeof(float));
}
void NSphere::normalisePoint(unsigned i)
{
	this->normalisePoint(this->points[i]);
}
void NSphere::normalisePoint(float* p)
{
	float length = this->computeLength(p);
	this->scalarMultiplication(p, 1.0/length);
}
void NSphere::perturbPoint(float* p, float d)
{
	for (int i = 0; i < this->dimensionality; i++) {
		p[i] += ((rand() % 3) - 1) * d;
	}
}
void NSphere::scalarMultiplication(float* p, float scalar)
{
	for (int i = 0; i < this->dimensionality; i++) {
		p[i] *= scalar;
	}
}
void NSphere::setWeight(unsigned i, unsigned j, float weight)
{
	unsigned index;
	this->weightIndexesToWeightArrayIndex(index, i, j);
	this->weights[index] = weight;
}
void NSphere::solve()
{
	float totalEnergyRise = 0.0;

	for (int i = 0; i < 10; i++) {
		for (int j = 10; j >= 0; j--) {
			float energyRise = this->solveStep(2.0 * pow(j+3.5, -3.5));
			//std::cout << energyRise << std::endl;
			totalEnergyRise += energyRise;
		}
		//std::cout << std::endl;
	}

	std::cout << totalEnergyRise << " *" << std::endl;
}
float NSphere::solveStep(float inc)
{
	float totalEnergyRise = 0.0;

	for (int i = 0; i < this->numPoints; i++) {
		float energyRise = 0.0;
		
		float* savedPoint = this->getPointClone(i);

		energyRise -= this->computeEnergyDueToPoint(i);
		this->perturbPoint(this->points[i], inc);
		this->normalisePoint(this->points[i]);
		energyRise += this->computeEnergyDueToPoint(i);

		if (energyRise >= 0) {
			// Keep change
			totalEnergyRise += energyRise;
		} else {
			// Undo change
			this->setPoint(savedPoint, i);
		}
		delete savedPoint;
	}

	return totalEnergyRise;
}
void NSphere::setPoint(float* p, unsigned i)
{
	this->copyPoint(p, this->points[i]);
}
void NSphere::weightIndexesToWeightArrayIndex(
		  unsigned &index
		, unsigned i
		, unsigned j)
{
	if (i <= j) {
		index = j * (j - 1) / 2 + i;
	} else {
		index = i * (i - 1) / 2 + j;
	}
}
