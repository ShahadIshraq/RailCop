#   First_graph.py
#   Authour: Alan Richmond, Python3.codes

import re
from matplotlib import pylab
from pylab import plot, show, bar

f = open("test1", "r") 
myList = []

for line in f:
   if line.strip():           # line contains eol character(s)
       myList.append( int(line) )         # assuming single integer on each line

f.close()



#y = [3,5,9,2,6,4,7,8,1,5]   # a list of numbers
plot(myList)                     # draw the graph
show()                      # show it to me!
 
'''   for a barchart replace plot(y) with these:
x = [i for i in range(10)]
bar(x,y)
'''
