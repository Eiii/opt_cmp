#!/usr/bin/env python

#
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
import sys
from parse import load_data

### Globals?
REGRETS = "REGRETS"
WS = "WS"

###

def all_fns(data):
  fns = set()
  for fn, _ in data:
    fns.add(fn)
  return fns

def list_avg(l):
  return [sum(x)/len(x) for x in zip(*l)]

def calc_std(data):
  def isinf(f):
    return f == float("Inf")
  def replace(f):
    if any(map(isinf, f)):
      return [0]
    else:
      return f
  z = map(replace, zip(*data))
  results = [np.std(x) for x in z]
  return results

def data_for_fn(goal_fn, data):
  return {alg: v for ((fn, alg), v) in data.iteritems() if fn == goal_fn}

def main():
  data = load_data("bo.csv")
  for fn in all_fns(data):
    d = data_for_fn(fn, data)
    for k, v in d.iteritems():
      print k, list_avg(v[REGRETS])
      if k == "LOGO":
        print k, list_avg(v[WS])

if __name__ == "__main__":
  main()
  sys.exit()

###

use_log_scale = False
data = load_data("output.csv")
data = load_data("bo.csv", data)
ymax_dict = {'rosenbrock_2' : 4 }

#Plot data
for fn in all_fns(data):
  ax = plt.gca()
  lcl_data = data_for_fn(fn, data)
  for name, regrets, errs in lcl_data:
    x = range(len(regrets))
    err_top = [a+b for a,b in zip(regrets, errs)]
    err_bot = [a-b for a,b in zip(regrets, errs)]
    ax.plot(x, regrets, label=name)
    last_col = ax.lines[-1].get_color()
    ax.fill_between(x, err_bot, err_top, alpha=0.3, color=last_col)
  plt.legend()
  plt.title(fn)
  plt.xlabel('Function evaluations')
  plt.ylabel('Regret')

  if use_log_scale:
    plt.yscale('log', nonposy='clip')
  else:
    _, ymax = plt.ylim()
    if fn in ymax_dict:
      ymax = ymax_dict[fn]
    plt.ylim((-0.1,ymax))

  out_name = fn+'.png'
  plt.savefig(out_name, bbox_inches='tight')
  plt.clf()
