"""Plot cubic bezier generated from the test file."""
import numpy as np
import matplotlib.pyplot as plt
import csv
import argparse


class FixedPoint(int):
    """Fixed point utility class.

    Module containing an utility class to quickly parse and interpret
    fixed point numbers from strings.
    """

    def __new__(cls, raw_value, width, fractional_width):
        return  super().__new__(cls, raw_value)

    def __init__(self, raw_value, width, fractional_width):
        """Create a fixed point from the raw value."""
        self.fractional_width = fractional_width
        self.width = width

    @classmethod
    def from_binary_string(cls, value):
        """Somethin like '0b1001010.1010101'"""
        # Remove 0b
        value = value[2:]
        # Split fractional an integral parts
        integral = value
        fractional = ''
        if '.' in value:
            integral, fractional = value.split('.')
        raw_bin_string = '0b' + integral + fractional
        raw_value = int(raw_bin_string, 2)
        width = len(fractional) + len(integral)
        return cls(raw_value, width, len(fractional))

    def to_float(self):
        # Consider that the last bit represent the sign of my fixed point
        # number.
        bin_repr = bin(self)[2:]
        width = len(bin_repr)
        sign = -1 if self.width == width and bin_repr[0] == '1' else 1
        value = self if sign > 0 else (~self + 1) & ((1 << width) - 1)
        return sign * value / (1 << self.fractional_width)


def centered_cubic_bezier(c1, c2, c3):
    a = 3 * c1
    b = 3 * (c2 - 2 * c1)
    c = 3 * c1 - 3 * c2 + c3
    def fn(x):
        return a * x + b * x ** 2 + c * x ** 3
    return fn


def main():
    parser = argparse.ArgumentParser(
        description='Read hex numbers and plot fixed point values.')
    parser.add_argument('--c1', metavar='C1', type=int, default=190,
                        help='Control point c1.')
    parser.add_argument('--c2', metavar='C2', type=int, default=-232,
                        help='Control point c2.')
    parser.add_argument('--c3', metavar='C3', type=int, default=-2,
                        help='Control point c3.')
    parser.add_argument('csv', metavar='csv', type=str,
                        help='File name containing hex values.')

    args = parser.parse_args()

    data = []
    with open(args.csv, newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=' ')
        for row in reader:
            value = FixedPoint(int(row[1], 16), 74, 50)
            data.append((int(row[0], 16) / (1 << 16), value.to_float()))
    data = np.array(data)

    bezier = centered_cubic_bezier(args.c1, args.c2, args.c3)
    x = np.linspace(0, 1)

    plt.plot(x, bezier(x), label='theoretical')
    plt.plot(data[:, 0], data[:, 1], label='xls')
    plt.legend()
    plt.show()


if __name__ == '__main__':
    main()
