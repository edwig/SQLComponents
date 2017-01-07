//////////////////////////////////////////////////////////////////////////
//
// SMX String
//
// Std Mfc eXtension (SMX)String is a string derived from std::string
// But does just about everything that MFC CString also does
// The string is derived from std::string with CString methods
// SMX = std::string with MFC eXtensions
//
// Copyright (c) 2016 ir. W.E. Huisman MSC
//
//////////////////////////////////////////////////////////////////////////

// C4239: nonstandard extension used
#pragma warning (disable: 4239)

#include "SMX_String.h"
#include <stdio.h>
#include <stdexcept>
#include <atlstr.h>

// Base CTOR
String::String()
      // :m_references(1)
{
}

// CTOR from character pointer
String::String(PCTSTR p_string)
       :string(p_string)
       //,m_references(1)
{
}

// CTOR from other string
String::String(String& p_string)
       //:m_references(1)
{
  *this = p_string.c_str();
}

// CTOR from std::string
String::String(string& p_string)
       //:m_references(1)
{
  *this = p_string.c_str();
}

BSTR String::AllocSysString()
{
  USES_CONVERSION;
  wstring str(A2CW(this->c_str()));
  BSTR bstrResult = ::SysAllocString(str.c_str());

  if(bstrResult == NULL)
  {
    throw std::bad_alloc();
  }
  return(bstrResult);
}

// Append a string, or n chars from a string
//
void String::Append(PCTSTR p_string,int p_length)
{
  string str(p_string);
  append(str.substr(0,p_length));
}

// Append a formatted string
void String::AppendFormat(PCTSTR p_format,...)
{
  va_list argList;
  va_start(argList,p_format);

  AppendFormatV(p_format,argList);

  va_end(argList);
}

void String::AppendFormat(UINT p_strID,...)
{
  va_list argList;
  va_start(argList,p_strID);

  AppendFormatV(p_strID,argList);

  va_end(argList);
}

void String::AppendFormatV(PCTSTR p_format,va_list p_list)
{
  // Getting a buffer of the correct length
  int len = _vscprintf(p_format,p_list) + 1;
  char* buffer = new char[len];
  // Formatting the parameters
  vsprintf_s(buffer,len,p_format,p_list);
  // Adding to the string
  append(buffer);

  delete[] buffer;
}

void String::AppendFormatV(UINT p_strID,va_list p_list)
{
  // Getting the string
  String str;
  if(str.LoadString(p_strID))
  {
    // Getting a buffer of the correct length
    int len = _vscprintf(str.c_str(),p_list) + 1;
    char* buffer = new char[len];
    // Formatting the parameters
    vsprintf_s(buffer,len,str.c_str(),p_list);
    // Adding to the string
    append(buffer);

    delete[] buffer;
  }
}

// Delete, returning new length
int String::Delete(int p_index,int p_count)
{
  erase(p_index,p_count);
  return length();
}

// Format a string
void String::Format(PCTSTR p_format,...)
{
  va_list argList;
  va_start(argList,p_format);

  FormatV(p_format,argList);

  va_end(argList);
}

void String::Format(UINT p_strID,...)
{
  va_list argList;
  va_start(argList,p_strID);

  FormatV(p_strID,argList);

  va_end(argList);
}

// Format a variable list
void String::FormatV(PCTSTR p_format,va_list p_list)
{
  // Getting a buffer of the correct length
  int len = _vscprintf(p_format,p_list) + 1;
  char* buffer = new char[len];
  // Formatting the parameters
  vsprintf_s(buffer,len,p_format,p_list);
  // Adding to the string
  *this = buffer;

  delete[] buffer;
}

void String::FormatV(UINT p_strID,va_list p_list)
{
  // Getting the string
  String str;
  if(str.LoadString(p_strID))
  {
    // Getting a buffer of the correct length
    int len = _vscprintf(str.c_str(),p_list) + 1;
    char* buffer = new char[len];
    // Formatting the parameters
    vsprintf_s(buffer,len,str.c_str(),p_list);
    // Adding to the string
    *this = buffer;

    delete[] buffer;
  }
}

// Format a message by system format instead of printf
void String::FormatMessage(PCTSTR p_format,...)
{
  va_list argList;
  va_start(argList,p_format);

  FormatMessageV(p_format,&argList);

  va_end(argList);
}

