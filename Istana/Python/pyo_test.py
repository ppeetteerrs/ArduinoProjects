from pyo import *
import time
from os import path
import os
from sys import stdin
from random import choice

# Start Server
s = Server(buffersize=1024).boot()
s.start()

# # Setup Files
sf_paths = [path.join(os.getcwd(), "sounds/{}.wav".format(i+1)) for i in range(6)]
tables = [SndTable(file_path) for file_path in sf_paths]
outputs = [None for i in range(6)]

while True:
    option1 = choice(range(500))
    option2 = choice(range(500))

    for i in range(6):
        if option1 == i or option2 == i:
            outputs[i] = Looper(tables[i], pitch=1, mode=0, mul=10).out()
    time.sleep(0.01)
