#include "group_writer.h"

GroupWriter::GroupWriter()
{
	_defaultModifier = make_shared<DefaultOutputModifier>();
}

void GroupWriter::writeGroup(vector<shared_ptr<Group>>& groups, const string& outputDir, const string& groupNamePrefix)
{
	writeGroup(groups, outputDir, groupNamePrefix, *_defaultModifier);
}

void GroupWriter::writeGroup(vector<shared_ptr<Group>>& groups, const string& outputDir, const string& groupNamePrefix, const OutputModifier& modifier)
{
	stringstream ss;
	auto i = 0;
	fs::path outdir(outputDir);
	if (!fs::exists(outputDir))
		fs::create_directory(outputDir);
	for (auto& g : groups)
	{
		ss << outputDir << "/" << groupNamePrefix << i;
		string gDir = ss.str();
		
		if (!fs::exists(gDir))
		{
			fs::create_directory(gDir);
		}

		for (auto& member : *g->members)
		{
			Mat pic = member->value.clone();
			if (modifier.modifier(pic))
			{
				ss << "/" << fixed << setprecision(3) << member->millisecond / 1000 << ".jpg";
				string filename = ss.str();
				ss.clear();
				ss.str(gDir);
				ss.seekp(0, ios::end);
				if (!fs::exists(filename))
					imwrite(filename, pic);
			}
		}
		ss.clear();
		ss.str("");
		i++;
	}
}