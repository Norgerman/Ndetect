#include <iostream>
#include <boost/filesystem.hpp>
#include "invalid_params_exception.h"
#include "detector.h"

using namespace std;
using namespace cv;
namespace fs = boost::filesystem;

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cout << "No picture input" << endl;
		return -1;
	}

	try
	{
		Detector d(argv[1]);
		auto result = d.detect();
		auto resultpic = d.markOnSource(result);
		if (!fs::exists("./test"))
		{
			fs::create_directory("./test");
		}
		imwrite("./test/source.jpg", *d.getImage());
		imwrite("./test/marked.jpg", *resultpic);
	}
	catch (const InvalidParamsException& e)
	{
		cout << e.msg << endl;
		return -1;
	}
}