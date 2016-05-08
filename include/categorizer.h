#ifndef CATEGORIZER_H
#define CATEGORIZER_H
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <memory>

using namespace std;
using namespace cv;

typedef struct
{
	double frame;
	double millisecond;
	shared_ptr<Mat> value;
} GroupMember;

typedef struct
{
	shared_ptr<vector<shared_ptr<GroupMember>>> members;
} Group;

class Categorizer
{
public:
	Categorizer();
	void addToGroup(const vector<shared_ptr<Mat>>& pictures, double frame, double millisecond);
	shared_ptr<vector<shared_ptr<Group>>> getGroups() const;
private:
	Scalar getMSSIM(const Mat& i1, const Mat& i2);
	double getPSNR(const Mat& i1, const Mat& i2);
	int getHashDiff(const Mat& src1, const Mat& src2);
	bool isSimilar(Scalar& mssim);
	bool isSimilar(double psnr);
	bool isSimilar(int hash);
	shared_ptr<vector<shared_ptr<Group>>> _groups;
};

shared_ptr<GroupMember> makeGroupMember(const shared_ptr<Mat>& value, double frame, double millisecond);

shared_ptr<Group> createGroup();

#endif // !CATEGORIZER_H