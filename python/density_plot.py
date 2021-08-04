import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('density.tsv',skiprows=3)

plt.imshow(data)
plt.show()