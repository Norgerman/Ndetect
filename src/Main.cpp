#include <iostream>
#include <string>
#include <cstring>
#include <boost/filesystem.hpp>
#include <iomanip>
#include "invalid_params_exception.h"
#include "detector.h"
#include "video_loader.h"
#include "categorizer.h"
#include "group_writer.h"
#include "face_modifier.h"

using namespace std;
using namespace cv;
namespace fs = boost::filesystem;

typedef struct {
	string fileName;
	string outputDir;
	string groupPrefix;
	bool faceOnly;
} CommandLineArg;

typedef enum
{
	START,
	INFILE,
	FACEONLY,
	OUTDIR,
	PREFIX,
	UNKNOWN,
	END
} CommandLineState;

bool parseCommandLine(int argc, char** argv, CommandLineArg& arg);

string readOutDir(char* dir);

void displayCommand();

CommandLineState next(const char* arg, int& i);

int main(int argc, char** argv)
{
	if (argc < 7)
	{
		displayCommand();
		return -1;
	}
	auto arg = CommandLineArg();
	if (!parseCommandLine(argc, argv, arg))
	{
		displayCommand();
		return -1;
	}

	try
	{
		auto loader = VideoLoader(arg.fileName);
		double frame = 0;
		double millisecond = 0;
		auto categorizer = Categorizer();
		auto resultSet = vector<shared_ptr<Mat>>();
		stringstream ss;
		string srcDir;
		Mat res;
		ss << arg.outputDir << "/source";
		srcDir = ss.str();
		if (!fs::exists(srcDir)) 
		{
			fs::create_directories(srcDir);
		}

		while (loader.next(res, frame, millisecond))
		{
			auto d = Detector(res);
			auto result = d.detect();
			d.cut(result, resultSet);
			categorizer.addToGroup(resultSet, frame, millisecond);
			cout << "frame: " << frame << " millisecond: " << millisecond << endl;
			ss << "/"  << fixed << setprecision(3) << millisecond / 1000 << ".jpg";
			imwrite(ss.str(), res);
			resultSet.clear();
			ss.clear();
			ss.str(srcDir);
			ss.seekp(0, ios::end);
		}
		cout << "detect completed" << endl;
		auto groups = categorizer.getGroups();
		auto writer = GroupWriter();
		if (arg.faceOnly)
		{
			auto face = FaceModifier();
			writer.writeGroup(*groups, arg.outputDir, arg.groupPrefix, face);
		}
		else
		{
			writer.writeGroup(*groups, arg.outputDir, arg.groupPrefix);
		}
	}
	catch (const InvalidParamsException& e)
	{
		cout << e.msg << endl;
		return -1;
	}
}

string readOutDir(char* dir)
{
	string s(dir);
	if (s[s.length() - 1] == '/')
		s.pop_back();
	return s;
}

CommandLineState next(const char* arg, int& i)
{
	if (arg == nullptr)
		return END;
	if (strcmp(arg, "-i") == 0)
	{
		i++;
		return INFILE;
	}
	else if (strcmp(arg, "-o") == 0)
	{
		i++;
		return OUTDIR;
	}
	else if (strcmp(arg, "-g") == 0)
	{
		i++;
		return PREFIX;
	}
	else if (strcmp(arg, "-f") == 0)
	{
		return FACEONLY;
	}
	else
	{
		i++;
		return UNKNOWN;
	}
}

bool parseCommandLine(int argc, char** argv, CommandLineArg& arg)
{
	int i = 0;
	CommandLineState state = START;
	arg.faceOnly = false;
	arg.groupPrefix = "g";
	while (i < argc)
	{
		switch (state)
		{
		case START: break;
		case INFILE: arg.fileName = argv[i]; break;
		case OUTDIR: arg.outputDir = readOutDir(argv[i]); break;
		case FACEONLY: arg.faceOnly = true; break;
		case PREFIX: arg.groupPrefix = argv[i]; break;
		default:
			return false;
		}
		i++;
		state = ::next(argv[i], i);
	}
	if (arg.fileName.length() == 0 || arg.outputDir.length() == 0)
		return false;
	return true;
}

void displayCommand()
{
	cout << "usage: ";
	cout << "ndetect -i filename -o dir [-g prefix] [-f]" << endl;
	cout << "-i filename : special video file to process" << endl;
	cout << "-o dir : the directory to output the result" << endl;
	cout << "-g prefix : the prefix of the group directory name such as dir/g01 default g" << endl;
	cout << "-f : if set only output face" << endl;
}