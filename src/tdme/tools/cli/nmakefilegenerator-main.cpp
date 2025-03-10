#include <string>
#include <vector>

#include <tdme/tdme.h>
#include <tdme/application/Application.h>
#include <tdme/engine/Version.h>
#include <tdme/os/filesystem/FileNameFilter.h>
#include <tdme/os/filesystem/FileSystem.h>
#include <tdme/os/filesystem/FileSystemInterface.h>
#include <tdme/utilities/Console.h>
#include <tdme/utilities/Exception.h>
#include <tdme/utilities/StringTools.h>

using std::string;
using std::to_string;
using std::vector;

using tdme::application::Application;
using tdme::engine::Version;
using tdme::os::filesystem::FileNameFilter;
using tdme::os::filesystem::FileSystem;
using tdme::os::filesystem::FileSystemInterface;
using tdme::utilities::Console;
using tdme::utilities::Exception;
using tdme::utilities::StringTools;

void scanPath(const string& path, vector<string>& sourceFiles, vector<string>& mainSourceFiles) {
	class SourceFilesFilter: public FileNameFilter {
		public:
			virtual ~SourceFilesFilter() {}

			bool accept(const string& pathName, const string& fileName) override {
				if (fileName == ".") return false;
				if (fileName == "..") return false;
				if (FileSystem::getInstance()->isPath(pathName + "/" + fileName) == true) return true;
				// skip on CPP files that gets #include ed
				if (StringTools::endsWith(StringTools::toLowerCase(fileName), ".incl.cpp") == true) return false;
				if (StringTools::endsWith(StringTools::toLowerCase(fileName), ".include.cpp") == true) return false;
				// CPP hit, yes
				if (StringTools::endsWith(StringTools::toLowerCase(fileName), ".cpp") == true) return true;
				return false;
			}
	};

	SourceFilesFilter sourceFilesFilter;
	vector<string> files;

	FileSystem::getInstance()->list(path, files, &sourceFilesFilter);

	for (const auto& fileName: files) {
		if (StringTools::endsWith(fileName, "-main.cpp") == true) {
			mainSourceFiles.push_back(path + "/" + fileName);
		} else
		if (StringTools::endsWith(fileName, ".cpp") == true) {
			sourceFiles.push_back(path + "/" + fileName);
		} else {
			scanPath(path + "/" + fileName, sourceFiles, mainSourceFiles);
		}
	}
}

int main(int argc, char** argv)
{
	Console::printLine(string("nmakefilegenerator ") + Version::getVersion());
	Console::printLine(Version::getCopyright());
	Console::printLine();

	if (argc != 2) {
		Console::printLine("Usage: nmakefilegenerator path_to_source");
		Application::exit(Application::EXITCODE_FAILURE);
	}

	auto pathToSource = string(argv[1]);

	try {
		Console::printLine("Scanning source files");
		vector<string> sourceFiles;
		vector<string> mainSourceFiles;
		scanPath(pathToSource, sourceFiles, mainSourceFiles);

		//
		string sourceFilesVariable = "\\\n";
		for (const auto& file: sourceFiles) sourceFilesVariable+= "\t" + file + "\\\n";
		sourceFilesVariable+= "\n";

		//
		string mainTargets;
		for (const auto& file: mainSourceFiles) {
			if (mainTargets.empty() == false) mainTargets+= " ";
			mainTargets+= StringTools::substring(file, file.rfind('/') + 1, file.find("-main.cpp"));
		}

		Console::printLine("Generating Makefile");

		//
		auto executablePath = StringTools::replace(argv[0], "\\", "/");
		auto tdme2Path = StringTools::substring(executablePath, 0, StringTools::toLowerCase(executablePath).rfind("/tdme2/") + string("/tdme2/").length());

		//
		auto makefileSource = FileSystem::getInstance()->getContentAsString(tdme2Path + "/resources/engine/templates/makefiles", "Makefile.nmake");
		auto makefileMainSourceTemplate = FileSystem::getInstance()->getContentAsString(tdme2Path + "/resources/engine/templates/makefiles", "Makefile.nmake.main");
		makefileSource = StringTools::replace(makefileSource, "{$source-files}", sourceFilesVariable);
		makefileSource = StringTools::replace(makefileSource, "{$main-targets}", mainTargets);
		makefileSource+= "\n";

		//
		for (const auto& file: mainSourceFiles) {
			auto makefileMainSource = makefileMainSourceTemplate;
			auto mainTarget = StringTools::substring(file, file.rfind('/') + 1, file.find("-main.cpp"));
			auto mainTargetSource = file;
			auto mainTargetExecutable = mainTarget + ".exe";
			makefileMainSource = StringTools::replace(makefileMainSource, "{$main-target}", mainTarget);
			makefileMainSource = StringTools::replace(makefileMainSource, "{$main-target-source}", mainTargetSource);
			makefileMainSource = StringTools::replace(makefileMainSource, "{$main-target-executable}", mainTargetExecutable);
			makefileSource+= makefileMainSource + "\n";
		}

		//
		FileSystem::getInstance()->setContentFromString(".", "Makefile.nmake", makefileSource);
	} catch (Exception& exception) {
		Console::printLine("An error occurred: " + string(exception.what()));
	}

	//
	Application::exit(Application::EXITCODE_SUCCESS);
}
