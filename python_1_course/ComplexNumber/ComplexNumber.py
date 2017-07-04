import sys


class ComplexNumber:
    def __init__(self, real=0, imaginary=0):
        self._real = real
        self._imaginary = imaginary

    def get_real(self):
        return self._real

    def get_imaginary(self):
        return self._imaginary

    def __add__(self, other):
        return ComplexNumber(self._real + other.get_real(),
                             self._imaginary + other.get_imaginary())

    def __sub__(self, other):
        return ComplexNumber(self._real - other.get_real(),
                             self._imaginary - other.get_imaginary())

    def __mul__(self, other):
        numerator = self._real * other.get_real() - \
            self._imaginary * other.get_imaginary()
        denominator = self._real * other.get_imaginary() + \
            self._imaginary * other.get_real()
        return ComplexNumber(numerator, denominator)

    def get_inverse(self):
        length_square = self._real**2 + self._imaginary**2
        return ComplexNumber(self._real / length_square,
                             -self._imaginary / length_square)

    def __truediv__(self, other):
        return self * other.get_inverse()

    def __str__(self):
        if self._real == 0 and self._imaginary == 0:
            return "0.00"
        real = "%.2f" % self._real
        imaginary = "%.2f" % self._imaginary + 'i'
        if self._real == 0:
            return imaginary
        if self._imaginary == 0:
            return real
        if self._imaginary > 0:
            return real + " + " + imaginary
        else:
            imaginary = imaginary[1:]
            return real + " - " + imaginary

if __name__ == "__main__":
    for line in sys.stdin.readlines():
        print(eval(line.strip()))
