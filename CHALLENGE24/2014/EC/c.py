import sys
from pprint import pprint

DIM         = 65536
LLEN        = 16

polygon     = []
line        = []
registers   = [i for i in range(15)]
program     = ''
data        = ''
d           = 0
read        = False

class Exit(Exception): pass

def writeRegister(R, B):
    global registers
    if R == 15:
        if B > 255:
            raise Exit()

        sys.stdout.write(chr(B))
        return

    registers[R] = B

def readRegister(R):
    global d, data, registers, read
    if R == 15:
        if read == False:
            d   += 1

        if d > len(data):
            return 256

        read = True
        return ord(data[d - 1])

    return registers[R]

def findline(x, y):
    global line
    for t, ll in enumerate(line):
        _, _, _, _, P = ll[:5]
        PT = ll[5:]

        for v in range(0, P):
            if PT[2 * v] == x and PT[2 * v + 1] == y:
                return t

    return -1

def inpolygon((x, y), poly):
    cnt     = poly[8]
    poly    = poly[9:]

    for v in range(cnt):
        if poly[2 * v] == x and poly[2 * v + 1] == y:
            return True

    return False

def getpolygon(l, old):
    global line, polygon
    first   = line[l][5:7]
    second  = line[l][-2:]

    if inpolygon(first, polygon[old]):
        search = second

    else:
        search = first

    for p, poly in enumerate(polygon):
        if inpolygon(search, poly):
            return p

    return -1

def interpret():
    global program, data, polygon, line, read
    program         = program.split('\n')
    polygons, lines = map(int, program[0].split())

    for p in range(polygons):
        polygon.append(map(int, program[p + 1].split()))

    for l in range(lines):
        line.append(map(int, program[polygons + l + 1].split()))

    ptr = 0
    verts = []
    for p in range(polygons):
        cnt     = polygon[p][8]
        poly    = polygon[p][9:]

        for v in range(cnt):
            verts.append((poly[2 * v + 1] * DIM + poly[2 * v], poly[2 * v], poly[2 * v + 1]))

    verts.sort()
    for p in range(polygons):
        if inpolygon(verts[0][1:], polygon[p]):
            ptr = p
            break

    while True:
        #print 'ptr', ptr
        # STAGE 1
        SR, SG, SB, SA, FR, FG, FB, FA, CNT = polygon[ptr][:9]
        VERTS = polygon[ptr][9:]
        A = (FR & 0xF0) >> 4
        B = FR & 0x0F
        C = (FG & 0xF0) >> 4
        D = FG & 0x0F
        E = (FB & 0xF0) >> 4
        F = FB & 0x0F
        X = FA
        Y = SR
        Z = SG * 256 + SB
        T = SA

        #print polygon[ptr][:8]
        #print A, B, C, D, E, F, '|', X, Y, Z, T
        #pprint(registers)

        read = False
        #print '(', Y, '+', readRegister(B), '+', readRegister(C), '*', readRegister(D), '+', readRegister(readRegister(F) & 0x0F), ') *', (255 - X), '/', ((255 - T) + readRegister(E)) + Z
        writeRegister(A, (((Y + readRegister(B) + readRegister(C) * readRegister(D) + readRegister(readRegister(F) & 0x0F)) * (255 - X) % (2 ** 32)) / ((255 - T) + readRegister(E)) + Z) % (2 ** 16))

        # STAGE 2
        sortme = []
        for v in range(CNT):
            x, y = VERTS[2 * v:2 * v + 2]
            t = findline(x, y)
            if t != -1:
                sortme.append((y * DIM + x, x, y, t))

        sortme.sort()
        res = sortme[-1][3]
        for (_, x, y, t) in sortme:
            LR, LG, LB, LA, P = line[t][:5]
            POINTS = line[t][5:]
            Q = x & 0x0F
            L = LR * 256 + LG
            H = LB * 256 + LA
            if readRegister(Q) >= L and readRegister(Q) < H:
                res = t
                break

        ptr = getpolygon(res, ptr)

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("usage: {0} program input".format(sys.argv[0]))
        sys.exit(1)

    program = open(sys.argv[1], 'rb').read()
    data    = open(sys.argv[2], 'rb').read()

    try:
        interpret()

    except Exit:
        pass
