#!/usr/bin/python3.5

import matplotlib.pyplot as plt
import matplotlib.animation as animation
import os, errno
import time
import json
fig = plt.figure()

stat = open("stat", "r")
all_of_it = stat.read()
 
# close the file
stat.close()

fig.suptitle(all_of_it, fontsize=14, fontweight='bold')
ax1 = fig.add_subplot(1,1,1)


f = open("graph", "r")
pullData = f.readline()




def animate(i):
    
    pullData = f.readline()
    dataArray = pullData.split(' ')
   

    xar = []
    yar = []
    for eachLine in dataArray:
        if len(eachLine)>1:
            x,y = eachLine.split(',')
            xar.append(float(x))
            yar.append(float(y))
    #print(xar)
    #print(yar)        
    #print('\n')
    ax1.clear()
    ax1.plot(xar,yar)
    #ax1.text(3, 2, 'unicode: Institut für Festkörperphysik')

if __name__ == '__main__':
    #ani = animation.FuncAnimation(fig, animate, interval=1000)
    ani = animation.FuncAnimation(fig, animate, init_func=init, frames=200, interval=20, blit=True)
    plt.show()
     
#    dict = {'Python' : '.py', 'C++' : '.cpp', 'Java' : '.java'}
#    for key in dict.keys():
#        print(key)
#        print(dict[key])
#    json = json.dumps(dict)
#    f = open("dict.json","w")
#    f.write(json)
#    f.close()

