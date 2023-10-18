import pandas as pd
import matplotlib.pyplot as plt

plt.rcParams["figure.figsize"] = [7.50, 3.50]
plt.rcParams["figure.autolayout"] = True

headers = ['m', 'n', 'V', 'sx', 'sy', 'tx', 'ty', 'time1', 'time2', 'time4', 'time8']

df = pd.read_csv('results.csv', names=headers)

df.drop('m', axis=1, inplace=True)
df.drop('n', axis=1, inplace=True)
df.drop('sx', axis=1, inplace=True)
df.drop('sy', axis=1, inplace=True)
df.drop('tx', axis=1, inplace=True)
df.drop('ty', axis=1, inplace=True)

df.drop(0, axis=0, inplace=True)

df['V'] = df['V'].astype(float)
df['time1'] = df['time1'].astype(float)
df['time2'] = df['time2'].astype(float)
df['time4'] = df['time4'].astype(float)
df['time8'] = df['time8'].astype(float)

ax1 = df.plot.scatter(x = 'V', y = 'time1', s = 10, color = 'r', label = '1 thread');
ax2 = df.plot.scatter(x = 'V', y = 'time2', s = 10, ax = ax1, color = 'b', label = '2 threads');
ax3 = df.plot.scatter(x = 'V', y = 'time4', s = 10, ax = ax1, color = 'g', label = '4 threads');
ax4 = df.plot.scatter(x = 'V', y = 'time8', s = 10, ax = ax1, color = 'purple', label = '8 threads');


plt.show()
