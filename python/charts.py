from matplotlib import pyplot as plt 
l1 = [1.844845,2.376974,2.273959,2.141857]
l2 = [3.929487,4.790447,2.779488,4.837241]
l4 =   [7.761439,5.848254,7.008684,7.701633]
l8 =   [4.028433,11.85865,9.712960,28.40671]
x = ['100k','500k','1M','5M']
y = ['MPI(16 cpus)','MPI(8 cpus)', 'MPI(4 cpus)', 'MPI(2 cpus)']
plt.figure(figsize =(8,6))
plt.title('Speedup coefficient of the previous runs')
plt.plot(x,l8)
plt.plot(x,l4)
plt.plot(x,l2)
plt.plot(x,l1)
plt.legend(y)
plt.xlabel("array size")
plt.ylabel("speedup rateo")
plt.savefig('./charts/speedup.png')
plt.close()

'''
l1 = [0.185739, 1.541518, 4.343471, 34.431294]
l2 = [0.100680,0.648521,1.910092,16.075440]
l4 =   [0.047268,0.321790,1.562687,7.117961]
l8 =   [0.023931,0.263586,0.619727,4.470648]
l16 = [0.046107, 0.129991, 0.447183, 1.212083]
x = ['100k','500k','1M','5M']
y = ['MPI(16 cpus)','MPI(8 cpus)', 'MPI(4 cpus)', 'MPI(2 cpus)', 'sequential(1 cpu)']
plt.figure(figsize =(8,6))
plt.title('Performance with different input sizes (logarithmic scale y axis)')
plt.plot(x,l16)
plt.plot(x,l8)
plt.plot(x,l4)
plt.plot(x,l2)
plt.plot(x,l1)
plt.yscale("log")
plt.legend(y)
plt.xlabel("array size")
plt.ylabel("Elapsed time (seconds)")
plt.savefig('./charts/inputsChartLog.png')
plt.close()
'''
'''
z = ['O1','O2','O3','no opt']
o100k=[0.209533,0.196411,0.197216,0.272793]
o500k=[1.570790,2.083179,1.961177,1.920316]
o1M=[5.259612,4.733656,5.632962,5.259389]
o5M=[34.630427,31.375935,33.084449,34.837840]
plt.figure(figsize = (8,6))
plt.suptitle('Compiler Optimizations Analysis')
plt.scatter(z,o5M)
plt.scatter(z,o1M)
plt.scatter(z,o500k)
plt.scatter(z,o100k)
plt.yscale('log')
plt.xlabel('Optimizations')
plt.ylabel('Execution time (lower is better)')
plt.legend(x[::-1])
for a,b in zip(z,o100k):
    label = "{:.2f}s".format(b)
    plt.annotate(label, (a,b),textcoords="offset points", xytext=(0,10), ha='center')
for a,b in zip(z,o500k):
    label = "{:.2f}s".format(b)
    plt.annotate(label, (a,b),textcoords="offset points", xytext=(0,10), ha='center')
for a,b in zip(z,o1M):
    label = "{:.2f}s".format(b)
    plt.annotate(label, (a,b),textcoords="offset points", xytext=(0,10), ha='center')
for a,b in zip(z,o5M):
    label = "{:.2f}s".format(b)
    plt.annotate(label, (a,b),textcoords="offset points", xytext=(0,10), ha='center')

plt.savefig('./charts/opt_boxplot.png')
plt.close()
'''