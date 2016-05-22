#include "categorizer.h"

shared_ptr<GroupMember> makeGroupMember(const Mat& value, double frame, double millisecond)
{
	shared_ptr<GroupMember> gm = make_shared<GroupMember>();
	gm->frame = frame;
	gm->millisecond = millisecond;
	gm->value = value;
	return gm;
}

shared_ptr<Group> createGroup()
{
	shared_ptr<Group> g = make_shared<Group>();
	g->members = make_shared<vector<shared_ptr<GroupMember>>>();
	return g;
}

Categorizer::Categorizer()
{
	_groups = make_shared<vector<shared_ptr<Group>>>();
}

shared_ptr<vector<shared_ptr<Group>>> Categorizer::getGroups() const
{
	return _groups;
}

void Categorizer::addToGroup(const vector<Mat>& pictures, double frame, double millisecond)
{
	if (_groups->size() == 0)
	{
		for (auto& e : pictures)
		{
			auto g = createGroup();
			auto m = makeGroupMember(e, frame, millisecond);
			g->members->push_back(m);
			_groups->push_back(g);
		}
	}
	else
	{
		for (auto& e : pictures)
		{
			bool matched = false;
			size_t i = 0;
			auto newMember = makeGroupMember(e, frame, millisecond);

			for (; i < _groups->size(); i++)
			{
				auto m1 = _groups->at(i)->members->at(0)->value;
				auto src2 = e.clone();
				auto diff = getHistDiff(m1, src2);
				if (isSimilar(diff))
				{
					matched = true;
					break;
				}
			}

			if (matched)
			{
				_groups->at(i)->members->push_back(newMember);
			}
			else
			{
				auto g = createGroup();
				g->members->push_back(newMember);
				_groups->push_back(g);
			}
		}
	}
}

Scalar Categorizer::getGroupMSSIM(const Mat& pic, const Group& g)
{
	auto src = pic.clone();
	auto size = g.members->size();
	auto v0 = 0;
	auto v1 = 0;
	auto v2 = 0;
	for (auto& m : *g.members)
	{
		resize(src, src, m->value.size(), 0, 0, cv::INTER_LANCZOS4);
		auto res = getMSSIM(src, m->value);
		v0 += res[0];
		v1 += res[1];
		v2 += res[2];
	}
	return Scalar(v0 / size, v1 / size, v2 / size);
}

Scalar Categorizer::getMSSIM(const Mat& i1, const Mat& i2)
{
	const double C1 = 6.5025, C2 = 58.5225;
	int d = CV_32F;

	Mat I1, I2;
	i1.convertTo(I1, d);
	i2.convertTo(I2, d);

	Mat I2_2 = I2.mul(I2);
	Mat I1_2 = I1.mul(I1);
	Mat I1_I2 = I1.mul(I2);

	Mat mu1, mu2;
	GaussianBlur(I1, mu1, Size(11, 11), 1.5);
	GaussianBlur(I2, mu2, Size(11, 11), 1.5);

	Mat mu1_2 = mu1.mul(mu1);
	Mat mu2_2 = mu2.mul(mu2);
	Mat mu1_mu2 = mu1.mul(mu2);

	Mat sigma1_2, sigma2_2, sigma12;

	GaussianBlur(I1_2, sigma1_2, Size(11, 11), 1.5);
	sigma1_2 -= mu1_2;

	GaussianBlur(I2_2, sigma2_2, Size(11, 11), 1.5);
	sigma2_2 -= mu2_2;

	GaussianBlur(I1_I2, sigma12, Size(11, 11), 1.5);
	sigma12 -= mu1_mu2;

	Mat t1, t2, t3;

	t1 = 2 * mu1_mu2 + C1;
	t2 = 2 * sigma12 + C2;
	t3 = t1.mul(t2);

	t1 = mu1_2 + mu2_2 + C1;
	t2 = sigma1_2 + sigma2_2 + C2;
	t1 = t1.mul(t2);

	Mat ssim_map;
	divide(t3, t1, ssim_map); 

	Scalar mssim = mean(ssim_map);
	return mssim;
}

double Categorizer::getHistDiff(const Mat& i1, const Mat& i2)
{
	Mat src1, src2;
	cvtColor(i1, src1, COLOR_BGR2HSV);
	cvtColor(i2, src2, COLOR_BGR2HSV);

	int h_bins = 50; int s_bins = 60;
	int histSize[] = { h_bins, s_bins };

	float h_ranges[] = { 0, 180 };
	float s_ranges[] = { 0, 256 };

	const float* ranges[] = { h_ranges, s_ranges };

	int channels[] = { 0, 1 };

	MatND hist_src1;
	MatND hist_src2;

	calcHist(&src1, 1, channels, Mat(), hist_src1, 2, histSize, ranges, true, false);
	normalize(hist_src1, hist_src1, 0, 1, NORM_MINMAX, -1, Mat());

	calcHist(&src2, 1, channels, Mat(), hist_src2, 2, histSize, ranges, true, false);
	normalize(hist_src1, hist_src1, 0, 1, NORM_MINMAX, -1, Mat());

	return compareHist(hist_src1, hist_src2, HISTCMP_INTERSECT);
}

int Categorizer::getHashDiff(const Mat& src1, const Mat& src2)
{
	Mat resize1, resize2;
	resize(src1, resize1, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);
	resize(src2, resize2, cv::Size(8, 8), 0, 0, cv::INTER_CUBIC);

	cvtColor(resize1, resize1, CV_BGR2GRAY);
	cvtColor(resize2, resize2, CV_BGR2GRAY);

	int iAvg1 = 0, iAvg2 = 0;
	int arr1[64], arr2[64];

	for (int i = 0; i < 8; i++)
	{
		uchar* data1 = resize1.ptr<uchar>(i);
		uchar* data2 = resize1.ptr<uchar>(i);

		int tmp = i * 8;

		for (int j = 0; j < 8; j++)
		{
			int tmp1 = tmp + j;

			arr1[tmp1] = data1[j] / 4 * 4;
			arr2[tmp1] = data2[j] / 4 * 4;

			iAvg1 += arr1[tmp1];
			iAvg2 += arr2[tmp1];
		}
	}

	iAvg1 /= 64;
	iAvg2 /= 64;

	for (int i = 0; i < 64; i++)
	{
		arr1[i] = (arr1[i] >= iAvg1) ? 1 : 0;
		arr2[i] = (arr2[i] >= iAvg2) ? 1 : 0;
	}

	int iDiffNum = 0;

	for (int i = 0; i < 64; i++)
		if (arr1[i] != arr2[i])
			++iDiffNum;
	return iDiffNum;

}

bool Categorizer::isSimilar(Scalar& mssim)
{
	int i = mssim[0] >= 0.5;
	int j = mssim[1] >= 0.5;
	int k = mssim[2] >= 0.5;
	return (i + j + k) >= 2;
}

bool Categorizer::isSimilar(double diff)
{
	return diff >= 11;
}

bool Categorizer::isSimilar(int hash)
{
	return hash <= 5;
}