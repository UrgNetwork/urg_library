#ifndef QRK_DETECT_OS_H
#define QRK_DETECT_OS_H

/*!
  \file
  \~japanese
  \brief 動作OSの検出
  \~english
  \brief Detects the operating system
  \~
  \author Satofumi KAMIMURA

  $Id$
*/

#if defined(_WIN32)
#define QRK_WINDOWS_OS

#if defined(_MSC_VER) || defined(__BORLANDC__)
#define QRK_MSC
#elif defined __CYGWIN__
#define QRK_CYGWIN
#elif defined __MINGW32__
#define QRK_MINGW
#endif

#elif defined __linux__
#define QRK_LINUX_OS

#else
// \~japanese 検出できないときを、Mac 扱いにしてしまう
// \~english If cannot detect the OS, assumes it is a Mac
#define QRK_MAC_OS
#endif

#endif /* !QRK_DETECT_OS_H */
