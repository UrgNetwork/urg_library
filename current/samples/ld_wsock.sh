#!/bin/sh

if test $MSYSTEM == "MINGW32"; then
  echo "-lwsock32"
fi
