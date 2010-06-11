#ifndef URG_ETHERNET_H
#define URG_ETHERNET_H

/*!
  \file
  \brief イーサーネット通信

  \author Satofumi KAMIMURA

  $Id$
*/

#include "urg_ethernet_t.h"


int ethernet_open(ethernet_t *ethernet, const char *address, long port);

// !!!

#endif /* !URG_ETHERNET_H */
