#!/usr/bin/env python

import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
import csv

title = ""
xlabel = "Samples"
ylabel = "Avg. Regret"
ylabel_running = "Avg. Running Experiments"
dataset = (
           ('soo', 'SOO'),
           ('logo', 'LOGO'),
           ('bo', 'bayesopt (50 runs)'),
          )

def regret_file(prefix):
  return prefix+'.csv'

def load_regret_data(prefix):
  fname = regret_file(prefix)
  all_regrets = []
  with open(fname) as f:
    reader = csv.reader(f, delimiter='\t')
    for line in reader:
      x = int(line[0])
      r = float(line[1])
      all_regrets.append((x,r))
  return all_regrets

###

fig, ax = plt.subplots()

for prefix, name in dataset:
  regrets = load_regret_data(prefix)
  x, y = zip(*regrets)
  ax.plot(x, y, label=name)

ax.set_yscale('log')
ax.set_xlabel(xlabel)
ax.set_ylabel(ylabel)
plt.title(title)
plt.legend()
plt.savefig('out.png')

