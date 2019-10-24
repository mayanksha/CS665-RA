import matplotlib.pyplot as plt
import numpy as np
import sys

if (len(sys.argv) != 2):
    print("2 Arguments must be given!")
    sys.exit (-1)

a = np.loadtxt(sys.argv[1])
# print("L3 hits = "+str(a[-1]))
# a=a[:-1]
# nopages=len(a)/31

from matplotlib.ticker import FormatStrFormatter

fig, ax = plt.subplots()
#counts, bins, patches = ax.hist(a,bins=16, range=(1, 64), facecolor='yellow', edgecolor='gray')
counts, bins, patches = ax.hist(a,facecolor='yellow', edgecolor='gray')
print(counts)
print(bins)
# Set the ticks to be at the edges of the bins.
ax.set_xticks(bins)
# Set the xaxis's tick labels to be formatted with 1 decimal place...
ax.xaxis.set_major_formatter(FormatStrFormatter('%0.1f'))

# Change the colors of bars at the edges...
twentyfifth, seventyfifth = np.percentile(a, [25, 75])
for patch, rightside, leftside in zip(patches, bins[1:], bins[:-1]):
    if rightside < twentyfifth:
        patch.set_facecolor('green')
    elif leftside > seventyfifth:
        patch.set_facecolor('red')

# Label the raw counts and the percentages below the x-axis...
bin_centers = bins
for count, x in zip(counts, bin_centers):
    # Label the raw counts
    ax.annotate(str(count), xy=(x, 0), xycoords=('data', 'axes fraction'),
        xytext=(0, -18), textcoords='offset points', va='top', ha='center')

    # Label the percentages
    percent = '%0.0f%%' % (100 * float(count) / counts.sum())
    ax.annotate(percent, xy=(x, 0), xycoords=('data', 'axes fraction'),
        xytext=(0, -32), textcoords='offset points', va='top', ha='center')


# Give ourselves some more room at the bottom of the plot
plt.subplots_adjust(bottom=0.15)
plt.title("Frequency of Evicted Block Number When eviction is exactly 1")
plt.savefig(sys.argv[1] + ".png")
#plt.show()