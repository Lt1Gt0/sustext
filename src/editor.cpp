#include "headers/editor.hpp"

/*---- INITIALIZATION ----*/
void Editor::Init(){
	/* Set default values for the global editorConfig struct */
	E.cx, E.cy, E.rx = 0;
	E.rowOff, E.colOff, E.numrows = 0; // Scroll to the top left of the screen by default 
	E.numrows = 0;
	E.row = new erow{0,NULL};
	if(Terminal::getWindowSize(&E.screenRows, &E.screenCols) == -1) Terminal::die("getWindowSize");
}

/*---- ROW OPERATIONS ----*/
int Editor::RowCxToRx(erow* row, int cx){
	int rx = 0;
	for(int i = 0; i < cx; i++){
		// Check to see how many columns to the left of the next tab the cursor is
		if(row->chars[i] == '\t')
			rx += (SUSTEXT_TAB_STOP - 1) - (rx % SUSTEXT_TAB_STOP);
		rx++;
	}

	return rx;
}

void Editor::UpdateRow(erow* row){
	int tabs = 0;
	for(int j = 0; j < row->size; j++){
		if(row->chars[j] == '\t') tabs++;
	}

	free(row->render);
	row->render = (char*)malloc(row->size + tabs*(SUSTEXT_TAB_STOP - 1) + 1);

	int i = 0;
	for(int j = 0; j < row->size; j++){
		if(row->chars[j] == '\t'){
			row->render[i++] = ' ';
			while(i % SUSTEXT_TAB_STOP != 0) row->render[i++] = ' ';
		} else {
			row->render[i++] = row->chars[j];
		}
	}
	row->render[i] = '\0';
	row->rsize = i;
}

void Editor::AppendRow(char* s, size_t len){
	E.row = (erow*)realloc(E.row, sizeof(erow) * (E.numrows + 1));

	int at = E.numrows;
	E.row[at].size = len;
	E.row[at].chars = (char*)malloc(len+1);

	memcpy(E.row[at].chars, s, len);
	E.row[at].chars[len] = '\0';

	// Initialize render for buffer
	E.row[at].rsize = 0;
	E.row[at].render = NULL;
	UpdateRow(&E.row[at]);

	E.numrows++;
}

/*---- INPUT ----*/
void Editor::MoveCursor(int key){
	//Prevent cursor from going past the size of the screen not the file
	erow* row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];

	switch(key){
		case ARROW_LEFT:
			if(E.cx != 0) E.cx--;
			else if(E.cy > 0){
				E.cy--;
				E.cx = E.row[E.cy].size;
			}
			break;
		case ARROW_RIGHT:
			if(row && E.cx < row->size) E.cx++;
			else if(row && E.cx == row->size){
				E.cy++;
				E.cx = 0;
			}
			break;
		case ARROW_UP:
			if(E.cy != 0) E.cy--;
			break;
		case ARROW_DOWN:
			if(E.cy < E.numrows) E.cy++;
			break;
	}

	// Cursor snap to end of line
	row = (E.cy >= E.numrows) ? NULL : &E.row[E.cy];
	int rowlen = row ? row->size : 0;
	if(E.cx > rowlen){ E.cx = rowlen; }
}

/* TODO

This method will change with different opening
modes (sus, read, write, etc...)
*/
int Editor::OpenFile(char* filename){
	//Open a file in read mode
	FILE* fp = fopen(filename, "r"); 		// This line will eventually change
	if(!fp) Terminal::die("fopen");

	char* line = NULL;
	size_t linecap = 0;
	ssize_t linelen;

	//Get the line at index 0 of the file
	linelen = getline(&line, &linecap, fp);

	//Get the length of the line from the file
	while((linelen = getline(&line, &linecap, fp)) != -1){
		//stop if the escape sequence for new line or return carriage is next
		while(linelen > 0 && (line[linelen - 1] == '\n' ||
		line[linelen - 1] == '\r'))
			linelen--;
		AppendRow(line, linelen);
	}
	//Deallocate memory from line and close file connection
	free(line);
	fclose(fp);

	return 1;
}

