#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#ifndef WX_PRECOMP
	//(*HeadersPCH(MainF)
	#include <wx/menu.h>
	#include <wx/filedlg.h>
	#include <wx/frame.h>
	//*)
#endif

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>
#include "Chip8.h"

extern sf::Font font;

class System
{
    sf::RenderTexture* canvas;
    Console* console;
    sf::Clock rclock;
    bool paused;
public:
    System(sf::RenderTexture*);
    ~System();
    void load(std::string filename);
    void Update();
    void Render();
    std::string getDebugText();
    void close();
};

#endif // SYSTEM_H_INCLUDED
