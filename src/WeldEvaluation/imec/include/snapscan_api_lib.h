/////////////////////////////////////////////////////
//
//  Date: 9 April, 2018
//
//  Authors:
//    Wouter Charle
//
//  Copyright 2018 imec. All rights reserved.
//
////////////////////////////////////////////

#ifndef SNAPSCAN_API_LIB_H
#define SNAPSCAN_API_LIB_H


#if defined(_MSC_VER) || defined(__CYGWIN__)
// MSVC and CYGWIN compilers

// import definitions using SNAPSCAN_API and SNAPSCAN_API_LOCAL
# define SNAPSCAN_API __declspec(dllimport)
# define SNAPSCAN_API_LOCAL

// automatic linkage
# ifndef SNAPSCAN_NO_AUTO_LINK
//  link to import library
#   pragma comment(lib,"snapscan_api.lib")
# endif

#else
// not MSVC compiler
# define SNAPSCAN_API
# define SNAPSCAN_API_LOCAL
#endif


#endif // SNAPSCAN_API_LIB_H
