#include <wx/wx.h>

class Hello : public wxFrame
{
public:
  Hello();
};

class App : public wxApp
{
public:
  virtual bool OnInit();
};


Hello::Hello() : wxFrame(NULL, -1, wxT("Hello wxWidgets!"))
{
}

IMPLEMENT_APP(App)

bool App::OnInit()
{
  Hello *hello = new Hello();
  hello->Show();

  return true;
}

