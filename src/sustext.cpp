#include "globals.h"
#include "sustext.h"
#include "terminal.h"
#include "editor.h"
#include "prototypes.h"
#include "flaghandler.h"
#include "filehandler.h"

#include <stdint.h>

uint64_t Flags;
Editor::ConfigData eConfig;
Terminal::ConfigData tConfig;

int main(int argc, char** argv)
{
	Sustext::Initialize(argc, argv);

	while (true) {
		Editor::RefreshScreen();
		Editor::ProcessKeypress();
	}

	return 0;
}

void Sustext::Initialize(int argc, char** argv)
{
	// Initialize editor and terminal
	Editor::Initialize(argc, argv);
	Terminal::enableRawMode();
	FlagHandler::Initialize(argc, argv);

	if (argc >= 2) {
		if (FlagHandler::Enabled(FILEIN)) {
			if (!FileHandler::OpenFile(eConfig.filepath)) {
				Terminal::die("fopen");
			}
		}
		// Check for other terminal flags
	}

	Editor::SetStatusMessage("HELP: Ctrl-s = save | Ctrl-q = quit | Ctrl-f = find", FindCallBack);
}