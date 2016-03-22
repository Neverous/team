import csv
import numpy
import scipy.io.wavfile
from matplotlib import pyplot

class MorseDecoder(object):
    def __init__(self, filename):
        self.dict = {}
        for entry in csv.DictReader(open('morse.csv'), delimiter=',', fieldnames=['char', 'code']):
            self.dict[entry['code']] = entry['char']

        self.dict['|'] = ' '
        self.input  = scipy.io.wavfile.read(filename)[1]
        self.length = len(self.input)
        power = 0
        while 2 ** power < self.length:
            power += 1

        self.input = numpy.append(self.input, numpy.zeros(2 ** power - self.length))
        self.output = []

    def decode(self):
        spectogram  = pyplot.specgram(self.input)[0]
        spectogram  = (numpy.matrix(numpy.ones(len(spectogram))) * numpy.matrix(spectogram)).transpose()
        threshold   = numpy.max(spectogram) / 2.0
        last        = 0
        count       = 0
        for b, value in enumerate(spectogram):
            count += 1
            if value > threshold:
                if last == 0:
                    self.output.append(-count)

                last += 1

            elif last > 0:
                self.output.append(last)
                count = 0
                last = 0

    def tostring(self):
        result = ['']
        for value in self.output:
            if value > 5:
                result[-1] += '.' if value < 10 else '-'

            elif value < -5:
                if value < -200:
                    result.append('|')

                if value < -100:
                    result.append('')

        return ' '.join(result)

    def totext(self):
        string = self.tostring()
        return ''.join(map(self.dict.__getitem__, string.split()))

if __name__ == '__main__':
    import sys

    morse = MorseDecoder(sys.argv[1])
    morse.decode()
    print >>sys.stderr, morse.tostring()
    print morse.totext()
