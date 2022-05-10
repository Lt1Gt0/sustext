#pragma once
#ifndef _SUSTEXT_TERMINAL_H
#define _SUSTEXT_TERMINAL_H

#include <termios.h>

namespace Terminal
{
    typedef struct {
        struct termios OriginalTermios;
    } ConfigData;

    /**
     * Initialize the editor functions 
     */
    void initEditor();

    /**
     * exit the program with a message to display what error occured 
     * 
     * @param s error message
     */
    void die(const char* s);

    /**
     * Put the terminal window back to the origional state it was
     * at before (from raw input to canonical mode (cooked mode)) 
     */
    void disableRawMode();
    
    /**
     * Enable raw input mode for the terminal by enabling and disabling
     * canonical flags
     */
    void enableRawMode();
    
    /**
     * Read key input 
     * 
     * @return character that was pressed 
     */
    int editorReadKey();
    
    /**
     * Get the Cursor Position relative to the terminal window
     * 
     * @param rows (int) value to store row information  
     * @param cols (int) value to store column information
     * @return (int) exit status: 0 = success
     */
    int getCursorPosition(int *rows, int *cols);
    
    /**
     * Get the Window Size of the terminal
     * 
     * @param rows (int) value to store row information  
     * @param cols (int) value to store column information
     * @return (int) exit status: 0 = success
     */
    int getWindowSize(int *rows, int *cols);
}

#endif // _SUSTEXT_TERMINAL_H