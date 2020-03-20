#!/usr/bin/env python3
from gpiozero import Button
import signal
import subprocess
import os, sys

offGPIO = int(sys.argv[1]) if len(sys.argv) >= 2 else 21
holdTime = int(sys.argv[2]) if len(sys.argv) >= 3 else 3

# start subprocess for actual code
devnull = open('/dev/null', 'w')
p = subprocess.Popen(["./run"], stdout=devnull, shell=False)
pid = p.pid

# the function called to shut down the RPI
def shutdown():
	os.kill(pid, signal.SIGINT) # ctrl c on actual code
	# os.system("shutdown 1")     # shuts pi down in 1 min

btn = Button(offGPIO, hold_time=holdTime)
btn.when_held = shutdown
signal.pause()    # handle the button presses in the background

