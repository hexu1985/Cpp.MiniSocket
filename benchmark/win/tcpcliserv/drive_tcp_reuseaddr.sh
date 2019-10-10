#!/usr/bin/env bash

SRV_PORT=9999

./tcpserv $SRV_PORT &
SRV_PID=$!

sleep 1

./tcpcli 127.0.0.1 $SRV_PORT <<EOF
hello
world
bye
EOF

kill $SRV_PID
