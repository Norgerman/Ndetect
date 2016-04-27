#ifndef GROUP_WRITER_H
#define GROUP_WRITER_H

#include <memory>
#include <boost/filesystem.hpp>
#include <iomanip>
#include <sstream>
#include <opencv2/highgui.hpp>
#include "categorizer.h"
#include "output_modifier.h"

using namespace std;
using namespace cv;
namespace fs = boost::filesystem;

class GroupWriter
{
public:
	GroupWriter();
	void writeGroup(vector<shared_ptr<Group>>& groups, const string& outputDir, const string& groupNamePrefix);
	void writeGroup(vector<shared_ptr<Group>>& groups, const string& outputDir, const string& groupNamePrefix, const OutputModifier& modifier);
private:
	shared_ptr<OutputModifier> _defaultModifier;
};

#endif // !GROUP_WRITER_H