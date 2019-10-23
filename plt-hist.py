import matplotlib.pyplot as plt
import numpy as np
import sys

if (len(sys.argv) != 2):
    print("2 Arguments must be given!")
    sys.exit (-1)

a = np.loadtxt(sys.argv[1])
plt.hist(a, bins=np.arange(0, 400, 10))
plt.title(sys.argv[1] + " MB")
plt.savefig(sys.argv[1] + ".png")
