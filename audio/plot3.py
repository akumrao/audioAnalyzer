#!/usr/bin/python3.5

import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation, rc


# animate over some set of x, y
x = np.linspace(-4, 4, 100)
y = np.sin(x)

# First set up the figure, the axes, and the plot element
fig, ax = plt.subplots()
plt.close()
ax.set_xlim(( -4, 4))
ax.set_ylim((-2, 2))
line1, = ax.plot([], [], lw=2)
line2, = ax.plot([], [], lw=2)

# initialization function: plot the background of each frame
def init():
    line1.set_data(x, y)      
    return (line1,)
  
# animation function: this is called sequentially
def animate(i):
  at_x = x[i]
  
  # gradient_line will have the form m*x + b
  m = np.cos(at_x)
  b = np.sin(at_x) - np.cos(at_x)*at_x
  gradient_line = m*x + b
  
  line2.set_data(x, gradient_line)
  return (line2,)

anim = animation.FuncAnimation(fig, animate, init_func=init, frames=100, interval=100, blit=True)

rc('animation', html='jshtml')
anim

plt.show()
