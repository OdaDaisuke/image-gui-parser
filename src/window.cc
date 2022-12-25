#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#endif

#include <boost/numeric/conversion/cast.hpp>

#include <wx/filename.h>
#include <wx/splitter.h>
#include <wx/wx.h>

#include "window.h"

using boost::numeric_cast;

static const wxSize DefaultFrameSize(720, 480);

Window::Window() : wxFrame(NULL, wxID_ANY, wxT("Image binary parser"))
{
    auto splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_THIN_SASH);
    splitter->SetSashPosition(numeric_cast<int>(DefaultFrameSize.GetWidth() * splitter->GetSashGravity()));

    // TODO: DnD section
    // TODO: Summary section

    auto sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(splitter, 1, wxEXPAND);
    SetSizer(sizer);
    sizer->SetSizeHints(this);

    SetSize(DefaultFrameSize);
}

