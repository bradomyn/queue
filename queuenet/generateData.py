#!/usr/bin/env python

import random

#> prio	frequency		payload
#> 7 		1 ms -> 100 us 	64 -> 1500 bytes
#> 6 		1 sec -> 1ms	64 -> 1500 bytes
#> 5 		1 sec -> 1ms	64 -> 1500 bytes
#> 4 		1 sec -> 1ms	64 -> 1500 bytes
#> 3 		2 sec -> 1ms	64 -> 1500 bytes
#> 2 		3 sec -> 1ms	64 -> 1500 bytes
#> 1 		3 sec -> 1ms	64 -> 1500 bytes
#> 0 		4 sec -> 1ms	64 -> 1500 bytes


# time frequency 'normed' to 100 us

time = 0
data = [
        {
            "prio": 7,
            "min": 1,
            "max": 10,
            "value": 0,
            "payload": 64,
            "payload_min": 64,
            "payload_max": 1500
        },
        {
            "prio": 6,
            "min": 10,
            "max": 10*1000,
            "value": 0,
            "payload": 64,
            "payload_min": 64,
            "payload_max": 1500
        },
		{
            "prio": 5,
            "min": 10,
            "max": 10*1000,
            "value": 0,
            "payload": 64,
            "payload_min": 64,
            "payload_max": 1500
        },
		{
            "prio": 4,
            "min": 10,
            "max": 10*1000,
            "value": 0,
            "payload": 64,
            "payload_min": 64,
            "payload_max": 1500
        },
		{
            "prio": 3,
            "min": 20,
            "max": 10*1000,
            "value": 0,
            "payload": 64,
            "payload_min": 64,
            "payload_max": 1500
        },
		{
            "prio": 2,
            "min": 30,
            "max": 10*1000,
            "value": 0,
            "payload": 64,
            "payload_min": 64,
            "payload_max": 1500
        },
		{
            "prio": 1,
            "min": 30,
            "max": 10*1000,
            "value": 0,
            "payload": 64,
            "payload_min": 64,
            "payload_max": 1500
        },
		{
            "prio": 0,
            "min": 40,
            "max": 10*1000,
            "value": 0,
            "payload": 64,
            "payload_min": 64,
            "payload_max": 1500
        }
    ]
     
f = open('data.txt', 'a')
def step(time, data):
    for packet in data:
        if packet["value"] == 0:
            f.write("%s %s\n" % (packet["prio"], packet["payload"]))
            print "%010d paket %s generated, payload %s" % (time, packet["prio"], packet["payload"])
            packet["value"] = random.randint(packet["min"], packet["max"])
            packet["payload"] = random.randint(packet["payload_min"], packet["payload_max"])
        else:
            packet["value"] = packet["value"] - 1
    return (time+1, data)

        
for i in range(10000):
    time, data = step(time, data)

