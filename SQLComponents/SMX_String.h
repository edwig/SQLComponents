//////////////////////////////////////////////////////////////////////////
//
// File: SMX_String.h
//
// Std Mfc eXtension String is a string derived from std::string
// But does just about everything that MFC CString also does
// Copyright (c) 2016 ir. W.E. Huisman MSC
//
//////////////////////////////////////////////////////////////////////////

#pragma once
#include <wtypes.h>
#include <string>
#include <stdarg.h>

using namespace std;

class String : public std::string
{
public:
  // Empty CTOR
  String();
  // CTOR from character pointer
  String(PCTSTR p_string);
  // CTOR from other string
  String(String& p_string);
  // CTOR from std::string
  String(string& p_string);

  // Convert String to BSTR. Free it with "SysFreeString"
  BSTR      AllocSysString();
  // Append a string, or n chars from a string
  void      Append(PCTSTR p_string);
  void      Append(PCTSTR p_string,int p_length);
  // Append a single character
  void      AppendChar(char p_char);
  // Append a formatted string
  void      AppendFormat(PCTSTR p_format,...);
  void      AppendFormat(UINT   p_strID ,...);
  // Append a formatted variable list
  void      AppendFormatV(PCTSTR p_format,va_list p_list);
  void      AppendFormatV(UINT   p_strID, va_list p_list);
  // ANSI/OEM Conversions
  void      AnsiToOem();
  void      OemToAnsi();
  // Collate
  int       Collate(PCTSTR p_string);
  int       CollateNoCase(PCTSTR p_string);
  // Compare
  int       Compare(PCTSTR p_string);
  int       CompareNoCase(PCTSTR p_string);
  // Construct. MFC does this, but it's unclear/undocumented why!
  // void    Construct(String* p_string);

  // Delete, returning new length
  int       Delete(int p_index,int p_count);
  // Make empty
  void      Empty();
  // Find position or -1 for not found
  int       Find(PCTSTR p_string,int p_start = 0);
  int       Find(char   p_char,  int p_start = 0);
  // Find on of the chars or -1 for not found
  int       FindOneOf(PCTSTR p_string);
  // Format a string
  void      Format(PCTSTR p_format,...);
  void      Format(UINT   p_strID ,...);
  // Format a variable list
  void      FormatV(PCTSTR p_format,va_list p_list);
  void      FormatV(UINT   p_strID, va_list p_list);
  // Format a message by system format instead of printf
  void      FormatMessage(PCTSTR p_format,...);
  void      FormatMessage(UINT   p_strID, ...);
  // Format a message by system format from variable list
  void      FormatMessageV(PCTSTR p_format,va_list* p_list);
  void      FormatMessageV(UINT   p_strID, va_list* p_list);
  // Free extra space (shrinking the string)
  void      FreeExtra();
  // Getting a shell environment variable
  BOOL      GetEnvironmentVariable(PCTSTR p_variable);
  // Getting string length
  int       GetLength();
  // Getting a char from the string
  int       GetAt(int p_index);
  // Length of allocated string capacity
  unsigned  GetAllocLength();
  // Getting buffer of at least p_length + 1 size
  PSTR      GetBufferSetLength(int p_length);
  // Releasing the buffer again
  void      ReleaseBuffer(int p_newLength = -1);
  void      ReleaseBufferSetLength(int p_newLength);
  // Getting the string
  PCSTR     GetString();
  // Insert char or string
  int       Insert(int p_index,PCTSTR p_string);
  int       Insert(int p_index,char   p_char);
  // See if string is empty
  bool      IsEmpty();
  // Taking the left side of the string
  String    Left(int p_length);
  // Locking the buffer
  PCSTR     LockBuffer();
  // Load a string from the resources
  BOOL      LoadString(UINT p_strID);
  BOOL      LoadString(HINSTANCE p_inst,UINT p_strID);
  BOOL      LoadString(HINSTANCE p_inst,UINT p_strID,WORD p_languageID);
  // Make lower/upper case or reverse
  void      MakeLower();
  void      MakeReverse();
  void      MakeUpper();
  // Take substring out of the middle
  String    Mid(int p_index);
  String    Mid(int p_index,int p_length);    
  // Preallocate a string of specified size
  void      Preallocate(int p_length);
  // Remove all occurrences of char
  int       Remove(char p_char);
  // Replace a string or a character
  int       Replace(PCSTR p_old,PCSTR p_new);
  int       Replace(char  p_old,char  p_new);
  // Find last occurrence of a char
  int       ReverseFind(char p_char);
  // Get substring from the right 
  String    Right(int p_length);
  // Set char at a position
  void      SetAt(int p_index,char p_char);
  // SetString interface
  void      SetString(PCSTR p_string);
  void      SetString(PCSTR p_string,int p_length);
  // Set string from a COM BSTR
  BSTR      SetSysString(BSTR* p_string);
  // Leftmost string (not) in argument
  String    SpanExcluding(PCSTR p_string);
  String    SpanIncluding(PCSTR p_string);
  // Length of the string
 static int StringLength (PCSTR p_string);
  // Return tokenized strings
  String    Tokenize(PCSTR p_tokens,int& p_curpos);
  // Trim the string
  String&   Trim();
  String&   Trim(char  p_char);
  String&   Trim(PCSTR p_string);
  String&   TrimLeft();
  String&   TrimLeft(char  p_char);
  String&   TrimLeft(PCSTR p_string);
  String&   TrimRight();
  String&   TrimRight(char  p_char);
  String&   TrimRight(PCSTR p_string);
  // Truncate the string
  void      Truncate(int p_length);
  // Unlocking the buffer
  void      UnlockBuffer();

