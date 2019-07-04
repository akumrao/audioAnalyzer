"""
Matplotlib Animation Example

author: Jake Vanderplas
email: vanderplas@astro.washington.edu
website: http://jakevdp.github.com
license: BSD
Please feel free to use and modify this, but keep the above information. Thanks!
"""
#!/usr/bin/python3.5

import numpy as np
from matplotlib import pyplot as plt
from matplotlib import animation

# First set up the figure, the axis, and the plot element we want to animate
fig, ax = plt.subplots(2, 2)
ax[0, 0].set_xlim(( -4, 4))
ax[0, 0].set_ylim((-2, 2))

#ax = plt.axes(xlim=(0, 2), ylim=(-2, 2))
line1, = ax[0, 0].plot([], [], lw=2)
line2, = ax[0, 0].plot([], [], lw=2)


ax[0, 1].set_xlim(( -4, 4))
ax[0, 1].set_ylim((-2, 2))

#ax = plt.axes(xlim=(0, 2), ylim=(-2, 2))
line3, = ax[0, 1].plot([], [], lw=2)
line4, = ax[0, 1].plot([], [], lw=2)




# initialization function: plot the background of each frame
def init():
   return [line2, line1,  line3,  line4]

# animation function.  This is called sequentially
def animate(i):
    x =[1, 2, 3, 3]
    y = [1, 2, 3, 3]
    line2.set_data(x, y)
    line3.set_data(x, y)
    
    x = np.linspace(0, 2, 1000)
    y = np.sin(2 * np.pi * (x - 0.01 * i))
    line1.set_data(x, y)
    line4.set_data(x, y)
    return [line2, line1,  line3,  line4]

# call the animator.  blit=True means only re-draw the parts that have changed.
anim = animation.FuncAnimation(fig, animate, init_func=init,frames=200, interval=20, blit=True)
#anim = animation.FuncAnimation(fig, animate, init_func=init, frames=100, interval=100, blit=True)

# save the animation as an mp4.  This requires ffmpeg or mencoder to be
# installed.  The extra_args ensure that the x264 codec is used, so that
# the video can be embedded in html5.  You may need to adjust this for
# your system: for more information, see
# http://matplotlib.sourceforge.net/api/animation_api.html
#anim.save('basic_animation.mp4', fps=30, extra_args=['-vcodec', 'libx264'])

plt.show()
