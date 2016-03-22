import csv
import numpy
import scipy.io.wavfile
from matplotlib import pyplot

class PeakDecoder(object):
    def __init__(self, filename):
        self.input  = scipy.io.wavfile.read(filename)[1]
        self.length = len(self.input)
        power = 0
        while 2 ** power < self.length:
            power += 1

        self.input = numpy.append(self.input, numpy.zeros(2 ** power - self.length))
        self.count = 0

    def decode(self):
        spectogram  = pyplot.specgram(self.input, 1024)[0]
        spectogram  = (numpy.matrix(numpy.ones(len(spectogram))) * numpy.matrix(spectogram)).transpose()
        threshold   = numpy.max(spectogram) / 10.0
        last        = 0
        for b, value in enumerate(spectogram):
            if value > threshold:
                if last == 0:
                    self.count += 1

                last = 1

            else:
                last = 0

if __name__ == '__main__':
    import sys

    morse = PeakDecoder(sys.argv[1])
    morse.decode()
    print morse.count
