#include <iostream>
#include <ctime>
#include <memory>
#include <io.h>
#include <fcntl.h>
#include "ProgramLoader.h"
#include "StackMachine.h"
#include "3rdparty\optionparser.h"
#include "GlobalOptions.h"
#include "gc.h"
#include "misc.h"
//#include <vld.h>

enum  optionIndex { UNKNOWN, HELP, VERBOSE, DEBUG, DISASSEMBLE };

const option::Descriptor usage[] =
{
	{UNKNOWN, 0,"" , ""    ,option::Arg::None, "USAGE: ai5r [options] file\n\n"
	"Options:" },
	{HELP,    0,"" , "help",option::Arg::None, "  --help  \tPrint usage and exit." },
	{VERBOSE,    0,"v", "verbose",option::Arg::None, "  --verbose, -v  \tWrite information about the current executing instruction and other info during execution." },
	{DEBUG,    0,"d", "debug",option::Arg::None, "  --debug, -d  \tMake runtime accept connections from debuggers." },
	{DISASSEMBLE,    0,"q", "disassemble",option::Arg::None, "  --disassamble, -q  \tDisassemble and write all instructions to stdout." },
	{0,0,0,0,0,0}
};

int main(int argc, char* argv[]) 
{

	

	argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
	option::Stats  stats(usage, argc, argv);
	option::Option options[128], buffer[129];
	option::Parser parse(usage, argc, argv, options, buffer);

	if (parse.error())
		return 1;

	if (options[HELP] || argc == 0) {
		option::printUsage(std::cout, usage);
		return 0;
	}

	if(parse.nonOptionsCount() > 0)
	{
		clock_t begin = clock();

		// Make console UTF-16 aware.
		_setmode(_fileno(stdout), _O_U8TEXT);
		try
		{
			bool isVerbose = options[VERBOSE] != 0;
			bool disassemble = options[DISASSEMBLE] != 0;
			#if _DEBUG
			isVerbose = true;
			#endif

			if(isVerbose)
				GlobalOptions::setVerbose();

			std::shared_ptr<StackMachine> machine = ProgramLoader::LoadFromFile(parse.nonOption(0));

			int returnCode = 0;

			if(disassemble)
			{
				machine->disassembleProgram();
			}
			else
			{
				machine->startMainThread();
				returnCode = machine->waitForTermination();
				// Terminate all remaining threads.
				machine->terminateAllThreads();
			}


			DebugOut(L"Status") << "Program ended with code: " << returnCode;


			GC::collect(true);

			GC::cleanup();

			clock_t end = clock();
			double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
			
			DebugOut(L"Status") << "Execution time: " << elapsed_secs;


			return returnCode;

		}
		catch(ProgramLoadError& error)
		{
			std::wcout << L"Error\t: Program load failed: "+error.getMessage() << std::endl;
			return -1;
		}
	}
	else
	{
		option::printUsage(std::cout, usage);
		return 0;
	}
}