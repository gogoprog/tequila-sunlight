#!/usr/bin/python
# tequila-sunlight-updater script :
#   Retrieve jobs status and send command to the daemon

import json
import sys
import urllib
import urllib.request
import urllib.error

jenkinsUrl = "http://builder:8080/api/json/"
daemonUrl = "http://localhost:8080/"

print( "[tequila-sunlight] Starting update" )

try:
    stream = urllib.request.urlopen( jenkinsUrl )
except urllib.error.URLError as e:
    print( "URL Error: " + str(e.code) )
    sys.exit(2)

try:
    response = stream.read().decode("utf-8")
    buildStatusJson = json.loads( response )
except:
    print( "Failed to parse json" )
    sys.exit(3)

if buildStatusJson["jobs"]:
    for job in buildStatusJson["jobs"]:
        print(job['name'] + ":" + job['color'])
else:
    sys.exit(4)

print( "[tequila-sunlight] Done" )
sys.exit(0)