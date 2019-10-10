#!/usr/bin/env bash

SRV_PORT=$(($RANDOM + 1024))

./tcpserv_byname "::" $SRV_PORT &
SRV_PID=$!

sleep 1

./tcpcli_byname "::1" $SRV_PORT <<EOF
hello
world
bye
EOF

kill $SRV_PID
