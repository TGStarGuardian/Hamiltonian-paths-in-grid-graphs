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

speedup2 = df['time1'] / df['time2']
speedup4 = df['time1'] / df['time4']
speedup8 = df['time1'] / df['time8']

df.insert(2, 'speedup2', speedup2, True)
df.insert(5, 'speedup4', speedup4, True)
df.insert(7, 'speedup8', speedup8, True)

bx1 = df.plot.scatter(x = 'V', y = 'speedup2', s = 10, color = 'r', label = '2 thread');
bx2 = df.plot.scatter(x = 'V', y = 'speedup4', s = 10, ax = bx1, color = 'b', label = '4 threads');
bx3 = df.plot.scatter(x = 'V', y = 'speedup8', s = 10, ax = bx1, color = 'g', label = '8 threads');

from sklearn import linear_model
from sklearn import metrics
import numpy as np

x = np.array(df['V'])
y = np.array(df['speedup2'])
model = linear_model.LinearRegression()
model.fit(x.reshape(-1, 1), y)

print(model.coef_[0])
y_predicted = model.predict(x.reshape(-1, 1))
mae = metrics.mean_absolute_error(y_predicted, y)
mse = metrics.mean_squared_error(y_predicted, y)
r2 = metrics.r2_score(y_predicted, y)
print('MAE = ', mae)
print('MSE = ', mse)
print('R2 = ', r2)

x1 = [0.0, model.intercept_]
x2 = [1000000.0, 1000000.0 * model.coef_[0] + model.intercept_]

plt.axline(x1, x2)

x = np.array(df['V'])
y = np.array(df['speedup4'])
model = linear_model.LinearRegression()
model.fit(x.reshape(-1, 1), y)

print(model.coef_[0])
y_predicted = model.predict(x.reshape(-1, 1))
mae = metrics.mean_absolute_error(y_predicted, y)
mse = metrics.mean_squared_error(y_predicted, y)
r2 = metrics.r2_score(y_predicted, y)
print('MAE = ', mae)
print('MSE = ', mse)
print('R2 = ', r2)


x1 = [0.0, model.intercept_]
x2 = [1000000.0, 1000000.0 * model.coef_[0] + model.intercept_]

plt.axline(x1, x2)

x = np.array(df['V'])
y = np.array(df['speedup8'])
model = linear_model.LinearRegression()
model.fit(x.reshape(-1, 1), y)

print(model.coef_[0])
y_predicted = model.predict(x.reshape(-1, 1))
mae = metrics.mean_absolute_error(y_predicted, y)
mse = metrics.mean_squared_error(y_predicted, y)
r2 = metrics.r2_score(y_predicted, y)
print('MAE = ', mae)
print('MSE = ', mse)
print('R2 = ', r2)


x1 = [0.0, model.intercept_]
x2 = [1000000.0, 1000000.0 * model.coef_[0] + model.intercept_]

plt.axline(x1, x2)


plt.show()
