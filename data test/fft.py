import re
from matplotlib import pylab
from pylab import plot, show
from scipy.fftpack import fft, ifft
import numpy as np

f = open("test1", "r") 
myList = []

for line in f:
   if line.strip():           # line contains eol character(s)
       myList.append( int(line) )         # assuming single integer on each line

f.close()

x = np.array(myList[25000:30000])

y = fft(x)

plot(y)                     # draw the graph
show()
