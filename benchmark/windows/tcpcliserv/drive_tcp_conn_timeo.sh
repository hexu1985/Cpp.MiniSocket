#!/usr/bin/env bash

SRV_PORT=$(($RANDOM + 1024))

./tcpserv_conn_timeo $SRV_PORT &
SRV_PID=$!

sleep 1

for i in {1..5}
do
    ./tcpcli_conn_timeo 127.0.0.1 $SRV_PORT &
done

./tcpcli_conn_timeo 127.0.0.1 $SRV_PORT 

kill $SRV_PID
#killall tcpcli_conn_timeo 
ps -ef | grep tcpcli_conn_timeo | awk '{print $2}' | xargs kill -9
