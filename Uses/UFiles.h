//--- UFiles.h
#ifndef UFilesH
#define UFilesH
//---------------------------------------------------------------------------
/*
  File treatment unit (for external memory structures).
*/
#include <stdio.h>
#include <vcl.h>
#include <time.h>
//---------------------------------------------------------------------------
/* aMode type   Description
        r	Open for reading only.
        w	Create for writing.
        a	Append; open for writing at end-of-file or create for writing if the file does not exist.
        r+	Open an existing file for update (reading and writing).
        w+	Create a new file for update (reading and writing).
        a+	Open for append; open (or create if the file does not exist) for update at the end of the file.
        b	Binary
        t	Text

        rb	Open a binary file for reading only.
        wb	Create a binary file for writing.
        r+b	Open an existing binary file for update (reading and writing).
        w+b	Create a new binary file for update (reading and writing).
*/

//---------------------------------------------------------------------------
// Generic file exception class.
//---------------------------------------------------------------------------
class EFile
{
public:
  EFile(): errorCode(0), errorMsg(""){}
  EFile(int pErrorCode): errorCode(pErrorCode), errorMsg(""){}
  EFile(int pErrorCode, AnsiString pErrorMsg): errorCode(pErrorCode), errorMsg(pErrorMsg){}
  int ErrorCode() {return errorCode;}
  AnsiString ErrorMsg() {return errorMsg;}
private:
  int errorCode;
  AnsiString errorMsg;
};

//---------------------------------------------------------------------------
// Derived file exception classes.
//---------------------------------------------------------------------------
class EFileNoClose: public EFile{};
class EFileCantClose: public EFile{};
class EFileNoRemove: public EFile{};
class EFileNoRename: public EFile{};
class EFileNoSeek: public EFile{};
class EFileNoOpen: public EFile{};

//---------------------------------------------------------------------------
// File modelling class.
//---------------------------------------------------------------------------
class TFile
{
public:
  TFile(AnsiString pFileName);
  virtual ~TFile();
  void Close();
  bool Eof();
  bool FileExists();
  long FilePos();
  long FileSize();
  bool IsOpen() {return opened;}
  void Open(const char *pMode);
  int Read(void *pInfo, size_t pSize, size_t pCount = 1);
  void Remove();
  void Rename(AnsiString pName);
  void SeekFromCur(long pPositions);
  void SeekFromEnd(long pPosition);
  void SeekTo(long pPosition);
  int Write(void *pInfo, size_t pSize, size_t pCount = 1);
protected:
  FILE *file;
  AnsiString fileName;
  bool opened;
};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// Prototype functions for saving to and loading from files.
//---------------------------------------------------------------------------
typedef void (*TSaveFunct)(void* , TFile* , void* );
typedef void* (*TLoadFunct)(TFile* , void *);

#endif

