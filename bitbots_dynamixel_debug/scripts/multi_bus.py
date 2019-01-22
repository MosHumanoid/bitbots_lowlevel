import subprocess
#subprocess.Popen(["rosrun bitbots_dynamixel_debug ping.py", "1", "--port", "ttyUSB0"])
import os
#os.spawnl(os.P_NOWAIT, "rosrun bitbots_dynamixel_debug ping.py 1 --port ttyUSB0")
import multiprocessing
from bitbots_dynamixel_debug.connector import MultiConnector


c = MultiConnector(2, ["/dev/ttyUSB4".encode('utf-8'), "/dev/ttyUSB5".encode('utf-8'), "/dev/ttyUSB6".encode('utf-8'), "/dev/ttyUSB7".encode('utf-8')], 2000000)#4615384)
jobs = []

"""for i in range(0,4):    
    p = multiprocessing.Process(target=c.remove_return_delay_time, args=(i, 56, True))
    jobs.append(p)
"""
ids =[]
ids.append([1, 2, 3, 4, 5])
ids.append([6, 7, 8, 9, 10])
ids.append([11, 12, 13, 14, 15])
ids.append([16, 18, 19, 20])

for i in range(0,4):
    p = multiprocessing.Process(target=c.sync_read_loop, args=(i, ids[i], 9, 1, True))
    jobs.append(p)

for i in range(0,4):
    jobs[i].start()

c.closePort()