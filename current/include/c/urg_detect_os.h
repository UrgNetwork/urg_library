#ifndef URG_DETECT_OS_H
#define URG_DETECT_OS_H

/*!
  \file
  \brief OS ‚ÌŒŸo

  \author Satofumi KAMIMURA

  $Id: urg_detect_os.h,v 0caa22c18f6b 2010/12/30 03:36:32 Satofumi $
*/

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__)
#define URG_WINDOWS_OS

#if defined(_MSC_VER)
#define URG_MSC
#endif

#elif defined(__linux__)
#define URG_LINUX_OS

#else
// ŒŸo‚Å‚«‚È‚¢‚Æ‚«‚ğAMac ˆµ‚¢‚É‚µ‚Ä‚µ‚Ü‚¤
#define URG_MAC_OS
#endif

#endif /* !URG_DETECT_OS_H */
