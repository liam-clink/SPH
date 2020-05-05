import matplotlib.pyplot as plt
import numpy as np

#inside = np.loadtxt('inside.dat')
#outside = np.loadtxt('outside.dat')
vertices = np.loadtxt('vertices.dat')
boundary = np.loadtxt('boundary.dat')

print(vertices)

plt.scatter(boundary[:,0],boundary[:,1],s=1)
#plt.scatter(inside[:,0],inside[:,1],s=1)
#plt.scatter(outside[:,0],outside[:,1],s=1)
plt.plot(vertices[0,:],vertices[1,:])
plt.savefig('inside_outside_test.png',dpi=300)
