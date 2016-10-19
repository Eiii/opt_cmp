#!/usr/bin/env python

import json
import os

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

#Specifically for BO
def modify_name(name, version):
  if name == "BO1":
    info=version.split(",")[1:-1]
    info_dict = {}
    for k,v in [x.split(":") for x in info]:
      info_dict[k] = v 

    info_str = "-".join(['', info_dict["crit"], info_dict["kernel"]])
    return name[:-1]+info_str
  else:
    return name

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
      name = modify_name(name, result["VERSION"])
      d = {}
      for k,v in result.iteritems():
        d[str(k)] = to_nan(v)
      results[(fn, name)] = d
  return results

def load_all(path, data=None):
  files = [f for f in os.listdir(path) if ".out" in f]
  for f in files:
    filepath = os.path.join(path, f)
    data = load_data(filepath, data)
  return data

def remove_data(data, fns):
  return {k:data[k] for k in data if not any([x in k[1] for x in fns])}

def main():
  data = load_data("output.json")
  print data

if __name__ == "__main__":
  main()
