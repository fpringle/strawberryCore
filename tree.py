#!/usr/bin/python

import os
import math
from functools import reduce
from itertools import product

class tree:
    def __init__(self, lastmove, value, movestr=None, children=None):
        self.lastmove = lastmove.strip()
        self.value    = value

        if movestr is None:
            self.movestr = lastmove
        elif isinstance(movestr, str):
            self.movestr = movestr.strip()
        else:
            raise ValueError('argument 3 must be None or a string')

        if children is None:
            self.children = []
        elif isinstance(children,list) and all( isinstance(child,tree) for child in children ):
                self.children = children
        else:
            raise ValueError('argument 4 must be None or a list of trees')

    def add_child(self,*args):
        if len(args)==1 and isinstance(args[0], tree):
            self.children.append(args[0])
        elif len(args)==2:
            child = tree( *args,movestr=self.movestr+' '+args[0] )
            self.children.append(child)
        else:
            raise ValueError

    def is_leaf(self):
        return len(self.children)==0

    def leaves(self):
        if self.is_leaf(): yield self
        else:
            for child in self:
                yield from child.leaves()

    def __repr__(self):
        if self.is_leaf():
            return f'({self.lastmove},{self.value})'
        else:
            return f'[({self.lastmove},{self.value}), [' + ', '.join([child.__repr__() for child in self.children]) + ']]'

    def __getitem__(self,key):
        if isinstance(key,int):
            return self.children[key]
        else:
            temp = self
            for k in key:
                temp = temp[k]
            return temp

    def __iter__(self):
        for child in self.children:
            yield child

    def iter_all(self):
        yield self
        for child in self.children:
            yield from child.iter_all()

    def shape(self):
        tmp = [self]
        ret = ()
        while tmp:
            ret += (len(tmp),)
            tmp = reduce(lambda a,b: a+b, [x.children for x in tmp])

        return tuple(ret)

    def depth(self):
        return len(self.shape())

def negamax( node, _depth=None, final=True ):
    if _depth is None:
        depth = node.depth()-1
    else:
        depth = _depth

    if not depth: return node.value

    mx = - math.inf
    for child in node:
        score = -negamax( child, depth-1, False )
        if final: print(child.lastmove, score)
        if score > mx:
            bestmove = child.lastmove
            mx = score

    if final:
        return bestmove
    else:
        return mx

def make_leaves(parent,file):
    ret = []
    for line in file.readlines():
        moves,score = line.split(' : ')
        moves = moves.split(' ')[::-1]
        if parents == moves[:-1]:
            ret.append((moves[-1],score))

    return ret

def make_subtrees(parent):
    ret = []
    for line in file.readlines():
        moves,score = line.split(' : ')
        moves = moves.split(' ')[::-1]
        if parents == moves[:-1]:
            ret.append((moves[-1],score))

    return ret

def is_suffix(small, big):
    return small == big[:len(small)]

def make_tree(rootstr,rootval,*files):
    score_dict = { rootstr: rootval }

    for file in files:
        for line in file.readlines():
            moves, score = line.split(' : ')
            moves = ' '.join(moves.split(' ')[::-1])
            score_dict[moves] = int(score)

    max_len = len(files)
    d = [{rootstr:score_dict[rootstr]}] + [dict((k,v) for k,v in score_dict.items() if k!=rootstr and k.count(' ')==l) for l in range(max_len)]
    print([len(y) for y in d])
 #   for i,v in enumerate(d): print(i,v,end='\n\n')

#    paths = {root:()}
#    q = [self]
#    while q:
#        v = q.pop(0)
#        for i,w in enumerate(v):
#            paths[w] = paths[v] + (i,)
#            q.append(w)

    root = tree('',rootval)

    levels = [[] for _ in range(max_len+1)]
    levels[0].append(root)

    for i in range(1,max_len+1):
#        print(root)
        lvs = list(root.leaves())
        for k,v in d[i].items():
            for leaf in levels[i-1]:
#                print(leaf.movestr,k)
                if is_suffix(leaf.movestr,k):
                    leaf.add_child(k.split(' ')[-1],v)
                    levels[i].append(leaf[-1])

    assert [len(k) for k in d] == list(root.shape())
    return root


if __name__ == '__main__':
    duration = 1  # seconds
    freq = 1760  # Hz
    print('starting...')
#    os.system('play -nq -t alsa synth {} sine {}'.format(duration, freq))
    n = 3
    f = [open(f'startpos_{i}.txt','r') for i in range(1,n+1)]

    x = make_tree('root',0,*f)
    print('tree created')
    for file in f:
        file.close()

    print(negamax(x))

#    for _ in range(5): os.system('play -nq -t alsa synth {} sine {}'.format(duration, freq))

else:
    r=tree('root',0)
    r.add_child('a2a4',10)
    r.add_child('a2a3',5)
    r[0].add_child('a7a5',0)
    r[0].add_child('a7a6',5)
    r[1].add_child('a7a6',0)
    r[1].add_child('a7a5',-5)
    print(r)
    print(r[0])
    print(r[0,1])
    print(r.shape())
