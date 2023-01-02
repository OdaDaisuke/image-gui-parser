#pragma once

#include <wx/wx.h>
#include <wx/dataview.h>
#include <wx/propgrid/propgrid.h>

class Window : public wxFrame
{
public:
    Window();
    void OpenFile(const wxString &);
    void OpenFiles(const wxArrayString &);

protected:
    wxStaticBitmap *imagePreview = nullptr;
    wxBoxSizer     *boxSizer = nullptr;
    wxPropertyGrid *propGrid = nullptr;
    wxDataViewCtrl *filePropertyList = nullptr;

    wxDataViewItem selected = {};
};