void Editor::ProcessKeypress(){
	int c = Terminal::editorReadKey();

	// Bane of my existance
	switch(c){
		case CTRL_KEY('q'):

			//[2J will erase all of the diaply without moving the cursor position
			write(STDOUT_FILENO, "\x1b[2J", 4);

			//Return cursor to home
			write(STDOUT_FILENO, "\x1b[H", 3);
			exit(0);
			break;

		case HOME_KEY:
			E.cx = 0;
			break;
		case END_KEY:	
			if(E.cx < E.numrows)
				E.cx = E.row[E.cy].size;
			break;

	    case PAGE_UP:
		case PAGE_DOWN:
			if(c == PAGE_UP){
				E.cy = E.rowOff;
			} else if (c == PAGE_DOWN){
				E.cy = E.rowOff + E.screenRows - 1;
				if(E.cy > E.numrows) E.cy = E.numrows;
			}
			break;

		case ARROW_UP:
		case ARROW_DOWN:
		case ARROW_LEFT:
		case ARROW_RIGHT:
			MoveCursor(c);
			break;
	}
}

/*---- OUTPUT ----*/
void Editor::Scroll(){
	E.rx = 0;
	if(E.cy < E.numrows){
		E.rx = RowCxToRx(&E.row[E.cy], E.cx);
	}
	if(E.cy < E.rowOff){
		E.rowOff = E.cy;
	}
	if(E.cy >= E.rowOff + E.screenRows){
		E.rowOff = E.cy - E.screenRows + 1;
	}
	if(E.rx < E.colOff){
		E.colOff = E.rx;
	}
	if(E.rx >= E.colOff + E.screenCols){
		E.colOff = E.rx - E.screenCols + 1;

	}
}

void Editor::DrawRows(struct AppendBuffer::abuf *ab) {
	for(int y = 0; y < E.screenRows; y++) {
		int filerow = y + E.rowOff;
		if(filerow >= E.numrows){
			// Prepare the append buffer
			if (E.numrows == 0 && y == E.screenRows / 3) {
				// The welcome text will only show if the editor is opened as a standalone
				// program with no inputs, on file open there is no welcome
				char welcome[80];
				int welcomelen = snprintf(welcome, sizeof(welcome),
				  "sustext editor -- version %s", SUSTEXT_VERSION);
				if (welcomelen > E.screenCols) welcomelen = E.screenCols;
				int padding = (E.screenCols - welcomelen) / 2;

				// Padding will always be present on blank lines
				if (padding) {
					abAppend(ab, "~", 1);
					padding--;
			  	}
			  while (padding--) abAppend(ab, " ", 1);
			  abAppend(ab, welcome, welcomelen);
			} else { abAppend(ab, "~", 1); }
		} else {
			int len = E.row[filerow].rsize - E.colOff;
			// Prevent the length for being a negative number
			if(len < 0) len = 0;
			if(len > E.screenCols) len = E.screenCols;
			AppendBuffer::abAppend(ab, &E.row[filerow].render[E.colOff], len);
		}

		// Since everything for the row is appended to the buffer everything 
		// after the cursor which effecively produces a clean row
		abAppend(ab, "\x1b[K", 3);

		// Account for the last row
		if (y < E.screenRows - 1) { abAppend(ab, "\r\n", 2); }
	}
}

void Editor::RefreshScreen(){
	Scroll();
	struct AppendBuffer::abuf ab = ABUF_INIT;

	// Use the ?25l esacape sequence to hide to cursor on refresh
	abAppend(&ab, "\x1b[?25l", 6);
	abAppend(&ab, "\x1b[H", 3);

	DrawRows(&ab);

	char buf[32];
	snprintf(buf, sizeof(buf), "\x1b[%d;%dH", (E.cy - E.rowOff) + 1, (E.rx - E.colOff) + 1);
	abAppend(&ab, buf, strlen(buf));

	abAppend(&ab, "\x1b[?25h", 6);

	write(STDOUT_FILENO, ab.b, ab.len);
	abFree(&ab);
}