#include "sustext.hpp"

Sustext::Sustext(int _argc, char** _argv){
	argc = _argc;
	argv = _argv;
	flags = SusFlags(argc, argv);

	/*
	Editor Initialization
	*/
}

Sustext::~Sustext(){

}



/*
int main(int argc, char** argv){
	SusFlags sFlags = SusFlags(argc, argv);
	Terminal::enableRawMode();
	Editor::Init();

	//Check if parameters
	if(argc >= 2){
		sFlags.SetFlags();

		//If the file does not exist the terminal just dies and closes with
		//the exit code 'fopen'
		if(sFlags.Enabled(FILEIN))
			if(!Editor::OpenFile(sFlags.argout.filepath)) Terminal::die("fopen");
	}

	Editor::SetStatusMessage("HELP: Ctrl-s = save | Ctrl-q = quit");

	while(1){
		Editor::RefreshScreen();
		Editor::ProcessKeypress();
	}
	return 0;
}
*/