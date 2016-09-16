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

def list_std(data):
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

def data_filter(data, goal_fn, dtype):
  return {alg: v[dtype] for ((fn, alg), v) in data.iteritems() if fn == goal_fn and dtype in v}

def main():
  data = load_data("bo.csv")
  pass

def output_all_plots():
  use_log_scale = False
  ymax_dict = {'rosenbrock_2' : 4 }
  data = load_data("bo.csv")

  for fn in all_fns(data):
    regrets = data_filter(data, fn, REGRETS)
    plt.clf()
    plot_regrets(fn, regrets)
    out_name = fn+'_regrets.png'
    plt.savefig(out_name, bbox_inches='tight')

  for fn in all_fns(data):
    ws = data_filter(data, fn, WS)
    plt.clf()
    plot_regrets(fn, ws)
    out_name = fn+'_ws.png'
    plt.savefig(out_name, bbox_inches='tight')

def plot_regrets(fn, regrets, log_scale=False, ymax_dict={}):
  ax = plt.gca()
  lcl_data = {k: (list_avg(v), list_std(v)) for (k, v) in regrets.iteritems()}
  for (name, (reg, err)) in lcl_data.iteritems():
    x = range(len(reg))
    err_top = [a+b for a,b in zip(reg, err)]
    err_bot = [a-b for a,b in zip(reg, err)]
    ax.plot(x, reg, label=name)
    last_col = ax.lines[-1].get_color()
    ax.fill_between(x, err_bot, err_top, alpha=0.3, color=last_col)
  plt.legend()
  plt.title(fn)
  plt.xlabel('Function evaluations')
  plt.ylabel('Regret')

  if log_scale:
    plt.yscale('log', nonposy='clip')
  else:
    _, ymax = plt.ylim()
    if fn in ymax_dict:
      ymax = ymax_dict[fn]
    plt.ylim((-0.1,ymax))

def plot_ws(fn, ws):
  ax = plt.gca()
  lcl_data = {k: (list_avg(v), list_std(v)) for (k, v) in ws.iteritems()}
  for (name, (reg, err)) in lcl_data.iteritems():
    x = range(len(reg))
    err_top = [a+b for a,b in zip(reg, err)]
    err_bot = [a-b for a,b in zip(reg, err)]
    ax.plot(x, reg, label=name)
    last_col = ax.lines[-1].get_color()
    ax.fill_between(x, err_bot, err_top, alpha=0.3, color=last_col)
  plt.legend()
  plt.title(fn)
  plt.xlabel('Function evaluations')
  plt.ylabel('W value')

if __name__ == "__main__":
  output_all_plots()