void String::FormatMessage(UINT p_strID,...)
{
  va_list argList;
  va_start(argList,p_strID);

  FormatMessageV(p_strID,&argList);

  va_end(argList);
}

void String::FormatMessageV(PCTSTR p_format,va_list* p_list)
{
  // format message into temporary buffer pszTemp
  CHeapPtr<CHAR,CLocalAllocator> pszTemp;

  // FormatMessage returns zero in case of failure or the number of characters
  // if it is success, but we may actually get 0 as a number of characters.
  // So to avoid this situation use SetLastError and GetLastErorr to determine
  // whether the function FormatMessage has failed.
  DWORD dwLastError = ::GetLastError();
  ::SetLastError(0);

  DWORD dwResult = ::FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER
                                  ,p_format,0,0,reinterpret_cast<PSTR>(&pszTemp),0,p_list);

  if((dwResult == 0) && (::GetLastError() != 0))
  {
    throw std::bad_alloc();
  }
  ::SetLastError(dwLastError);

  *this = (LPSTR)pszTemp;
}

void String::FormatMessageV(UINT p_strID,va_list* p_list)
{
  String format;
  
  if(format.LoadString(p_strID))
  {
    // format message into temporary buffer pszTemp
    CHeapPtr<CHAR,CLocalAllocator> pszTemp;
    DWORD dwLastError = ::GetLastError();
    ::SetLastError(0);

    DWORD dwResult = ::FormatMessage(FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ALLOCATE_BUFFER
                                     ,format.c_str(),0,0,reinterpret_cast<PSTR>(&pszTemp),0,p_list);

    if((dwResult == 0) && (::GetLastError() != 0))
    {
      throw std::bad_alloc();
    }
    ::SetLastError(dwLastError);

    *this = (LPSTR)pszTemp;
  }
}

// Getting buffer of at least p_length + 1 size
PSTR String::GetBufferSetLength(int p_length)
{
  reserve(p_length);
  return (PSTR)c_str();
}

// Getting a shell environment variable
BOOL String::GetEnvironmentVariable(PCTSTR p_variable)
{
  BOOL  result = FALSE;
  ULONG length = ::GetEnvironmentVariable(p_variable,NULL,0);

  if(length == 0)
  {
    Empty();
  }
  else
  {
    PSTR pszBuffer = new char[length + 1];
    ::GetEnvironmentVariable(p_variable,pszBuffer,length);
    *this = pszBuffer;
    delete [] pszBuffer;
    result = TRUE;
  }
  return result;
}

// Insert char or string
int String::Insert(int p_index,PCTSTR p_string)
{
  insert(p_index,p_string);
  return length();
}

int String::Insert(int p_index,char p_char)
{
  insert(p_index,1,p_char);
  return length();
}

// Taking the left side of the string
String String::Left(int p_length)
{
  if(p_length < 0)
  {
    p_length = 0;
  }
  return String(substr(0,p_length));
}

// Load a string from the resources
BOOL String::LoadString(UINT p_strID)
{
  const ATLSTRINGRESOURCEIMAGE* resource = nullptr;
  HINSTANCE instance = _AtlBaseModule.GetHInstanceAt(0);

  for(int ind = 1; instance != NULL && resource == nullptr; instance = _AtlBaseModule.GetHInstanceAt(ind++))
  {
    resource = AtlGetStringResourceImage(instance,p_strID,0);
    if(resource)
    {
      return LoadString(instance,p_strID,0);
    }
  }
  return FALSE;
}

BOOL String::LoadString(HINSTANCE p_inst,UINT p_strID,WORD p_languageID)
{
  const ATLSTRINGRESOURCEIMAGE* image = AtlGetStringResourceImage(p_inst,p_strID,p_languageID);
  if(image == nullptr)
  {
    return(FALSE);
  }
  int logicSize = image->nLength + 1;
  int  realSize = logicSize * sizeof(WCHAR);
  WCHAR* temp = new WCHAR[logicSize + 1];
  memcpy_s(temp,realSize,(void*)image->achString,realSize);
  ((char*)temp)[realSize - 1] = 0;
  ((char*)temp)[realSize - 2] = 0;

  USES_CONVERSION;
  *this = (LPTSTR) CW2A(temp);

  delete [] temp;
  return(TRUE);
}

