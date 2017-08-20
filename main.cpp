/*
	XML File:
	<?xml version="1.0" encoding="UTF-8" ?>
	<result>
		<listfile name="abc.txt" src="src" dst="dst"></listfile>
		<listfile name="mmm.txt" src="src" dst="dst"></listfile>
	</result>

	Использовал библиотеку boost, сборка mcvs
*/

#include <boost/filesystem.hpp>
#include <iostream>
#include <string>
#include "tinyxml2.h"

using namespace tinyxml2;
using namespace std;
namespace fs = boost::filesystem;

int main()
{
	char overwrite;
	string fullFileNameSrc, fullFileNameDst;
	XMLDocument doc;
	doc.LoadFile("config.xml");

	XMLElement *filesList = doc.FirstChildElement("result");
	try
	{
		for (XMLElement* child = filesList->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
		{
			overwrite = 'n';
			fullFileNameSrc = string(child->Attribute("src")) + "\\" + string(child->Attribute("name"));
			fullFileNameDst = string(child->Attribute("dst")) + "\\" + string(child->Attribute("name"));

			if (!fs::exists(child->Attribute("src")) || !fs::is_directory(child->Attribute("src")) ||
				!fs::exists(fullFileNameSrc) || !fs::is_regular_file(fullFileNameSrc))
			{
				cout << "\"" << child->Attribute("src") << "\" does not exist or is not a directory! Or \"" << fullFileNameSrc
					<< "\" does not exist or is not a file!" << endl;
				continue;
			}

			if (!fs::exists(child->Attribute("dst")))
				fs::create_directory(child->Attribute("dst"));

			if (fs::exists(fullFileNameDst))
			{
				cout << fullFileNameDst << " is already exist. Type \"y\" to overwrite it: ";
				cin >> overwrite;
				if (overwrite != 'y')
					continue;
			}

			boost::filesystem::copy_file(fullFileNameSrc, fullFileNameDst);
		}
	}
	catch (exception e)
	{
		cout << "An exception occurred: " << e.what();
	}

	return 0;
}