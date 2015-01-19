from matplotlib import pyplot
import pylab
from mpl_toolkits.mplot3d import Axes3D
import os

def visualise(points, filename):
    global idxFig

    #fig = pylab.figure(figsize=(10, 10))
    fig = pylab.figure()
    ax = Axes3D(fig)

    # Put points in cube corners to set axis scale
    sequence_containing_x_vals = [ 1, -1,  1, -1,  1, -1,  1, -1]
    sequence_containing_y_vals = [ 1,  1, -1, -1,  1,  1, -1, -1]
    sequence_containing_z_vals = [ 1,  1,  1,  1, -1, -1, -1, -1]

    for point in points.itervalues():
        sequence_containing_x_vals.append(point[0])
        sequence_containing_y_vals.append(point[1])
        sequence_containing_z_vals.append(point[2])

    ax.scatter(sequence_containing_x_vals,
                    sequence_containing_y_vals,
                    sequence_containing_z_vals, s=1, alpha=0.5)

    pylab.savefig('/tmp/plots/' + filename + '.png')

    pylab.close(fig)

base = "/tmp/points/"
filenames = os.listdir(base)
filenames.sort()
for filename in filenames:
    with open (base + filename) as pointFileHandle:
        pointFileString = pointFileHandle.read()
    points = eval(pointFileString)
    visualise(points, filename)
