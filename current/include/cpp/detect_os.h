#ifndef QRK_DETECT_OS_H
#define QRK_DETECT_OS_H

/*!
  \file
  \brief “®ìOS‚ÌŒŸo

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
// ŒŸo‚Å‚«‚È‚¢‚Æ‚«‚ğAMac ˆµ‚¢‚É‚µ‚Ä‚µ‚Ü‚¤
#define QRK_MAC_OS
#endif

#endif /* !QRK_DETECT_OS_H */
