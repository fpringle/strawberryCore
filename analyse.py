#!/usr/bin/python

import sys
from pprint import pprint


def parse_line(line):
    fields = line.strip().split(' ')
    if len(fields) == 1:
        dashes, val, move = '',*fields,''
    elif len(fields) == 2:
        (dashes, val), move = fields,''
    elif len(fields) == 3:
        dashes, val, move = fields
    else:
        raise Exception(f"invalid line: {line}")
    dashes = dashes.count('-')
    val = int(val.strip(','))
    return dashes, val, move


"""
tree syntax:
    tree     := {val: [children]}
    val      := (int,str)    # value,move
    children := tree*

"""

def _parse_tree(lines,lo):
    ret = {}
    root_line = lines[lo]
    root_d, root_v, root_m = parse_line(root_line)
    ret[(root_v,root_m)] = []
    if lo+1 == len(lines):
        return ret,lo+1

    i = lo+1
    while i<len(lines):
        line = lines[i]
        d,v,m = parse_line(line)
        if d<=root_d:
            return ret,i
        else:
            suc,i = _parse_tree(lines,i)
            ret[(root_v,root_m)].append(suc)

    return ret,i

def parse_tree(lines):
    return _parse_tree(lines,0)[0]

def test():
    with open("test_tree.txt",'r') as f:
        lines = f.read().strip().split('\n')
    tree = parse_tree(lines)
    return tree

if __name__ == '__main__':
    lines = sys.stdin.read()
    lines = lines.strip().split('\n')
    tree = parse_tree(lines)
    pprint(tree)
