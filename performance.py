from math import ceil
from timeit import timeit
from spam import getprimes as cgetprimes

def pgetprimes(n):
    if n <= 1: return 0
    if n == 2: return 1
    primes = []
    for i in range(3, n, 2):
        for j in range(3, ceil(i**(0.5))+1, 2):
            if i % j == 0: break
        else: primes.append(i)
    return primes

n, m = 10 ** 6, 1
tppy = timeit(lambda: pgetprimes(n), number=m)
tcpy = timeit(lambda: cgetprimes(n), number=m)
print('python pure', tppy)
print('c++ ext', tcpy)
print(tppy / tcpy)