// Lock the buffer returning the string
// Does not exactly what CString does!!
PCSTR String::LockBuffer()
{
  // Lock();
  return c_str();
}

void String::MakeLower()
{
  for(size_t ind = 0;ind < length();++ind)
  {
    replace(ind,1,1,(char)tolower(at(ind)));
  }
}

void String::MakeUpper()
{
  for(size_t ind = 0;ind < length(); ++ind)
  {
    replace(ind,1,1,(char)toupper(at(ind)));
  }
}

// Releasing the buffer again
void String::ReleaseBuffer(int p_newLength /*=-1*/)
{
  if(p_newLength < 0)
  {
    shrink_to_fit();
  }
  else
  {
    resize(p_newLength,0);
  }
}

void String::ReleaseBufferSetLength(int p_newLength)
{
  if(p_newLength < 0 || p_newLength > (int)capacity())
  {
    throw std::bad_array_new_length();
  }
  else
  {
    resize(p_newLength,0);
  }
}

// Remove all occurrences of char
int String::Remove(char p_char)
{
  int count = 0;
  size_t pos = 0;
  while(pos < size())
  {
    if(at(pos) == p_char)
    {
      erase(pos,1);
      ++count;
    }
    else
    {
      ++pos;
    }
  }
  return count;
}

// Replace a string or a character
int String::Replace(PCSTR p_old,PCSTR p_new)
{
  int count  = 0;
  int length = strlen(p_old);
  size_t pos = find(p_old);

  while(pos != string::npos)
  {
    replace(pos,length,p_new);
    pos = find(p_old);
    ++count;
  }
  return count;
}

int String::Replace(char p_old,char p_new)
{
  int count = 0;
  size_t pos = find(p_old);

  while(pos != string::npos)
  {
    replace(pos,1,1,p_new);
    pos = find(p_old);
    ++count;
  }
  return count;
}

// Get substring from the right 
String  String::Right(int p_length)
{
  int sz = (int) size();
  if(p_length > sz)
  {
    p_length = sz;
  }
  return String(substr(sz - p_length,p_length));
}

// Set char at a position
void String::SetAt(int p_index,char p_char)
{
  int sz = (int) size();
  if(p_index < 0 || p_index > sz)
  {
    throw std::bad_array_new_length();
  }
  if(p_index == sz)
  {
    push_back(p_char);
  }
  else
  {
    replace(p_index,1,1,p_char);
  }
}

// SetString interface
void String::SetString(PCSTR p_string)
{
  if(p_string == nullptr)
  {
    throw std::bad_typeid();
  }
  DWORD_PTR str   = (DWORD_PTR) p_string;
  DWORD_PTR begin = (DWORD_PTR) c_str();
  DWORD_PTR end   = (DWORD_PTR) c_str() + size();
  if(begin <= str && str <= end)
  {
    // It was a substring of the existing string
    erase(0,str - begin);
  }
  else
  {
    // Completely different string
    *this = p_string;
  }
}

void String::SetString(PCSTR p_string,int p_length)
{
  if(p_string == nullptr)
  {
    throw std::bad_typeid();
  }
  std::string str(p_string);
  if(p_length < (int)str.size())
  {
    str.resize(p_length);
  }
  SetString(str.c_str());
}

// Set string from a COM BSTR
// Does something different than CString, because it does NOT 
// reduce the amount of string space, but copies the BSTR to the String
BSTR String::SetSysString(BSTR* p_string)
{
  int nLen = ::MultiByteToWideChar(CP_ACP,0,c_str(),size(),NULL,NULL);
  BOOL bSuccess = ::SysReAllocStringLen(p_string,NULL,nLen);
  if(bSuccess)
  {
    ::MultiByteToWideChar(CP_ACP,0,c_str(),size(),*p_string,nLen);
  }
  else
  {
    throw bad_alloc();
  }
  return(*p_string);
}

// Leftmost string not in argument
String String::SpanExcluding(PCSTR p_string)
{
  if(p_string == nullptr)
  {
    throw std::bad_typeid();
  }
  return Left(strcspn(c_str(),p_string));
}

