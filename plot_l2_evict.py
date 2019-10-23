import matplotlib.pyplot as plt
import numpy as np
import sys

if (len(sys.argv) != 2):
    print("2 Arguments must be given!")
    sys.exit (-1)

a = np.loadtxt(sys.argv[1])
print("L3 hits = "+str(a[-1]))
a=a[:-1]
plt.hist(a, bins=np.arange(0, 500, 20))
plt.title(sys.argv[1] + " MB")
plt.savefig(sys.argv[1] + ".png")
