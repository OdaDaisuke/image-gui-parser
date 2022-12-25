#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#endif
#include <wx/wx.h>
#include "app.h"
#include "window.h"

bool App::OnInit()
{
  if (!wxApp::OnInit()) {
    return false;
  }
  Window *window = new Window();
  window->Show();

  return true;
}

IMPLEMENT_APP(App)
