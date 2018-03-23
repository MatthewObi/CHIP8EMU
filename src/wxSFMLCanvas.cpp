#include "wxSFMLCanvas.h"

wxSFMLCanvas::wxSFMLCanvas(
        wxWindow *parent = nullptr,
        wxWindowID windowId = -1,
        const wxPoint &position = wxDefaultPosition,
        const wxSize &size = wxDefaultSize,
        long style = 0) :
    wxPanel(parent, windowId, position, size, style)
    {
        #ifdef __WXGTK__

        // GTK implementation requires to go deeper to find the
        // low-level X11 identifier of the widget
        gtk_widget_realize(m_wxwindow);
        gtk_widget_set_double_buffered(m_wxwindow, false);
        GdkWindow* Win = gtk_widget_get_window( (GtkWidget *) GetHandle() );
        XFlush(GDK_WINDOW_XDISPLAY(Win));
        win = new sf::RenderWindow(GDK_WINDOW_XWINDOW(Win));

        #else

        win = new sf::RenderWindow(GetHandle());
        /*auto resX = win->getSize().x;
        auto resY = win->getSize().y;
        int newH = (512*resY)/resX;
        //int displace = (newH - 256)/(-2);

        win->setView(sf::View(sf::FloatRect(0, 0, 512, newH)));*/

        #endif // __WXGTK__
    }

wxSFMLCanvas::~wxSFMLCanvas()
{
    cleanUp();
    win->close();
    delete win;
}

void wxSFMLCanvas::onIdle(wxIdleEvent& event)
{
    // Send a paint message when control is idle, to ensure max framerate
    Refresh();
}

void wxSFMLCanvas::onPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);     // Prepare control to be repainted
    onUpdate();             // Tick update
    win->display();              // Draw
}

void wxSFMLCanvas::onUpdate() {}

wxBEGIN_EVENT_TABLE(wxSFMLCanvas, wxPanel)
  EVT_IDLE(wxSFMLCanvas::onIdle)
  EVT_PAINT(wxSFMLCanvas::onPaint)
  EVT_ERASE_BACKGROUND(wxSFMLCanvas::onEraseBackground)
wxEND_EVENT_TABLE()
