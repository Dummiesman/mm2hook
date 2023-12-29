#pragma once
#include <mm2_common.h>
#include <string>
#include <vector>
#include "file.h"

namespace MM2
{
    // Forward declarations
    class Stream;

    // External declarations

    // Class definitions

    class Stream {
    private:
        static hook::Type<coreFileMethods> coreFileStandard;
        static hook::Type<coreFileMethods> zipFile;
    public:
        static hook::Type<coreFileMethods *> sm_DefaultOpenMethods;
        static hook::Type<coreFileMethods *> sm_DefaultCreateMethods;

        AGE_API static void     DumpOpenFiles();
        AGE_API static Stream*  Open(LPCSTR filename, bool readOnly);
        AGE_API static Stream*  Open(LPCSTR filename, const coreFileMethods* fileMethods, bool readOnly);                                
        AGE_API static Stream*  Create(LPCSTR filename);
        AGE_API static Stream*  Create(LPCSTR filename, const coreFileMethods* fileMethods);                                                          
        AGE_API int             Read(LPVOID dstBuf, int size);
        AGE_API int             Write(const LPVOID srcBuf, int size);
        AGE_API int             GetCh();
        AGE_API int             PutCh(unsigned char ch);
        AGE_API int             Seek(int offset);
        AGE_API int             Tell();
        AGE_API int             Close();
        AGE_API int             Size();
        AGE_API int             Flush();

        std::string ReadString(int length);
        void WriteString(std::string str);
        std::string AsString();

        static void BindLua(LuaState L);
    };

    AGE_EXT int     fseek       (Stream *stream, int position, seekWhence whence);
    AGE_EXT int     fgets       (char *buffer, int length, Stream *stream);
    AGE_EXT void    fprintf     (Stream *stream, char const *format, ...);
    AGE_EXT int     fscanf      (Stream *stream, char const *format, ...);
}