#include "pch.h"

#include <fstream>
#include "../ImageBrowserModel/Main.h"
#include "../ImageBrowserModel/MainFrame.h"


class GuiTest : public testing::Test
{
protected:
	Main *app;
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

TEST_F(GuiTest, TraverseFileTreeTest)
{
	const std::string IMAGE_PATH_LIST = { "Assets\\arrow_right.jpg\nAssets\\hexagram.jpg\nAssets\\star.jpg\n" };

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

int main(int argc, char *argv[])
{
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}