// Leftmost string in argument
String String::SpanIncluding(PCSTR p_string)
{
  if(p_string == nullptr)
  {
    throw std::bad_typeid();
  }
  return Left(strspn(c_str(),p_string));
}

// Length of the string
int String::StringLength(PCSTR p_string)
{
  if(p_string == nullptr)
  {
    return 0;
  }
  return strlen(p_string);
}

// Return tokenized strings
String String::Tokenize(PCSTR p_tokens,int& p_curpos)
{
  if(p_curpos < 0)
  {
    throw std::bad_alloc();
  }
  if((p_tokens == nullptr) || (*p_tokens == 0))
  {
    if(p_curpos < GetLength())
    {
      return (String(c_str() + p_curpos));
    }
  }
  else
  {
    PCSTR pszPlace = c_str() + p_curpos;
    PCSTR pszEnd   = c_str() + size();
    if(pszPlace < pszEnd)
    {
      int nIncluding = strspn(pszPlace,p_tokens);

      if((pszPlace + nIncluding) < pszEnd)
      {
        pszPlace += nIncluding;
        int nExcluding = strcspn(pszPlace,p_tokens);

        int iFrom  = p_curpos + nIncluding;
        int nUntil = nExcluding;
        p_curpos   = iFrom + nUntil + 1;

        return(Mid(iFrom,nUntil));
      }
    }
  }
  // return empty string, done tokenizing
  p_curpos = -1;

  return String("");
}

String& String::TrimLeft(char p_char)
{
  int count = 0;
  PCSTR str = c_str();
  while(*str && *str == p_char)
  {
    ++str;
    ++count;
  }

  erase(0,count);
  return *this;
}

String& String::TrimLeft(PCSTR p_string)
{
  // if we're not trimming anything, we're not doing any work
  if((p_string == nullptr) || (*p_string == 0))
  {
    return(*this);
  }

  PCSTR psz = c_str();
  while((*psz != 0) && (strchr(p_string,*psz) != NULL)) ++psz;

  if(psz != c_str())
  {
    // fix up data and length
    int iFirst = int(psz - c_str());
    erase(0,iFirst);
  }
  return(*this);
}

String& String::TrimRight(char p_char)
{
  size_t pos = size() - 1;
  while(pos != string::npos)
  {
    if(at(pos) != p_char)
    {
      ++pos;
      break;
    }
    --pos;
  }
  erase(pos);
  return *this;
}

String& String::TrimRight(PCSTR p_string)
{
  // if we're not trimming anything, we're not doing any work
  if((p_string == nullptr) || (*p_string == 0))
  {
    return(*this);
  }

  // Start at the ending of the string
  size_t pos = size() - 1;
  while(pos != string::npos)
  {
    if(strchr(p_string,at(pos)) != nullptr)
    {
      --pos;
    }
    else
    {
      ++pos;
      break;
    }
  }

  if(pos >= 0)
  {
    // truncate at left-most matching character
    erase(pos,string::npos);
  }
  return(*this);
}

//////////////////////////////////////////////////////////////////////////
//
// OPERATORS
//
//////////////////////////////////////////////////////////////////////////

String::operator char*()
{
  return (char*)c_str();
}

String::operator const char*()
{
  return c_str();
}

String
String::operator+(String& p_extra)
{
  String string(c_str());
  string.append(p_extra);
  return string;
}

String&
String::operator+=(String& p_extra)
{
  append(p_extra);
  return *this;
}

//////////////////////////////////////////////////////////////////////////
//
// StringData counterparts
//
//////////////////////////////////////////////////////////////////////////

// void String::AddRef()
// {
//   _InterlockedIncrement(&m_references);
// }
// 
// void String::Release()
// {
//   if(_InterlockedDecrement(&m_references) <= 0)
//   {
//     delete this;
//   }
// }
// 
// void String::Lock()
// {
//   // Locked buffers can't be shared
//   --m_references;
//   if(m_references == 0)
//   {
//     m_references = -1;
//   }
// 
// }
// 
// void String::Unlock()
// {
//   if(IsLocked())
//   {
//     ++m_references;
//     if(m_references == 0)
//     {
//       m_references = 1;
//     }
//   }
// }
