#!/usr/bin/env bash

SRV_PORT=$(($RANDOM + 1024))
./daytimetcpsrv $SRV_PORT &
SRV_PID=$!
sleep 1
./daytimetcpcli 127.0.0.1 $SRV_PORT
kill $SRV_PID
