#include <iostream>
#include <boost/filesystem.hpp>
#include <iomanip>
#include "invalid_params_exception.h"
#include "detector.h"
#include "video_loader.h"
#include "categorizer.h"
#include "group_writer.h"

using namespace std;
using namespace cv;
namespace fs = boost::filesystem;

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		cout << "input not found" << endl;
		return -1;
	}

	try
	{
		auto loader = VideoLoader(argv[1]);
		double frame = 0;
		double millionsecond = 0;
		auto categorizer = Categorizer();
		auto resultSet = vector<shared_ptr<Mat>>();
		stringstream ss;
		Mat res;
		while (loader.next(res, frame, millionsecond))
		{
			auto d = Detector(res);
			auto result = d.detect();
			d.cut(result, resultSet);
			categorizer.addToGroup(resultSet, frame, millionsecond);
			cout << "frame: " << frame << " millionsecond: " << millionsecond << endl;
			resultSet.clear();
		}
		cout << "detect completed" << endl;
		auto groups = categorizer.getGroups();
		auto writer = GroupWriter();
		writer.writeGroup(*groups, "./test", "g");
		/*auto i = 0;
		if (!fs::exists("./test"))
			fs::create_directory("./test");
		for (auto& g : *groups)
		{
			ss << "./test/g" << i;
			string gDir = ss.str();
			if (!fs::exists(gDir))
			{
				fs::create_directory(gDir);
			}
			for (auto& member : *g->members)
			{
				ss << "/" << fixed << setprecision(3) << member->millisecond / 1000 << ".jpg";
				string filename = ss.str();
				ss.clear();
				ss.str(gDir);
				ss.seekp(0, ios::end);
				imwrite(filename, *member->value);
			}
			ss.clear();
			ss.str("");
			i++;
		}*/
		/*Detector d(argv[1]);
		auto result = d.detect();
		auto resultpic = d.markOnSource(result);
		if (!fs::exists("./test"))
		{
			fs::create_directory("./test");
		}
		imwrite("./test/source.jpg", *d.getImage());
		imwrite("./test/marked.jpg", *resultpic);

		fs::path rootDir("./test/a");

		d.cut(result, rootDir);

		if (!fs::exists("./test/b"))
		{
			fs::create_directory("./test/b");
		}

		stringstream ss;
		vector<shared_ptr<Mat>> items;
		d.cut(result, items);
		for (size_t i = 0; i < items.size(); i++)
		{
			ss << "./test/b/" << i << ".jpg";
			imwrite(ss.str(), *items[i]);
			ss.clear();
			ss.str("");
		}*/
	}
	catch (const InvalidParamsException& e)
	{
		cout << e.msg << endl;
		return -1;
	}
}