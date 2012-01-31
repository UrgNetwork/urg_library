#!/bin/sh

if [ "${OS}" != "Windows_NT" -a "#${GROUP}" = "" ] ; then
  echo "-lrt"
fi
