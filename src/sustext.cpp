#include "globals.h"
#include "sustext.h"
#include "terminal.h"
#include "editor.h"
#include "prototypes.h"
#include "flaghandler.h"
#include "filehandler.h"
#include "Debug/logger.h"

void Sustext::Initialize(int argc, char** argv)
{
	LOG_INFO << "Initializing Sustext" << std::endl;
	// Initialize editor and terminal
	Editor::Initialize(argc, argv);
	Terminal::EnableRawMode();
    FlagHandler::Initialize(argc, argv);
    
    if (FlagHandler::Enabled(FlagHandler::Identifier::fileIn)) {
        if (!FileHandler::OpenFile(eConfig.filepath))
            Terminal::die((int)Severity::medium, "fopen");     
        
        // Check for other flags and handle them
    }    

    Editor::SetStatusMessage("HELP: Ctrl-s = save | Ctrl-q = quit | Ctrl-f = find", FindCallBack);

	LOG_SUCCESS << "Initialized Sustext" << std::endl;
}
