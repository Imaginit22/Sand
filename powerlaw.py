import matplotlib.pyplot as plt
import numpy as np

sizes = np.loadtxt('topples.txt', dtype=int)
print(sizes[0:10])
sizes = np.array(list(map(int, sizes)))
sizes, counts = np.unique(sizes, return_counts=True)

plt.figure()
plt.loglog(sizes, counts, 'o')  
plt.xlabel('sizes')
plt.ylabel('counts')
plt.title('Log-Log Plot to Check for Power Law')
plt.grid(True)
plt.show()