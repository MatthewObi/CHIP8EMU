#include "System.h"
#include <fstream>

sf::Font font;

System::System(sf::RenderTexture* tg)
{
    canvas = tg;
    console = new Console;
    console->init();
    rclock.restart();
    #ifdef WRITE_DEBUG
    font.loadFromFile("debug.ttf");
    #endif
}

System::~System() {
    delete console;
}

void System::load(std::string filename) {
    console->init();
    std::ifstream ifs;
    ifs.open(filename, std::ifstream::binary);
    console->load(ifs);
    ifs.close();
    rclock.restart();
}

void System::Update() {
    if(console->isLoaded()) {
        uint64_t dt = rclock.getElapsedTime().asMicroseconds();
        if(dt < (1666 / 1)) return;
        rclock.restart();
        if(!console->isPaused())
            console->update(dt / (1666 / 1));
    } else rclock.restart();
}

void System::Render() {
    canvas->clear(sf::Color::Black);
    if(console->isLoaded()) {
        console->render(canvas);
        if(console->isReadyToRender()) {
            //console->render(canvas);
        }
    }
}

std::string System::getDebugText() {
    #ifdef WRITE_DEBUG
    std::stringstream s;
    console->debug(s);
    return s.str();
    #else
    return std::string("");
    #endif
}

void System::close() {
    console->init();
    canvas->clear(sf::Color::Black);
}
