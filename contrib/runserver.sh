#!/usr/bin/env bash

if [ ! -x server ];
then
	echo "usage: $0"
	echo "	run this in the project root directory"
	exit 1
fi

clear

onexit() {
	kill -- -$serverpid
	exit
}

trap onexit INT

./server 8081 &
serverpid=$!
2>/dev/null inotifywait ./server
kill -- -$serverpid

clear
echo autoreloading...
sleep 1

exec $0
