#include "categorizer.h"

shared_ptr<GroupMember> makeGroupMember(const shared_ptr<Mat>& value, double frame, double millionsecond)
{
	shared_ptr<GroupMember> gm = make_shared<GroupMember>();
	gm->frame = frame;
	gm->millisecond = millionsecond;
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

void Categorizer::addToGroup(const vector<shared_ptr<Mat>>& pictures, double frame, double millionsecond)
{
	if (_groups->size() == 0)
	{
		for (auto& e : pictures)
		{
			auto g = createGroup();
			auto m = makeGroupMember(e, frame, millionsecond);
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
			auto newMember = makeGroupMember(e, frame, millionsecond);

			for (; i < _groups->size(); i++)
			{
				auto m1 = _groups->at(i)->members->at(0)->value;
				auto src2 = e->clone();
				resize(src2, src2, m1->size(), 0, 0, cv::INTER_LANCZOS4);
				auto mssim = getMSSIM(*m1, src2);
				if (isSimilar(mssim))
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

bool Categorizer::isSimilar(Scalar& mssim)
{
	int i = mssim[0] >= 0.5;
	int j = mssim[1] >= 0.5;
	int k = mssim[2] >= 0.5;
	return (i + j + k) >= 2;
}