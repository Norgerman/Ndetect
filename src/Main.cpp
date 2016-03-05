#include "stdafx.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv) 
{
	cout << "under construction!" << endl;

	cout << "try to create a mat to test whether opencv link correctly!" << endl;

	shared_ptr<Mat> i = make_shared<Mat>();

	cout << "mat type: " << i->type() << endl;
}