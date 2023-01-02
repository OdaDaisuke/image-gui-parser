#pragma once

#include <boost/numeric/conversion/cast.hpp>
#include <wx/wfstream.h>

class FileInputStream
{
public:
    static inline std::shared_ptr<FileInputStream> Open(const wxString &path)
    {
        return std::make_shared<FileInputStream>(path);
    }
    explicit FileInputStream(const wxString &path)
    {
        tmpImageStream = std::make_shared<wxFileInputStream>(path);
        if (!tmpImageStream->IsOk())
            throw std::runtime_error("No such file or directory.");
    }
    size_t read(void *ptr, size_t size)
    {
        return size ? tmpImageStream->Read(ptr, size).LastRead() : 0;
    }
    void seek(uint64_t position)
    {
        if (tmpImageStream->SeekI(boost::numeric_cast<wxFileOffset>(position)) == wxInvalidOffset)
            throw std::runtime_error("Failed to seek");
    }
    uint64_t tell() const
    {
        return boost::numeric_cast<uint64_t>(tmpImageStream->TellI());
    }
    uint64_t size() const
    {
        return tmpImageStream->GetSize();
    }
private:
    std::shared_ptr<wxFileInputStream> tmpImageStream;
};
