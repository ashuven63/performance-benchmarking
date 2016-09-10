for MS in 4 16 64 256 1024 4096 16384 65536 262144 524288
do 
	taskset 0x1 pipe $MS
done  
