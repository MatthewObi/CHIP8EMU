#ifndef WXSFMLCANVAS_H
#define WXSFMLCANVAS_H

#include <iostream>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <memory>
#include <random>
#include <locale>
#include "System.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#ifdef __WXGTK__
    #include <gdk/gdkx.h>
    #include <gtk/gtk.h>
#endif

using namespace std;

static const int kDefaultWindowWidth = 640;
static const int kDefaultWindowHeight = 480;
static const int kCanvasMargin = 0;

#define BFWIDTH 64
#define BFHEIGHT 32

struct wxSFMLCanvas : public wxPanel
{
    sf::RenderWindow* win;
    wxSFMLCanvas(
        wxWindow *parent,
        wxWindowID windowId,
        const wxPoint &position,
        const wxSize &size,
        long style
    );

    virtual void onUpdate();

    void onIdle(wxIdleEvent& event);
    void onPaint(wxPaintEvent& event);

    // Explicitly overriding prevents wxWidgets from drawing, which could result in flicker
    void onEraseBackground(wxEraseEvent& event){}

    /*void createRenderWindow()
    {
        sf::RenderWindow::create(GetHandle(), sf::ContextSettings(32));
    }*/
    void setwxWindowSize(const wxSize& size)
    {
        this->SetSize(size);
    }

    void setRenderWindowSize(const sf::Vector2u& size)
    {
        win->setSize(size);
        //auto resX = size.x;
        //auto resY = size.y;
        //int newH = (BFWIDTH*resY)/resX;
        //int newW = (BFHEIGHT*resX)/resY;
        //int displace = (newH - BFHEIGHT)/(-2);
        //int displace = (newW - BFWIDTH)/(-2);
        //win->setView(win->getDefaultView());
        //win->setView(sf::View(sf::FloatRect(0, 0, newW, BFHEIGHT)));
    }

    virtual void cleanUp() {
        this->Hide();
    }

    virtual ~wxSFMLCanvas();

wxDECLARE_EVENT_TABLE();
};

struct Canvas : public wxSFMLCanvas
{
    sf::RenderTexture* target;
    System sys;
    Canvas(
        wxWindow* parent,
        wxWindowID id,
        wxPoint position,
        wxSize size,
        sf::RenderTexture* tg,
        long style = 0) :
    wxSFMLCanvas(parent, id, position, size, style), target(tg), sys(tg)
    {
    }

    virtual void onUpdate()
    {
        if(!IsShown()) return;

        sys.Update();
        sys.Render();
        target->display();

        //setPosition(sf::Vector2i(GetPosition().x, GetPosition().y));

        win->clear(sf::Color::White);
        sf::Sprite buff(target->getTexture());
        //sf::FloatRect bRect = buff.getGlobalBounds();
        //buff.setOrigin(0, 0);
        buff.setOrigin(0,0);
        //buff.setOrigin(buff.getTexture()->getSize().x / 2,buff.getTexture()->getSize().y / 2);
        //auto newH = (512*win->getView().getSize().y)/win->getView().getSize().x;
        //auto scyfactor = newH / 512;
        //auto scxfactor = scyfactor;
        //auto scyfactor = (win->getView().getSize().y)/(win->getView().getSize().x);
        buff.setScale(9.8, 9.8);
        buff.setPosition(0,0);
        //buff.setPosition(win->getView().getCenter());
        //buff.setPosition((win->getDefaultView().getSize().x / 2) - (buff.getGlobalBounds().width / 2),0);
        win->draw(buff);
        #ifdef WRITE_DEBUG
        sf::Text txt(sys.getDebugText(), font, 16);
        txt.setFillColor(sf::Color::Yellow);
        txt.setPosition((win->getDefaultView().getSize().x / 2) - (buff.getGlobalBounds().width / 2),0);
        win->draw(txt);
        #endif
    }

    void onResize(wxSizeEvent& event)
    {
        auto size = event.GetSize();

        auto newCanvasWidth = size.x - (2 * kCanvasMargin);
        auto newCanvasHeight = size.y - (2 * kCanvasMargin);

        // Resize Canvas window
        this->setwxWindowSize({newCanvasWidth, newCanvasHeight});
        this->setRenderWindowSize({(unsigned int)newCanvasWidth, (unsigned int)newCanvasHeight});
    }
};

#endif // WXSFMLCANVAS_H
