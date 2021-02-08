#!/usr/bin/env bash

SRV_PORT=$(($RANDOM + 1024))

./udpserv_byname "0.0.0.0" $SRV_PORT &
SRV_PID=$!

sleep 1

./udpcli_byname "127.0.0.1" $SRV_PORT <<EOF
hello
world
bye
EOF

kill $SRV_PID
