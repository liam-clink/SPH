import matplotlib.pyplot as plt
import numpy as np
directory = './verification/boundary'
inside = np.loadtxt(directory+'/round_2/inside.dat')
outside = np.loadtxt(directory+'/round_2/outside.dat')
vertices = np.loadtxt(directory+'/round_1/vertices.dat')
boundary = np.loadtxt(directory+'/round_1/boundary.dat')

print(vertices)

plt.scatter(boundary[:,0],boundary[:,1],s=1)
plt.scatter(inside[:,0],inside[:,1],s=1)
plt.scatter(outside[:,0],outside[:,1],s=1)
plt.plot(np.append(vertices[0,:], vertices[0,0]), np.append(vertices[1,:], vertices[1,0]))
plt.savefig(directory+'inside_outside_test.png',dpi=300)
