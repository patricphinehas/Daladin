import sys
from parser import *


def main():
    if len(sys.argv) <= 1:
        print "Error: no url given"
        return 1

    player = selectParser(sys.argv[1])
    player.load()
    player.play()

if __name__ == '__main__':
    main()
