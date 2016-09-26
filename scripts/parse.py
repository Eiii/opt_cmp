#!/usr/bin/env python

import csv

def next_line(reader):
  line = None
  while line == None or line[0] == '#':
    line = reader.next()
  return line

def load_data(fname, prev_results=None):
  if prev_results:
    results = prev_results
  else:
    results = {}
  with open(fname) as f:
    reader = csv.reader(f)
    try:
      while True:
        fn, name, num_sections = next_line(reader)
        data = {}
        for _ in range(int(num_sections)):
          load_section(data, reader)
        results[(fn, name)] = data
    except StopIteration:
      pass
  return results

def load_section(data, reader):
  name, count = next_line(reader)
  section_data = []
  for _ in range(int(count)):
    line = next_line(reader);
    section_data.append(map(float, line))
  data[name] = section_data


def main():
  data = load_data("bo.csv")
  print data

if __name__ == "__main__":
  main()
