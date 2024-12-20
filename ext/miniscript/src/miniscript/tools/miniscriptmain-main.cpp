#include <cstdlib>
#include <string>
#include <vector>

#include <miniscript/miniscript.h>
#include <miniscript/miniscript/Generator.h>
#include <miniscript/miniscript/Version.h>
#include <miniscript/utilities/Console.h>
#include <miniscript/utilities/StringTools.h>

using std::exit;
using std::string;
using std::to_string;
using std::vector;

using miniscript::miniscript::Generator;
using miniscript::miniscript::Version;
using miniscript::utilities::Console;
using miniscript::utilities::StringTools;

int main(int argc, char** argv)
{
	Console::printLine(string("miniscriptmain ") + Version::getVersion());
	Console::printLine(Version::getCopyright());
	Console::printLine();

	//
	if ((argc != 4 && argc != 5) || (argc == 5 && string(argv[4]) != "--use-library")) {
		Console::printLine("Usage: miniscriptmain script_filename script_class_name main_filename [--use-library]");
		return EXIT_FAILURE;
	}

	//
	auto scriptURI = string(argv[1]);
	auto className = string(argv[2]);
	auto mainURI = string(argv[3]);
	auto useLibrary = argc == 5?string(argv[4]) == "--use-library":false;

	//
	if (StringTools::endsWith(mainURI, "-main.cpp") == false) {
		Console::printLine("Error: main_filename must end with '-main.cpp'");
		return EXIT_FAILURE;
	}

	// generate
	Generator::generateMain(scriptURI, className, mainURI, useLibrary);

	//
	return EXIT_SUCCESS;
}
