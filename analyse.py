#!/usr/bin/python

import sys
from pprint import pprint
from math import inf

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

def _minimax(node, depth, player):
    (heuristic,_),children = list(node.items())[0]
    if depth==0 or children==[]:
        return heuristic
    if player==1:
        value = -inf
        for child in children:
            score = _minimax(child,depth-1,-player)
            value = max(value,score)
        return value
    else:
        value = +inf
        for child in children:
            score = _minimax(child,depth-1,-player)
            value = min(value,score)
        return value

def minimax(node,depth=-1):
    (heuristic,_),children = list(node.items())[0]
    if depth==0 or children==[]:
        return heuristic

    best_score = -inf
    for child in children:
        move = list(child.keys())[0][1]
        score = _minimax(child,depth-1,-1)
        if score > best_score:
            best_score = score
            best_move = move

    return best_move,best_score

def compare(input,depth):
    lines = input.strip().split('\n')
    best_move_line  = lines.pop()
    best_score_line = lines.pop()
    best_move_cpp  = best_move_line.split(' ')[-1]
    best_score_cpp = int(best_score_line.split(' ')[-1])
    tree = parse_tree(lines)
    best_move_py, best_score_py = minimax(tree,depth)
    if best_move_py == best_move_cpp:
        print(f"Python and C++ agree that the best move is {best_move_py} with a score of {best_score_py}")
        return True
    else:
        print(f"Python thinks the best move is {best_move_py} with a score of {best_score_py}")
        print(f"C++    thinks the best move is {best_move_cpp} with a score of {best_score_cpp}")
        return False

if __name__ == '__main__':
    try:
        depth = int(sys.argv[1])
    except:
        depth = -1
    input = sys.stdin.read()
    sys.exit(0 if compare(input,depth) else 1)
