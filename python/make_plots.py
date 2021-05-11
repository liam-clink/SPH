import matplotlib.pyplot as plt
import numpy as np
import os

frame_count = len([name for name in os.listdir('./data/positions/')])
print(frame_count)
pad_length = len(os.listdir('./data/positions/')[0].split('.')[0])

os.system('rm frames/*')
os.system('mkdir -p frames')

for i in range(frame_count):
    file_name = str(i).zfill(pad_length)
    data = np.loadtxt("./data/positions/"+file_name+".csv", delimiter=',')
    plt.scatter(data[:,1],data[:,2])
    plt.savefig("frames/"+file_name+".png",dpi=300)
    plt.clf()

