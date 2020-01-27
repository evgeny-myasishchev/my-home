#!/usr/bin/env micropython

import unittest
import sys

binPath = sys.path[0]
sys.path.insert(0, binPath + '/..')
sys.path.insert(0, binPath + '/../src')

moduleName = 'tests'

argNum = 1
while argNum < len(sys.argv):
    if sys.argv[argNum] == "-h":
        print('Run unit tests. Args:')
        print('-f <file path> Run unit tests for given file only')
        sys.exit(0)
    if sys.argv[argNum] == "-f":
        argNum += 1
        if argNum >= len(sys.argv):
            print('-f requires argument')
            sys.exit(1)
        moduleName = sys.argv[argNum].replace('.py', '', 1)
    argNum += 1
    pass

unittest.main(moduleName)