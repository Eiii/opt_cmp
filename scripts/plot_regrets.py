#!/usr/bin/env python

#
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
import sys
import functools
from parse import load_data
from math import isnan

### Globals?
REGRETS = "REGRETS"
WS = "WS"
DISTS = "DISTS"

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

def list_slope(width, l):
  slope = [float('nan')]*width
  for i in range(len(l))[width:-width]:
    start_val = l[i-width]
    end_val = l[i+width]
    s = -(end_val-start_val)/width
    slope.append(s)
  slope += [float('nan')]*width
  return slope

def data_for_fn(goal_fn, data):
  return {alg: v for ((fn, alg), v) in data.iteritems() if fn == goal_fn}

def data_filter(data, goal_fn, dtype):
  return {alg: v[dtype] for ((fn, alg), v) in data.iteritems() if fn == goal_fn and dtype in v}

def plot_vals_err(fn, regrets, ylabel="Regret", log_scale=False, ymax_dict={}):
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
  plt.ylabel(ylabel)

  if log_scale:
    plt.yscale('log', nonposy='clip')
  else:
    _, ymax = plt.ylim()
    if fn in ymax_dict:
      ymax = ymax_dict[fn]
    plt.ylim((-0.1,ymax))

def pair_slope_val(width, data, key):
  def remove_nan(l):
    temp = zip(*l)
    temp = filter(lambda x: not any(map(isnan, x)), temp)
    return zip(*temp)
  slopes = map(functools.partial(list_slope, width), data[REGRETS])
  z = map(remove_nan, zip(slopes, data[key]))
  #Remove any entries with NaN
  return z

def reduce_sublists(d):
  temp = zip(*d)
  temp = map(lambda l: reduce(lambda x, y: x+y, l), temp)
  return temp

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

def plot_scatter(fn, pair, ylabel, alpha=0.1, log_scale=True):
  slopes, ws = pair
  if log_scale:
    plt.xscale('log')
  plt.scatter(slopes, ws, s=40, alpha=alpha, edgecolors='none')
  plt.title(fn)
  plt.xlabel('Regret Improvement')
  plt.ylabel(ylabel)

def plot_singles(fn, top, bot):
  xs = range(len(top))
  plt.subplot(2,1,1)
  plt.yscale('log', nonposy='clip')
  plt.plot(xs, top)
  plt.subplot(2,1,2)
  plt.yscale('log', nonposy='clip')
  plt.plot(xs, bot)
  plt.xlabel('Function Evaluations')

def output_all_plots():
  use_log_scale = False
  ymax_dict = {'rosenbrock_2' : 4 }
  data = load_data("bo.csv")

  for fn in all_fns(data):
    regrets = data_filter(data, fn, REGRETS)
    plt.clf()
    plot_vals_err(fn, regrets)
    out_name = fn+'_regrets.png'
    plt.savefig(out_name, bbox_inches='tight')

  for fn in all_fns(data):
    ws = data_filter(data, fn, WS)
    plt.clf()
    plot_vals_err(fn, ws, ylabel="W")
    out_name = fn+'_ws.png'
    plt.savefig(out_name, bbox_inches='tight')

  for fn in all_fns(data):
    dists = data_filter(data, fn, DISTS)
    plt.clf()
    plot_vals_err(fn, dists, ylabel="Min. Distance")
    out_name = fn+'_dists.png'
    plt.savefig(out_name, bbox_inches='tight')

  for fn in all_fns(data):
    d = data_for_fn(fn, data)['LOGO']
    slope_ws = reduce_sublists(pair_slope_val(1, d, WS))
    plt.clf()
    plot_scatter(fn, slope_ws, 'W', alpha=0.05)
    out_name = fn+'_ws_scatter.png'
    plt.savefig(out_name, bbox_inches='tight')

  for fn in all_fns(data):
    d = data_for_fn(fn, data)['BO1']
    slope_dists = reduce_sublists(pair_slope_val(1, d, DISTS))
    plt.clf()
    plot_scatter(fn, slope_dists, 'Min. Distance')
    out_name = fn+'_dists_scatter.png'
    plt.savefig(out_name, bbox_inches='tight')

def output_singles():
  max_out = 20
  data = load_data("bo.csv")
  for fn in all_fns(data):
    print fn
    d = data_for_fn(fn, data)['BO1']
    pairs = zip(d[REGRETS], d[DISTS])
    for idx, (top, bot) in enumerate(pairs[:max_out]):
      idx += 1
      if idx % 10 == 0:
        print idx
      plt.clf()
      plot_singles(fn, top, bot)
      out_name = fn+'_single_'+str(idx)+'.png'
      plt.savefig(out_name, bbox_inches='tight')

def main():
  data = load_data("bo.csv")
  d = data_for_fn('hartman_3', data)['BO1']
  d = reduce_sublists(pair_slope_val(2, d, DISTS))
  print map(len, d)


if __name__ == "__main__":
  #main()
  #output_all_plots()
  output_singles()
