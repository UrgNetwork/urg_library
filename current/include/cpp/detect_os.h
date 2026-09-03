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
// \~japanese Windows 系 OS を検出
// \~english Detects the Windows family operating system
#define QRK_WINDOWS_OS

#if defined(_MSC_VER) || defined(__BORLANDC__)
// \~japanese Microsoft C/C++ 系コンパイラ
// \~english Microsoft C/C++ compiler
#define QRK_MSC
#elif defined __CYGWIN__
// \~japanese Cygwin 上でのビルド
// \~english Build under Cygwin
#define QRK_CYGWIN
#elif defined __MINGW32__
// \~japanese MinGW 上でのビルド
// \~english Build under MinGW
#define QRK_MINGW
#endif

#elif defined __linux__
// \~japanese Linux を検出
// \~english Detects Linux
#define QRK_LINUX_OS

#else
// \~japanese 検出できないときを、Mac 扱いにしてしまう
// \~english If cannot detect the OS, assumes it is a Mac
#define QRK_MAC_OS
#endif

#endif /* !QRK_DETECT_OS_H */
