#ifndef URG_DETECT_OS_H
#define URG_DETECT_OS_H

/*!
  \file
  \brief OS の検出

  \author Satofumi KAMIMURA

  $Id$
*/


#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__)
#define URG_WINDOWS_OS

#if defined(_MSC_VER)
#define URG_MSC
#endif

#elif defined(__linux__)
#define URG_LINUX_OS

#else
// 検出できないときを、Mac 扱いにしてしまう
#define MAC_OS
#endif

#endif /* !URG_DETECT_OS_H */
