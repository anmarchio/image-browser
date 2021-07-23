#include "stdafx.h"
#include "pch.h"

#include <fstream>
#include <experimental/filesystem>
#include "CppUnitTest.h"
#include "../ImageBrowserModel/Main.h"
#include "../ImageBrowserModel/MainFrame.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TraverseFileTreeTest
{		
	TEST_CLASS(TraverseFileTreeTest)
	{
	public:

		const std::string IMAGE_PATH_LIST = {"arrow_right.jpg\nhexagram.jpg\nstar.jpg"};
		
		TEST_METHOD(TraverseFileTree)
		{
			MainFrame *frame = new MainFrame("Tiny Image Browser");
			frame->selectedDirectory->Clear();

			std::experimental::filesystem::path target = std::experimental::filesystem::path("test.txt");
			frame->TraverseDirTree(
				std::experimental::filesystem::path("Assets"), 
				std::experimental::filesystem::path(target)
				);

			Assert::IsTrue(exists(target));

			std::ifstream samplefile("example.txt");
			std::string line;
			std::string content = "";
			if (samplefile.is_open())
			{
				while (getline(samplefile, line))
				{
					content.append(line);
				}
				samplefile.close();
			}
			Assert::AreEqual(content, IMAGE_PATH_LIST);
		}

	};
}