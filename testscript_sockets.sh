for MS in 4 16 64 256 1024 4096 16384 65536 262144 524288
do
	taskset 0x10 sockets_server $MS &
	sleep 1 
	taskset 0x1 socket_client $MS
	sleep 70
	pkill -f socket_client
	pkill -f sockets_server
done  
