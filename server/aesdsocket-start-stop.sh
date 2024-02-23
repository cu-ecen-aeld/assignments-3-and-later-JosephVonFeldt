#!/bin/bash

if [ "$1" = "start" ];
  then
    echo "Starting"
    start-stop-daemon -S -n aesdsocket -a /usr/bin/aesdsocket -d
    exit
fi



if [ "$1" = "stop" ];
  then
    echo "Ending"
    start-stop-daemon -K -n aesdsocket
    exit
fi

exit 1