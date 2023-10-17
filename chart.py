import pandas as pd
import matplotlib.pyplot as plt

plt.rcParams["figure.figsize"] = [7.50, 3.50]
plt.rcParams["figure.autolayout"] = True

headers = ['m', 'n', 'V', 'sx', 'sy', 'tx', 'ty', 'time']

df = pd.read_csv('time 1.csv', names=headers)

df.drop('m', axis=1, inplace=True)
df.drop('n', axis=1, inplace=True)
df.drop('sx', axis=1, inplace=True)
df.drop('sy', axis=1, inplace=True)
df.drop('tx', axis=1, inplace=True)
df.drop('ty', axis=1, inplace=True)

df.drop(0, axis=0, inplace=True)

df['V'] = df['V'].astype(int)
df['time'] = df['time'].astype(int)

df.plot.scatter(x = 'V', y = 'time', s = 10);

plt.show()

