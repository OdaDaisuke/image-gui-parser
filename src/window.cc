#include <functional>
#include <iostream>
#include <fstream>

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
#endif

#include <boost/numeric/conversion/cast.hpp>

#include <wx/msgdlg.h>
#include <wx/filename.h>
#include <wx/splitter.h>
#include <wx/wx.h>
#include "fstream.h"
#include "window.h"

using boost::numeric_cast;

static const wxSize DefaultFrameSize(720, 480);

static constexpr wxWindowID ID_IMAGE = 101;
static constexpr wxWindowID ID_SUMMARY = 102;
static constexpr wxWindowID ID_SUMMARY_ITEMS = 103;

unsigned char JPEGBinary[] = { 0xFF, 0xD8 };
unsigned char PNGBinary[] = { 0x89, 0x50, 0x4E, 0x47 };
unsigned char GIFBinary[] = { 0x47, 0x49, 0x46, 0x38 };

static const wxString AppTitle = wxS("Image binary parser");

static bool IsImage(const wxString& path)
{
    int8_t checkDataBuffer[4]{};
    std::fstream readImage(path, std::ios::in | std::ios::binary);
    readImage.read((char*)& checkDataBuffer, 4);

    if (memcmp((char*)& checkDataBuffer, (char*)& JPEGBinary, 2) == 0)
    {
        return true;
    }
    if (memcmp((char*)& checkDataBuffer, (char*)& PNGBinary, 4) == 0)
    {
        return true;
    }
    if (memcmp((char*)& checkDataBuffer, (char*)& GIFBinary, 4) == 0)
    {
        return true;
    }
    return false;
}

class DropTarget : public wxFileDropTarget
{
    typedef std::function<bool(wxCoord, wxCoord, const wxArrayString &)> callback_type;
public:
    explicit DropTarget(callback_type onDropFiles)
        : callback(onDropFiles)
    {
    }
    bool OnDropFiles(wxCoord x, wxCoord y, const wxArrayString &filenames) override
    {
        return callback(x, y, filenames);
    }
private:
    callback_type callback;
};

Window::Window() : wxFrame(NULL, wxID_ANY, AppTitle)
{
    auto splitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_THIN_SASH);
    splitter->SetSashPosition(numeric_cast<int>(DefaultFrameSize.GetWidth() * splitter->GetSashGravity()));
    splitter->SetSashGravity(0.25);

    wxImage::AddHandler(new wxPNGHandler());
    wxImage::AddHandler(new wxJPEGHandler());
    wxImage::AddHandler(new wxGIFHandler());
    imagePreview = new wxStaticBitmap(this, wxID_ANY, wxBitmap("sample.png", wxBITMAP_TYPE_PNG), wxPoint(50,100), wxSize(100, 500));

    // Image preview pane
    auto lpanel = new wxPanel(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxBORDER_SIMPLE);
    wxBoxSizer *lpanelBoxsizer = new wxBoxSizer(wxVERTICAL);
    lpanelBoxsizer->Add(imagePreview, 0, wxEXPAND);
    lpanel->SetSizer(lpanelBoxsizer);

    // Image summary pane
    auto rpanel = new wxPanel(splitter, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL | wxBORDER_SIMPLE);
    propGrid = new wxPropertyGrid(rpanel, ID_SUMMARY, wxDefaultPosition, wxDefaultSize,
        wxPG_BOLD_MODIFIED | wxPG_SPLITTER_AUTO_CENTER | wxBORDER_NONE);
    propGrid->ShowScrollbars(wxSHOW_SB_NEVER, wxSHOW_SB_NEVER);
    propGrid->Hide();
    filePropertyList = new wxDataViewCtrl(rpanel, ID_SUMMARY_ITEMS, wxDefaultPosition, wxDefaultSize, wxDV_ROW_LINES | wxBORDER_NONE);
    filePropertyList->Hide();
    boxSizer = new wxBoxSizer(wxVERTICAL);
    boxSizer->Add(propGrid, 0, wxEXPAND);
    boxSizer->Add(filePropertyList, 1, wxEXPAND);
    rpanel->SetSizer(boxSizer);

    splitter->SplitVertically(lpanel, rpanel);

    auto sizer = new wxBoxSizer(wxHORIZONTAL);
    sizer->Add(splitter, 1, wxEXPAND);
    SetSizer(sizer);
    sizer->SetSizeHints(this);

    SetSize(DefaultFrameSize);
    SetDropTarget(new DropTarget([this](wxCoord, wxCoord, const wxArrayString &filenames) {
        OpenFiles(filenames);
        return true;
    }));
}

void Window::OpenFile(const wxString &path)
try
{
    auto file = FileInputStream::Open(path);
    bool isImage = IsImage(path);
    if (!isImage) {
        wxMessageBox( wxT("Selected file is not a image") );
        return;
    }
    propGrid->Hide();
    filePropertyList->Hide();

    SetTitle(wxS("「" + path + "」 | ") + AppTitle);

    auto bitmapType = wxBITMAP_TYPE_PNG;
    // TODO jpeg, gif対応
    // if (imageType == JPEGBinary) {
    //     std::cout << "jpeg";
    //     bitmapType = wxBITMAP_TYPE_JPEG;
    // }
    imagePreview->SetBitmap(wxBitmap(wxT("" + path), bitmapType));
}
catch (const std::system_error &ex)
{
    auto code = static_cast<unsigned long>(ex.code().value());
    wxMessageBox(wxSysErrorMsg(code), AppTitle, wxOK | wxICON_WARNING, this);
}
catch (const std::exception &ex)
{
    wxMessageBox(ex.what(), AppTitle, wxOK | wxICON_WARNING, this);
}

void Window::OpenFiles(const wxArrayString &paths)
{
    auto first = true;
    for (const auto &path : paths) {
        if (first) {
            OpenFile(path);
            first = false;
        } else {
            auto window = new Window;
            window->Show();
            window->OpenFile(path);
        }
    }
}

