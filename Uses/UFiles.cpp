//--- UFiles.cpp

#pragma hdrstop
#include "UFiles.h"
/*
  File treatment unit (External memory structures).
*/
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// Methods of the File modelling class.
//---------------------------------------------------------------------------
TFile::TFile(AnsiString pFileName)
{
  fileName = pFileName;
  opened = false;
}
//---------------------------------------------------------------------------
TFile::~TFile()
{
  try
  {
    Close();
  }
  catch(EFileCantClose)
  {
    throw EFileCantClose();
  };
}
//---------------------------------------------------------------------------
void TFile::Close()
{
  if (opened)
  {
    if(fclose(file) == 0)
      opened = false;
    else
      throw EFileCantClose();
  }
}
//---------------------------------------------------------------------------
bool TFile::Eof()
{
  return feof(file);
}
//---------------------------------------------------------------------------
bool TFile::FileExists()
{
  bool exists = false;
  FILE *fAux = fopen(fileName.c_str(), "r");
  if (fAux != NULL)
  {
    exists = true;
    delete fAux;
  }
  return exists;
}
//---------------------------------------------------------------------------
long TFile::FilePos()
{
  return ftell(file);
}
//---------------------------------------------------------------------------
long TFile::FileSize()
{
  long position = ftell(file);
  fseek(file, 0, SEEK_END);
  long result = ftell(file);
  fseek(file, position, SEEK_SET);
  return result;
}
//---------------------------------------------------------------------------
void TFile::Open(const char *pMode)
{
  file = fopen(fileName.c_str(), pMode);
  if (file == NULL)
    throw EFileNoOpen();
  opened = true;
}
//---------------------------------------------------------------------------
int TFile::Read(void* pInfo, size_t pSize, size_t pCount)
{
  return fread(pInfo, pSize, pCount, file);
}
//---------------------------------------------------------------------------
void TFile::Remove()
{
  if (!opened)
  {
    if (remove(fileName.c_str()) != 0)
      throw EFileNoRemove();
  }
  else
    throw EFileNoClose();
}
//---------------------------------------------------------------------------
void TFile::Rename(AnsiString pName)
{
  if (!opened)
  {
    if (rename(fileName.c_str(), pName.c_str()) != 0)
      throw EFileNoRename();
    else
      fileName = pName;
  }
  else
    throw EFileNoClose();
}
//---------------------------------------------------------------------------
void TFile::SeekFromCur(long pPositions)
{
  if (fseek(file, pPositions, SEEK_CUR) != 0)
    throw EFileNoSeek();
}
//---------------------------------------------------------------------------
void TFile::SeekFromEnd(long pPosition)
{
  if (fseek(file, pPosition, SEEK_END) != 0)
    throw EFileNoSeek();
}
//---------------------------------------------------------------------------
void TFile::SeekTo(long pPosition)
{
  if (fseek(file, pPosition, SEEK_SET) != 0)
    throw EFileNoSeek();
}
//---------------------------------------------------------------------------
int TFile::Write(void *pInfo, size_t pSize, size_t pCount)
{
  return fwrite(pInfo, pSize, pCount, file);
}
//---------------------------------------------------------------------------


