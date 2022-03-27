#ifndef __IO_HPP
#define __IO_HPP

#include "defines.hpp"
#include "data.hpp"
#include "appendbuffer.hpp"
#include "terminal.hpp"

namespace Editor{
    /*---- INITIALIZATION ----*/
    
    /**
     * Initialize the editor functions 
     */
    void Init();

    /*---- ROW OPERATIONS ----*/

    /**
     * Convert the cursor x position given as an input
     * int a render index 
     * 
     * @param row 
     * @param cx 
     * @return int (rx)
     */
    int RowCxToRx(erow* row, int cx);
    
    /**
     * Display a prompt to the user on the bottom portion
     * of the status bar 
     * 
     * @param prompt 
     * @return char* 
     */
    char* Prompt(char* prompt);

    /**
     * fill contents of render stream 
     * 
     * @param erow 
     */
    void UpdateRow(erow* row);

    /**
     * Append a new row onto the editor screen 
     * 
     * @param s (char*) characters in the row
     * @param len (size_t) size of the row
     */
    void InsertRow(int at, char* s, size_t len);

    /**
     * Update the current row with an inputted char value 
     * 
     * @param row (erow*)
     * @param at position of row to insert char into
     * @param c char to input to row
     */
    void RowInsertChar(erow* row, int at, int c);

    /**
     * Similar to RowInsertChar but instead there is only the
     * input of the char to add to the current row the cursor
     * is on 
     * 
     * @param c char to insert
     */
    void InsertChar(int c);
    
    /**
     * Depending on where the cursor is when the enter key
     * is pressed, handle created a new line preserving
     * the row above or splitting it to a new line depending
     * where the cursor was placed 
     */
    void InsertNewLine();

    /**
     * Similar to RowInsertChar but instead you can feed a 
     * string value to be inserted into the row as a char* 
     * 
     * @param row
     * @param str 
     * @param len  
     */
    void RowAppendString(erow* row, char* str, size_t len);
    

    /**
     * Delete a row from the file while preserving the row
     * contents above and below 
     * 
     * @param at row position to remove
     */
    void DeleteRow(int at);
    
    /**
     * Remove single character from row 
     * 
     * @param row 
     * @param at where in the row to remove char
     */
    void RowDeleteChar(erow* row, int at);
    
    /**
     * remove the char from the end of a row
     * (handy for backspace) 
     */
    void DeleteChar();
    

    /**
     * clear row from memory 
     * 
     * @param row 
     */
    void FreeRow(erow* row);



    /*---- INPUTS ----*/

    /**
     * Take key input from user and move cursor accordingly 
     * @param key 
     */
    void MoveCursor(int key);

    /**
     * Take in key input from terminal and determine
     * what function to do
     */
    void ProcessKeypress();

    /*---- FILE I/O ----*/

    /**
     * Text that will display when the editor opens a file
     * @param filename
     */
    int OpenFile(char* filename);

    /**
     * save the currently opened file to disk 
     */
    void SaveFile();

    /**
     * Convert the contents stored in a row to a string 
     * 
     * @param buflen 
     * @return char* 
     */
    char* RowToString(int* buflen);


    /*---- OUTPUTS ----*/

    /**
     * Scroll the mouse/cursor around the editor
     */
    void Scroll();

    /**
     * Prepare the inputted buffer with the next frame 
     * @param ab 
     */
	void DrawRows(struct AppendBuffer::abuf* ab);

    /**
     * Display status bar at the bottom of the screen 
     * @param ab 
     */
    void DrawStatusBar(struct AppendBuffer::abuf* ab);

    /**
     * Display for information the user may need to see
     * if there is further information of an action
     * (close unsaved file, exit program, etc...) 
     * 
     * @param ab 
     */
    void DrawMessageBar(struct AppendBuffer::abuf* ab);

    /**
     * Update the screen buffer 
     */
	void RefreshScreen();

    /**
     * Set the Status Message with a formatted string
     * 
     * @param fmt 
     */
    void SetStatusMessage(const char* fmt...);	
}

#endif // __IO_HPP