#!/usr/bin/env python

import csv

def load_data(fname, prev_results=None):
  if prev_results:
    results = prev_results
  else:
    results = {}
  with open(fname) as f:
    reader = csv.reader(f)
    try:
      while True:
        fn, name, num_sections = reader.next()
        data = {}
        for _ in range(int(num_sections)):
          load_section(data, reader)
        results[(fn, name)] = data
    except StopIteration:
      pass
  return results

def load_section(data, reader):
  name, count = reader.next()
  section_data = []
  for _ in range(int(count)):
    section_data.append(map(float, reader.next()))
  data[name] = section_data


def main():
  data = load_data("bo.csv")
  print data

if __name__ == "__main__":
  main()
