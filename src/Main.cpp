#include "stdafx.h"
#include "detector.h"
#include <iostream>

using namespace std;
using namespace cv;

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
		imwrite("./source.jpg", *d.getImage());
		imwrite("./marked.jpg", *resultpic);
	}
	catch (const InvalidParamsException& e)
	{
		cout << e.what() << endl;
		return -1;
	}
}