  // OPERATORS

  operator char*();
  operator const char*();
  String   operator+ (String& p_extra);
  String&  operator+=(String& p_extra);

private:
//   In CString these are in StringData 
//   We do not use it here, as we do not use the same locking scheme
//   in a std::string derived class
//
//   void      AddRef();
//   bool      IsLocked();
//   bool      IsShared();
//   void      Release();
//   void      Lock();
//   void      Unlock();
// 
//   // Extra data
//   long      m_references;
};

//////////////////////////////////////////////////////////////////////////
//
// INLINING FOR PERFORMANCE
// All methods consisting of one line of code are inlined
//
//////////////////////////////////////////////////////////////////////////

inline void String::Append(PCTSTR p_string)
{
  append(p_string);
}

inline void String::AppendChar(char p_char)
{
  push_back(p_char);
}

inline void String::AnsiToOem()
{
  // Only works for MBCS, not for Unicode
  ::CharToOemBuff(c_str(),(LPTSTR)c_str(),length());
}

inline int String::Collate(PCTSTR p_string)
{
  return strcoll(c_str(),p_string);
}

inline int String::CollateNoCase(PCTSTR p_string)
{
  return _stricoll(c_str(),p_string);
}

inline int String::Compare(PCTSTR p_string)
{
  return strcmp(c_str(),p_string);
}

inline int String::CompareNoCase(PCTSTR p_string)
{
  return _stricmp(c_str(),p_string);
}

inline void String::Empty()
{
  clear();
}

inline int String::Find(PCTSTR p_string,int p_start /*= 0*/)
{
  // Returns string::npos (-1) or position
  return find(p_string,p_start);
}

inline int String::Find(char p_char,int p_start /*= 0*/)
{
  return find(p_char,p_start);
}

inline int String::FindOneOf(PCTSTR p_string)
{
  // Find on of the chars or -1 for not found
  return find_first_of(p_string,0);
}

inline void String::FreeExtra()
{
  shrink_to_fit();
}

inline int String::GetLength()
{
  return (int)length();
}

inline PCSTR String::GetString()
{
  return c_str();
}

inline int String::GetAt(int p_index)
{
  return at(p_index);
}

inline unsigned  String::GetAllocLength()
{
  return (unsigned)capacity();
}

inline bool String::IsEmpty()
{
  return empty();
}

inline BOOL String::LoadString(HINSTANCE p_inst,UINT p_strID)
{
  return LoadString(p_inst,p_strID,0);
}

inline void String::MakeReverse()
{
  _strrev((char*)c_str());
}

inline String String::Mid(int p_index)
{
  return String(substr(p_index));
}

inline String String::Mid(int p_index,int p_length)
{
  return String(substr(p_index,p_length));
}

inline void String::OemToAnsi()
{
  // Only works for MBCS, not for Unicode
  ::OemToCharBuff(c_str(),(LPTSTR)c_str(),length());
}

inline void String::Preallocate(int p_length)
{
  reserve(p_length);
}

inline int String::ReverseFind(char p_char)
{
  return find_last_of(p_char);
}

inline String& String::Trim()
{
  return TrimLeft().TrimRight();
}

inline String& String::Trim(char p_char)
{
  return TrimLeft(p_char).TrimRight(p_char);
}

inline String& String::Trim(PCSTR p_string)
{
  return TrimLeft(p_string).TrimRight(p_string);
}

inline String& String::TrimLeft()
{
  return TrimLeft(' ');
}

inline String& String::TrimRight()
{
  return TrimRight(' ');
}

inline void String::Truncate(int p_length)
{
  erase(p_length);
}

inline void String::UnlockBuffer()
{
  // Unlock();
}

//////////////////////////////////////////////////////////////////////////
//
// StringData counterparts
//
//////////////////////////////////////////////////////////////////////////

// inline bool String::IsLocked()
// {
//   return m_references < 0;
// }
// 
// inline bool String::IsShared()
// {
//   return m_references > 0;
// }

