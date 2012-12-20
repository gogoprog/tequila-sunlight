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
    print( "[tequila-sunlight] Failed to parse json" )
    sys.exit(3)

jobResults = {
    'blue': 0,
    'blue_anime': 0,
    'red': 0,
    'red_anime': 0,
    'disabled': 0,
    'aborted': 0,
    'yellow': 0,
    'yellow_anime': 0,
    'aborted': 0,
    'aborted_anime': 0
}

def sendCommand( cmd ):
    try:
        urllib.request.urlopen( daemonUrl + cmd )
        print("[tequila-sunlight] Sent : " + cmd)
    except:
        print("[tequila-sunlight] Error while sending to daemon")
        sys.exit(5)

if buildStatusJson["jobs"]:
    for job in buildStatusJson["jobs"]:
        jobResults[job['color']] += 1

    if jobResults['red'] + jobResults['red_anime'] > 0:
        sendCommand("RED/ON")
        sendCommand("GREEN/OFF")
    else:
        sendCommand("RED/OFF")
        sendCommand("GREEN/ON")

    runningJobs = jobResults['blue_anime'] + jobResults['red_anime'] + jobResults['yellow_anime']

    if runningJobs > 0:
        sendCommand("ORANGE/BLINK/" + str(20 - runningJobs))
    else:
        sendCommand("ORANGE/STOP_BLINK")
else:
    sys.exit(4)

print( "[tequila-sunlight] Done" )
sys.exit(0)