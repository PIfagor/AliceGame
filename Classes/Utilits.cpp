#include "Utilits.h"


USING_NS_CC;

void writeTextToFile(const std::string & content, const std::string & fullPath)
{
	FILE *fp = fopen(fullPath.c_str(), "w+t");
	if (!fp) {
		CCLOG("Error opening file: %s", fullPath.c_str());
		return;
	}
	fputs(content.c_str(), fp);
	fclose(fp);
}