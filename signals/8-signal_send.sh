#!/bin/bash

# Check if the correct number of arguments is provided
if [ $# -ne 1 ]; then
	echo "Usage: $0 <pid>"
	exit 1
fi

# Assign the provided PID to the 'pid' variable
pid="$1"

# Check if the specified PID is valid
if ! kill -0 "$pid" 2>/dev/null; then
	echo "Invalid PID: $pid"
	exit 1
fi

# Send the SIGQUIT signal to the target process
kill -QUIT "$pid"
