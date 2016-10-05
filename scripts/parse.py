#!/usr/bin/env python

import json

def to_nan(l):
  def _list_rec(x):
    if type(x) == list:
      return map(_list_rec, x)
    else:
      if x == None:
        return float('nan')
      else:
        return x
      
  if type(l) == list:
    return map(_list_rec, l)
  else:
    return l

def load_data(fname, prev_results=None):
  if prev_results:
    results = prev_results
  else:
    results = {}
  with open(fname) as f:
    rs = json.load(f)
    for result in rs:
      fn = result["FN_NAME"]
      name = result["NAME"]
      d = {}
      for k,v in result.iteritems():
        d[str(k)] = to_nan(v)
      results[(fn, name)] = d
  return results

def main():
  data = load_data("output.json")
  print data

if __name__ == "__main__":
  main()
