#!/usr/bin/env sh

# This script asserts that the server and the client do not attempt to bufferize
# a whole file before writing / transmitting it.
# This wrong behavior might be problematic when trying to transmit huge - bigger
# than the RAM size.

set -e

if [ ! -x client ] && [ ! -x serveur ];
then
	echo "usage: $0"
	echo "	run this in the project root directory"
	exit 1
fi


trap "kill $server_pid $client_pid; rm -Rf tmp null urandom" EXIT
mkdir -p tmp
ln -s /dev/null
ln -s /dev/urandom tmp/null

server_pid=$(./serveur 8080 &)
client_pid=$(./client localhost 8080 & << EOF
get tmp/null
EOF
)
fg 
