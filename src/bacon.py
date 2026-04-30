#!/usr/bin/env python3

import sys
import time
import requests
from datetime import datetime

def usage():
    print(f"Usage: {sys.argv[0]} <msg-agent> <url> <delay|'o' for one-shot>")
    sys.exit(1)

if len(sys.argv) < 4:
    usage()

agent = sys.argv[1]
url = sys.argv[2]
delay = sys.argv[3]

session = requests.Session()
session.headers.update({"User-Agent": agent})

while True:
    print(f"Sending beacon [{datetime.now()}] ", end="")

    try:
        r = session.get(url, verify=False, timeout=3)
        print(r.status_code)
        print(r.text[:200])
        if r.ok:
            print("sent!")
        else:
            print("error!")
    except Exception:
        print("error!")

    if delay == "o":
        break

    time.sleep(int(delay))
