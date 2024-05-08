#!/usr/bin/python3
import random
import sys
from functools import total_ordering
from math import floor

BITS=256
INTBITS=64
FRACBITS=192
SIGNBIT=BITS-1
SIGNMASK=1<<SIGNBIT
BOUND = 1<<BITS
MASK = BOUND-1
FRACMASK = (1<<FRACBITS)-1
DIGITS = "0123456789abcdefghijklmnopqrstuvwxyz"

@total_ordering
class Fixed256:
    def __init__(self, n):
        if isinstance(n, float):
            sign = 0
            if n < 0.0:
                sign = -1
                n = -n
            ipart = int(n)
            fpart = n - ipart
            self.n = (int(ipart) << 192) + int(fpart * 6277101735386680763835789423207666416102355444464034512896.0)
            if sign:
                self.n = BOUND - self.n
        elif isinstance(n, Fixed256):
            self.n = n.n
        else:
            self.n = n & MASK
    
    def __add__(self, other):
        return Fixed256(self.n + Fixed256(other).n)
    
    def __sub__(self, other):
        return Fixed256(self.n + BOUND - Fixed256(other).n)
    
    def __mul__(self, other):
        p = (self.n * Fixed256(other).n)
        roundbit = (p >> (FRACBITS-1)) & 1
        return Fixed256((p >> FRACBITS) + roundbit)
    
    def __truediv__(self, other):
        q = (self.n << FRACBITS) // Fixed256(other).n
        return Fixed256(q)

    def __neg__(self):
        return Fixed256(BOUND - self.n)

    def u32(self, i:int) -> int:
        return (self.n>>(32*i)) & 0xFFFFFFFF
    
    def u64(self, i:int) -> int:
        return (self.n>>(64*i)) & 0xFFFFFFFFFFFFFFFF

    def _signed(self) -> int:
        if self.n & SIGNMASK:
            return self.n - BOUND
        else:
            return self.n

    def C(self) -> str:
        words = ", ".join([f"0x{self.u64(i):016x}" for i in range(0, BITS//64)])
        return f"{{ .u64= {{ {words} }} }}"

    def __str__(self) -> str:
        return self.convert(10)

    def __lt__(self, other) -> bool:
        return self._signed() < other._signed()

    def __eq__(self, other) -> bool:
        return self.n == other.n

    def convert(self, base=10):
        sign = ''
        if self < Fixed256(0):
            sign = '-'
            n = (-self).n
        else:
            n = self.n
        assert n >= 0
        intpart = n >> FRACBITS
        intdigits = []
        while intpart > 0 or len(intdigits) == 0:
            intdigits.append(DIGITS[intpart % base])
            intpart //= base
        fracdigits = []
        n &= FRACMASK
        while n > 0:
            n *= base
            fracdigits.append(DIGITS[n >> FRACBITS])
            n &= FRACMASK
        intdigits = "".join(reversed(intdigits))
        if len(fracdigits) > 0:
            fracdigits = "".join(fracdigits)
            result = f"{sign}{intdigits}.{fracdigits}"
        else:
            result =  f"{sign}{intdigits}"
        return result

BINARY = {
    "add": lambda a, b: a+b,
    "sub": lambda a, b: a-b,
    "mul": lambda a, b: a*b,
    "div": lambda a, b: a/b,
}

COMPARE = {
    "lt": lambda a, b: a<b,
    "gt": lambda a, b: a>b,
    "le": lambda a, b: a<=b,
    "ge": lambda a, b: a>=b,
    "eq": lambda a, b: a==b,
    "ne": lambda a, b: a!=b,
}

UNARY = {
    "neg": lambda a: -a,
    "square": lambda a: a*a,
}

for i, op in enumerate(UNARY.keys()):
    print(f"static void fixed256_test_{op}(void) {{")
    print(f"  union Fixed{BITS} got;")
    rnd = random.Random(i+1)
    for j in range(0, 16):
        a = Fixed256(rnd.randrange(0, 1<<(BITS-32)))
        e = UNARY[op](a)
        print(f"  {{")
        print(f"    const union Fixed{BITS} a = {a.C()};")
        print(f"    const union Fixed{BITS} expect = {e.C()};")
        print(f"    Fixed{BITS}_{op}(&got, &a);")
        print(f"    CHECK(got, expect);")
        print(f"  }}")
    print(f"}}")
    print()

for i, op in enumerate(BINARY.keys()):
    print(f"static void fixed256_test_{op}(void) {{")
    print(f"  union Fixed{BITS} got;")
    rnd = random.Random(i+1)
    for j in range(0, 16):
        a = Fixed256(rnd.randrange(0, 1<<(BITS-32)))
        b = Fixed256(rnd.randrange(0, 1<<(BITS-32)))
        e = BINARY[op](a, b)
        print(f"  {{")
        print(f"    const union Fixed{BITS} a = {a.C()};")
        print(f"    const union Fixed{BITS} b = {b.C()};")
        print(f"    const union Fixed{BITS} expect = {e.C()};")
        print(f"    Fixed{BITS}_{op}(&got, &a, &b);")
        print(f"    CHECK(got, expect);")
        print(f"  }}")
    print(f"}}")
    print()

print(f"static void fixed256_test_compare(void) {{")
print(f"  bool got;")
rnd = random.Random(i+1)
pairs = []
edges = [Fixed256(1<<(BITS-1)), Fixed256(1<<FRACBITS), Fixed256(0), Fixed256((1<<(BITS-1))-1)]
for a in edges:
    for b in edges:
        pairs.append((a, b))
for j in range(0, 2):
    a = Fixed256(rnd.randrange(0, 1<<BITS))
    pairs.append((a, a))
for j in range(0, 14):
    a = Fixed256(rnd.randrange(0, 1<<BITS))
    b = Fixed256(rnd.randrange(0, 1<<BITS))
    pairs.append((a, b))
for a, b in pairs:
    print(f"  {{")
    print(f"    const union Fixed{BITS} a = {a.C()};")
    print(f"    const union Fixed{BITS} b = {b.C()};")
    for op in COMPARE.keys():
        e = COMPARE[op](a, b)
        e = str(e).lower()
        print(f"    got = Fixed{BITS}_{op}(&a, &b);")
        print(f"    CHECK_BOOL(got, {e});")
    print(f"  }}")
print(f"}}")
print()

print(f"static void fixed256_test_compare_unsigned(void) {{")
print(f"  bool got;")
for j in range(0, 16):
    a = Fixed256(rnd.randrange(0, 1<<BITS))
    if j < 2:
        b = a
    else:
        b = Fixed256(rnd.randrange(0, 1<<BITS))
    print(f"  {{")
    print(f"    const union Fixed{BITS} a = {a.C()};")
    print(f"    const union Fixed{BITS} b = {b.C()};")
    for op in COMPARE.keys():
        if op == 'eq' or op == 'ne':
            continue
        e = COMPARE[op](a.n, b.n)
        e = str(e).lower()
        print(f"    got = Fixed{BITS}_{op}_unsigned(&a, &b);")
        print(f"    CHECK_BOOL(got, {e});")
    print(f"  }}")
print(f"}}")
print()


print(f"static void fixed256_test_2str(void) {{")
print("  char buffer[1024];")
rnd = random.Random(1)
# TODO -ve values
values = [Fixed256(0),
          Fixed256(1<<FRACBITS),
          -Fixed256(1<<FRACBITS),
          Fixed256((1<<(BITS-1))-1),
          Fixed256(-((1<<(BITS-1)))),
          ] + [Fixed256(rnd.randrange(0, 1<<(BITS-32))) for i in range(0, 16)]
for a in values:
    print(f"  {{")
    print(f"    const union Fixed{BITS} a = {a.C()};")
    for base in [10, 16]:
        print(f"    Fixed256_2str(buffer, sizeof buffer, &a, {base});")
        print(f'    CHECK_STR(buffer, "{a.convert(base)}");')
    print(f"  }}")
print(f"}}")
print()

print(f"static void fixed256_test_str2(void) {{")
print(f"  union Fixed{BITS} got;")
rnd = random.Random(1)
# TODO -ve values
values = [Fixed256(0), Fixed256(1<<FRACBITS), -Fixed256(1<<FRACBITS)] + [Fixed256(rnd.randrange(0, 1<<(BITS-32))) for i in range(0, 16)]
for a in values:
    a = Fixed256(rnd.randrange(0, 1<<(BITS-32)))
    print(f"  {{")
    print(f"    const union Fixed{BITS} expect = {a.C()};")
    print(f'    Fixed256_str2(&got, "{a.convert()}", NULL);')
    print(f"    CHECK(got, expect);")
    print(f"  }}")
print(f"}}")
print()

print(f"static void fixed256_test_sqrt(void) {{")
print(f"  union Fixed{BITS} got;")
rnd = random.Random(1)
values = []
for i in range(0, 16):
    e = Fixed256(rnd.randrange(0, 1<<(BITS-33)))
    values.append((e*e, e))
for a,e in values:
    print(f"  {{")
    print(f"    const union Fixed{BITS} a = {a.C()};")
    print(f"    const union Fixed{BITS} expect = {e.C()};")
    print(f'    Fixed256_sqrt(&got, &a);')
    print(f"    CHECK(got, expect);")
    print(f"  }}")
print(f"}}")
print()

print(f"static void fixed256_test_double2(void) {{")
print(f"  union Fixed{BITS} got;")
rnd = random.Random(1)
for n in range(-192, 64, 8):
    if n < 0:
        a = random.uniform(-1.0 / (1<<-n), 1.0 / (1<<-n))
    else:
        a = random.uniform(-(1<<n), (1<<n)-1)
    e = Fixed256(a)
    print(f"  {{")
    print(f"    const union Fixed{BITS} expect = {e.C()};")
    print(f'    Fixed256_double2(&got, {a});')
    print(f"    CHECK(got, expect);")
    print(f"  }}")
print(f"}}")
print()

print(f"static void fixed256_test_2double(void) {{")
rnd = random.Random(1)
for n in range(-192+64, 64, 8):
    if n < 0:
        e = random.uniform(-1.0 / (1<<-n), 1.0 / (1<<-n))
    else:
        e = random.uniform(-(1<<n), (1<<n)-1)
    a = Fixed256(e)
    print(f"  {{")
    print(f"    const union Fixed{BITS} a = {a.C()};")
    print(f'    double got = Fixed256_2double(&a);')
    print(f"    CHECK_DOUBLE(got, {e});")
    print(f"  }}")
print(f"}}")
print()

