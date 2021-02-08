#!/usr/bin/env bash

SRV_PORT=$(($RANDOM + 1024))
./daytimetcpsrv_byname 0.0.0.0 $SRV_PORT &
SRV_PID=$!
sleep 1
./daytimetcpcli_byname 127.0.0.1 $SRV_PORT
kill $SRV_PID
