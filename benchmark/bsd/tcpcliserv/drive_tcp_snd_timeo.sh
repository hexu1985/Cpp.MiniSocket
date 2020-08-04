#!/usr/bin/env bash

SRV_PORT=$(($RANDOM + 1024))

./tcpserv_snd_timeo $SRV_PORT &
SRV_PID=$!

sleep 1

./tcpcli_snd_timeo 127.0.0.1 $SRV_PORT 

kill $SRV_PID

