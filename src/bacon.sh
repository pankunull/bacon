#!/usr/bin/env bash

if [ -z "$3" ]; then
	printf "Usage: %s <msg-agent> <url> <delay['o' for one-shot]>\n" "$0"
	exit 1
fi

AGENT="$1"
URL="$2"
DELAY="$3"

while true; do
	printf "Sending beacon [%s] " "$(date)"
	if curl -s -k -A "$AGENT" -- "$URL" 1>/dev/null; then
		printf "sent!\r"
	else
		printf "error!\r"
	fi
	echo
	if [ "$DELAY" = 'o' ]; then
		break
	fi
	sleep "$DELAY"
done
