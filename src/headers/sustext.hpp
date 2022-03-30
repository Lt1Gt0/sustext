#ifndef __SUSTEXT_HPP
#define __SUSTEXT_HPP

#include "defines.hpp"
#include "data.hpp"
#include "terminal.hpp"
#include "appendbuffer.hpp"
#include "editor.hpp"
#include "editorflags.hpp"
#include "filehandler.hpp"

class Sustext{
    public:
    int argc;
    char** argv;

    SusFlags flags;
    FileHandler filehandler;
    Editor editor;

    static EditorConfig E;
    
    public:
        Sustext(int _argc, char** _argv);
        ~Sustext();

        int Initialize();
};

#endif // __SUSTEXT_HPP