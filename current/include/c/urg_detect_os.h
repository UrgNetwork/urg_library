#ifndef URG_DETECT_OS_H
#define URG_DETECT_OS_H

/*!
  \file
  \~japanese
  \brief OS の検出
  \~english
  \brief Detects the current OS
  \~
  \author Satofumi KAMIMURA

  $Id$
*/

#if defined(_WIN32)
#define URG_WINDOWS_OS

#if defined(_MSC_VER) || defined(__BORLANDC__)
#define URG_MSC
#endif

#elif defined(__linux__)
#define URG_LINUX_OS

#else
// \~japanese 検出できないときを、Mac 扱いにしてしまう
// \~english If cannot detect the OS, assumes it is a Mac
#define URG_MAC_OS
#endif

#endif /* !URG_DETECT_OS_H */
