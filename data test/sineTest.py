import re
from matplotlib import pylab
from pylab import plot, show
from scipy.fftpack import fft, ifft
import numpy as np

myList = list(range(1000));


x = np.array(np.sin(myList))

y = fft(x)

plot(y)                     # draw the graph
show()
