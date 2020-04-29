// atlssmisc.h 
//
// Use class CStdString as WTL::CString: 
//		#include this file instead of <atlmisc.h> if you want WTL::CString to be a  
//		CStdString deriving from std::basic_string<TCHAR>.
//
// Copyright (c) Alain Rist 2006
//
// The use and distribution terms for this software are covered by the
// Common Public License 1.0 (http://opensource.org/licenses/cpl.php).
// By using this software in any fashion, you are agreeing to be bound by
// the terms of this license. You must not remove this notice, or
// any other, from this software.
//
//
// Notes: 
//		atlssmisc.h should be included first of WTL headers after atlwin.h.
//		StdString.h and this file must be in the same directory or allow: #include "StdString.h".
//		VS2005 and VCExpress projects should '#define _CRT_SECURE_NO_DEPRECATE' before '#include <atlbase.h>'
//		eVC4 projects should compile with /GX to enable unwind semantics for C++ exception handler.

///////////////////////////////////////////////////////////////////////////////////
// Recommended stdafx.h layout
/*
// stdafx.h 
//...
// Change these values to use different versions
//...

#define _CRT_SECURE_NO_DEPRECATE
#include <atlbase.h>

//#include <atlstr.h>  // uncomment for WTL ATL::CString support
//#define _WTL_NO_CSTRING // uncomment for WTL ATL::CString support or no WTL CString suppport
//#define _WTL_USE_CSTRING // uncomment for full WTL WTL::CString support

#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

//#include <atlmisc.h> // uncomment AND comment next line for original WTL:CString defintion
#include <atlssmisc.h> // CStdString based WTL:CString defintion
//...
*/
///////////////////////////////////////////////////////////////////////////////////

#ifndef __ATLSSMISC_H__
	#define __ATLSSMISC_H__

#pragma once

#if _WTL_VER > 0x750
	#error atlssmisc.h requires WTL version under or equal to 7.5
#endif

#ifdef __ATLMISC_H__
	#error atlssmisc.h cannot be included after atlmisc.h
#endif

#if defined(_WTL_USE_CSTRING) && (defined(_ATL_TMP_NO_CSTRING) || defined(__ATLSTR_H__) || defined(_WTL_NO_CSTRING))
	#error Incompatible '#define xxSTRxx's found.
#endif 

// push CStdString in global namespace
#include "StdString.h"	

#if defined(_ATL_TMP_NO_CSTRING) || defined(__ATLSTR_H__) // project uses ATL::CString
	#define _WTL_NO_CSTRING
#endif // defined(_ATL_TMP_NO_CSTRING) || defined(__ATLSTR_H__)

#ifdef _WTL_USE_CSTRING
	#undef _WTL_USE_CSTRING 
	#define _WTL_USE_CSTRING 0
#endif

#if !defined(_WTL_USE_CSTRING) && !defined(_WTL_NO_CSTRING)
	#define _WTL_USE_CSTRING 1
#endif // !defined(_WTL_USE_CSTRING) && !defined(_WTL_NO_CSTRING)

#ifdef _WTL_USE_CSTRING
	#define _WTL_USE_STDSTRING

	namespace WTL
	{
	#if _WTL_USE_CSTRING == 1	// class CString not forward declared
		// push ::CStdString as WTL::CString
		typedef ::CStdString CString;
	#else						// atlapp.h may have declared class CString;
		// provide a class CString derived from ::CStdString
		class CString : public ::CStdString
		{
		// These typedefs are private to CStdStr so are their CString equivalents
			typedef ::CStdString				MYBASE;	 // my base class 
			typedef CString						MYTYPE;	 // myself
			typedef  MYBASE::const_iterator		MYCITER; // you get the idea...
			typedef  MYBASE::size_type			MYSIZE;   
			typedef  MYBASE::value_type			MYVAL; 
			typedef  MYBASE::allocator_type		MYALLOC;
		public:
		// CStdStr constructors propagated to CString
			CString() : ::CStdString(){}
			CString(const MYTYPE& str) : ::CStdString(str){}
			CString(const std::string& str) : ::CStdString(str){}
			CString(const std::wstring& str) : ::CStdString(str){}
			template <typename TC> 
			CString(const TC* pTC, MYSIZE n) : ::CStdString(pTC, n){}
			CString(PCUSTR pU) : ::CStdString(pU){}
			CString(PCSTR pA) : ::CStdString(pA){}
			CString(PCWSTR pW) : ::CStdString(pW){}
			CString(MYCITER first, MYCITER last) : ::CStdString(first, last){}
			CString(MYSIZE nSize, MYVAL ch, const MYALLOC& al=MYALLOC()) : ::CStdString(nSize, ch, al){}
	#ifdef SS_INC_COMDEF
			CString(const _bstr_t& bstr) : ::CStdString(bstr){}
	#endif // SS_INC_COMDEF
		};
	#endif // _WTL_USE_CSTRING == 1
	}; // Namespace WTL

	// prepare inclusion of atlmisc.h without WTL::CString
		#undef _WTL_USE_CSTRING
		#define _WTL_NO_CSTRING
		#define __ATLSTR_H__ // for CFindFile and CRecentDocumentList CString support
#endif // _WTL_USE_CSTRING

#include<atlmisc.h>

#ifdef _WTL_USE_STDSTRING
// set back proper macro definitions
	#undef __ATLSTR_H__
	#undef _WTL_NO_CSTRING
	#define _WTL_USE_CSTRING
#endif // _WTL_USE_STDSTRING

#endif // __ATLSSMISC_H__
