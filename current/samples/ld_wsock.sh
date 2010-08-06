#!/bin/sh

if test x$MSYSTEM == x"MINGW32"; then
  echo "-lwsock32"
fi
