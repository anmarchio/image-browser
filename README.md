# Image Browser

Image browser is a simple tool for browsing local storages containing image files.
The tool is written in cpp and uses SQLite for managing fil meta data.

## Config WxWidgets in Visual Studio 2017/2019 and Visual Studio Community 2019

This tutorial follows the step by step instructions given in:

https://www.youtube.com/watch?v=sRhoZcNpMb4&ab_channel=CahootsMalone

### Download and Install wxWigets

1. Download wxWidgets source code from: https://www.wxwidgets.org/downloads/
1. Extract the foloder to the local drive, e. g. `C:\wxWidgets-3.1.3\`
1. Now open `wx_vc16.sln` for *VS2019* or `wx_vc15.sln` for *VS2017* from `C:\wxWidgets-3.1.3\build\msw`
1. Click "Compile" in the project for times with all possible configurations `(Debug, Win32),(Release,Win32),(Debug,x64),(Release,x64)` by clicking right on project and select `Compile Project Now`
1. Open `System Properties > Advanced > Environment Variables` and add the *system variabel* (!) `WXWIN` with value `C:\wxWidgets-3.1.3`, click `OK`

### Using wxWidgets

1. Create new project by clicking `New > Create New Project`, use `Visual C++ > Empty Project`
1. Checkout the instruction in `<wxW path>/docs/msw/install.md`
1. Select `View > Other Windows > Property Manager`
1. Right click in property manager on `<project_name> Add Existing Property Sheet ...`
1. Select `C:\wxWidgets-3.1.3\wxwidgets.props`
1. It now appears in the Property Manager
1. Copy `minimal.cpp` from `C:\wxWidgets-3.1.3\samples\minimal` to `Source Files` in current project
1. Try to build the code (it will fail!)

### Configure Project Properties

1. Right click in Solution Manager on project, select `Properties`
1. Set `Configuration Properties > Advanced > Character Set` to `Use Unicode Character Set`
1. Set `Configuration Properties > Linker > System > Subsystem` to `Windows (/SUBSYSTEM:WINDOWS)`
1. Build solution (will now suceed!).

## How to install (v142) Build tools in Visual studio

In order to work with paramTuner in Visual Studio you will need VS 2019, the VS 2017 Build Tools and the v142 Build Tools. Problems can appear after installing the VS 2017 Build Tools with not up to date v142 Build tools. Then you have to retarget to the v142 Build Tools with the following steps:

1. Open Visual Studio-Installer
1. Select 'modify' for Visual Studio 2019
1. Navigate to 'Individual Components'
1. Search for 'MSVC v142-VS2019 C++-x64/x86-Buildtools (newest)' and select them

## Run Unittests

### Configurations in the Google Test Framework

Build the main project as `static library (.lib), according to: https://stackoverflow.com/questions/19886397/how-can-i-solve-the-error-lnk2019-unresolved-external-symbol-function

```
You can turn your first program into a static library by changing it in the projects properties. There should be an option under the General tab where the project is set to build to an executable (.exe). You can change this to .lib. The .lib file will build to the same place as the .exe.

In your UnitTest1 project, you can go to its properties, and under the Linker tab in the category Additional Library Directories, add the path to which MyProjectTest builds. Then, for Additional Dependencies under the Linker - Input tab, add the name of your static library, most likely MyProjectTest.lib.

That should allow your project to build. Note that by doing this, MyProjectTest will not be a standalone executable program unless you change its build properties as needed, which would be less than ideal.
```

#### In Project `ImageBrowserTests`:

* Make sure, the main project `ImageBrowserModel` is referenced in `ImageBrowserTests`
* In the solution explorer click right on `ImageBrowserTests` and select `Project Properties`
* Make sure to add `C:\wxWidgets-3.1.3\wxwidgets.props` as property (see section `Using wxWidgets`)
* Compile the test project (`Right click > Rebuild`)
* Run tests using `Test > Execute > All Tests`