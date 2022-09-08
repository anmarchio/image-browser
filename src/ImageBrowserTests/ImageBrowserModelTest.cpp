#include "pch.h"

#include <fstream>
#include "../ImageBrowserModel/Main.h"
#include "../ImageBrowserModel/MainFrame.h"


class GuiTest : public testing::Test
{
protected:
	Main *app;
	std::string ASSETS_DIR = "..\\src\\ImageBrowserTests\\Assets\\";

	virtual void SetUp()
	{
		char appname[] = "ImageBrowserModel.lib";
		int argc = 1;
		char *argv[1] = { appname };
		app = new Main();
		wxApp::SetInstance(app);
		wxEntryStart(argc, argv);
		app->OnInit();
	}
	virtual void TearDown()
	{
		//wxTheApp->OnRun();
		app->OnExit();
		wxEntryCleanup();
	}
};

/*
TEST_F(GuiTest, TraverseFileTreeTest)
{
	const std::string IMAGE_PATH_LIST = { 
		"File;Type;Width;Height;ColorDepth\nAssets\arrow_right.jpg;image;140;87;binary\nAssets\hexagram.jpg;image;92;86;binary\nAssets\scenery.jpg;image;92;86;32 bit\nAssets\star.jpg;image;binary;92;86;binary"
	};

	app->frame->selectedDirectory->Clear();

	std::experimental::filesystem::path target = std::experimental::filesystem::path("test.txt");
	app->frame->TraverseDirTree(
		std::experimental::filesystem::path("Assets"),
		std::experimental::filesystem::path(target)
	);

	EXPECT_TRUE(exists(target));

	std::ifstream samplefile("test.txt");
	std::string line;
	std::string content = "";
	if (samplefile.is_open())
	{
		while (getline(samplefile, line))
		{
			content.append(line + "\n");
		}
		samplefile.close();
	}
	std::remove("test.txt");

	EXPECT_EQ(content, IMAGE_PATH_LIST);
}
*/

TEST_F(GuiTest, CheckImageSizeAndDepthTest)
{
	const std::string THIRTY_TWO_BIT = { "92;86;8 bit" };
	const std::string BINARY = { "140;87;binary" };

	//\\Assets\\arrow_right.jpg");
	//\\Assets\\hexagram.jpg");
	//\\Assets\\scenery.jpg");
	//\\Assets\\star.jpg");

	std::string colorDepth = app->frame->checkImageSizeAndDepth(std::experimental::filesystem::path(ASSETS_DIR + "scenery.jpg"));	
	EXPECT_EQ(colorDepth, THIRTY_TWO_BIT);
	
	std::string binaryDepth = app->frame->checkImageSizeAndDepth(std::experimental::filesystem::path(ASSETS_DIR + "arrow_right.jpg"));
	EXPECT_EQ(binaryDepth, BINARY);
}

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}