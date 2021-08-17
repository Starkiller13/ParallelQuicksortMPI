from matplotlib import pyplot as plt 

l100k = [0.023931,0.047268,0.100680,0.185739]
l500k = [0.263586,0.321790,0.648521,1.541518]
l1M =   [0.619727,1.562687,1.910092,4.343471]
l5M =   [4.470648,7.117961,16.075440,34.431294]
x = ['100k','500k','1M','5M']
y = ['MPI(8 cpus)', 'MPI(4 cpus)', 'MPI(2 cpus)', 'sequential(1 cpu)']
plt.figure(figsize =(8,6))
plt.title('Performance with different input sizes')
plt.plot(x,l100k)
plt.plot(x,l500k)
plt.plot(x,l1M)
plt.plot(x,l5M)
plt.legend(y)
plt.xlabel("array size")
plt.ylabel("Elapsed time (seconds)")
plt.savefig('./charts/inputsChart.png')
plt.close()

plt.figure(figsize =(8,6))
plt.title('Performance with different input sizes (logarithmic)')
plt.plot(x,l100k)
plt.plot(x,l500k)
plt.plot(x,l1M)
plt.plot(x,l5M)
plt.legend(y)
plt.xlabel("array size")
plt.ylabel("Elapsed time (seconds)")
plt.yscale('log')
plt.savefig('./charts/inputsChartLog.png')
plt.close()

o1_1=[5.187633,5.153617,5.802215,5.770064,5.789073]
o2_1=[5.746162,5.089227,5.672594,5.569185,5.608725]
o3_1=[5.633591,5.588410,5.648716,5.595168,5.644653]
o1_8=[0.667561,0.557386,0.603485,0.574538,0.344274]
o2_8=[0.616873,0.554488,0.547053,0.573301,0.431315]
o3_8=[0.426237,0.682554,0.322515,0.661038,0.323314]
plt.figure(figsize =(8,8))
plt.suptitle('Compiler Optimizations Analysis')
plt.subplot(2,1,1)
plt.boxplot([o1_1,o2_1,o3_1],labels=['O1','O2','O3'],patch_artist=True)
plt.title('Sequential (1 cpu)')
plt.xlabel('Compiler optimization')
plt.ylabel("Elapsed time (seconds)")
plt.subplot(2,1,2)
plt.title('Parallel (8 cpus)')
plt.boxplot([o1_8,o2_8,o3_8],labels=['O1','O2','O3'],patch_artist=True)
plt.xlabel('Compiler optimization')
plt.ylabel("Elapsed time (seconds)")
plt.tight_layout()
plt.savefig('./charts/opt_boxplot.png')
plt.close()