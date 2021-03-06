#ifndef CATEGORIZER_H
#define CATEGORIZER_H
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <memory>
#include <unordered_set>
#include "knn_matcher.h"
#include "make_unique.hpp"

using namespace std;
using namespace cv;

typedef struct
{
	double frame;
	double millisecond;
	Mat value;
} GroupMember;

typedef struct
{
	shared_ptr<vector<shared_ptr<GroupMember>>> members;
} Group;

class Categorizer
{
public:
	Categorizer();
	void addToGroup(const vector<Mat>& pictures, double frame, double millisecond);
	shared_ptr<vector<shared_ptr<Group>>> getGroups() const;
private:
	Scalar getMSSIM(const Mat& i1, const Mat& i2);
	double getHistDiff(const Mat& i1, const Mat& i2);
	int getHashDiff(const Mat& src1, const Mat& src2);
	float getKnnDiff(const Mat& src1, const Mat& src2);
	Scalar getGroupMSSIM(const Mat& pic, const Group& g);

	bool isSimilar(Scalar& mssim);
	bool isSimilar(double diff);
	bool isSimilar(int hash);
	bool isSimilar(float average);

	void moreSimilar(double& currentSimilarValue, size_t& currentSimilarValueIndex,
		const double& newValue, const size_t& newIndex,
		const unordered_set<int>& usedIndex);

	void moreSimilar(int& currentSimilarValue, size_t& currentSimilarValueIndex,
		const int& newValue, const size_t& newIndex,
		const unordered_set<int>& usedIndex);

	void moreSimilar(Scalar& currentSimilarValue, size_t& currentSimilarValueIndex,
		const Scalar& newValue, const size_t& newIndex, 
		const unordered_set<int>& usedIndex);

	void moreSimilar(float& currentSimilarValue, size_t& currentSimilarValueIndex,
		const float& newValue, const size_t& newIndex,
		const unordered_set<int>& usedIndex);

	shared_ptr<vector<shared_ptr<Group>>> _groups;
	unique_ptr<KnnMatcher> _matcher;
};

shared_ptr<GroupMember> makeGroupMember(const Mat& value, double frame, double millisecond);

shared_ptr<Group> createGroup();

#endif // !CATEGORIZER_H