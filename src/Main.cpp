#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include <direct.h>
#include <experimental/filesystem>

using std::cout; 
using std::cin;
using std::endl; 
using std::string;
using std::experimental::filesystem::directory_iterator;


const string DEFAULT_FILE_NAME = "image_index.html";

const string IMAGE_FILE_FORMATS[7] = { ".bmp", ".gif", ".jpg", ".jpeg", ".png", ".tif", ".tiff" };

int traverseDirTree(std::experimental::filesystem::path source, string target)
{
	for (const auto & file : directory_iterator(source))
	{
		if (std::experimental::filesystem::is_directory(file))
		{
			traverseDirTree(file.path(), target);
		}
		else
		{
			cout << file.path() << endl;
			std::ofstream imageFileList(target + "\\" + DEFAULT_FILE_NAME);
			if (imageFileList.is_open())
			{
				imageFileList << file.path() << "\n";
				imageFileList.close();
			}
			else cout << "Unable to open file";
		}
	}

	return EXIT_SUCCESS;
}


int main() 
{
	string source = ".";
	string target = ".";

	// traverseDirTree(source, target);

	return traverseDirTree(source, target);
}