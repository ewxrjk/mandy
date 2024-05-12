#!/usr/bin/python3
from random import Random
import sys
from functools import total_ordering
from math import floor

DIGITS = "0123456789abcdefghijklmnopqrstuvwxyz"

def Fixed(INTBITS:int, FRACBITS: int):

    BITS = INTBITS + FRACBITS
    SIGNBIT = BITS-1
    SIGNMASK=1<<SIGNBIT
    BOUND = 1<<BITS
    MASK = BOUND-1
    FRACMASK = (1<<FRACBITS)-1
    FRACMUL = float(1<<FRACBITS)

    @total_ordering
    class FixedN:
        def __init__(self, n):
            if isinstance(n, float):
                sign = 0
                if n < 0.0:
                    sign = -1
                    n = -n
                ipart = int(n)
                fpart = n - ipart
                self.n = (int(ipart) << FRACBITS) + int(fpart * FRACMUL)
                if sign:
                    self.n = BOUND - self.n
            elif isinstance(n, FixedN):
                self.n = n.n
            else:
                self.n = n & MASK
        
        def __add__(self, other):
            return FixedN(self.n + FixedN(other).n)
        
        def __sub__(self, other):
            return FixedN(self.n + BOUND - FixedN(other).n)
        
        def __mul__(self, other):
            p = (self.n * FixedN(other).n)
            roundbit = (p >> (FRACBITS-1)) & 1
            return FixedN((p >> FRACBITS) + roundbit)
        
        def __truediv__(self, other):
            q = (self.n << FRACBITS) // FixedN(other).n
            return FixedN(q)

        def __neg__(self):
            return FixedN(BOUND - self.n)

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
            if self < FixedN(0):
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

    FixedN.BITS = BITS
    FixedN.FRACBITS = FRACBITS
    FixedN.INTBITS = INTBITS
    FixedN.NAME = f"Fixed{BITS}"

    return FixedN

CLASSES = {
    "Fixed128": Fixed(32, 96),
    "Fixed256": Fixed(64, 192),
}

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

C = CLASSES[sys.argv[1]]

BITS = C.BITS
INTBITS = C.INTBITS
FRACBITS = C.FRACBITS
TESTNAME = C.NAME.lower()

for i, op in enumerate(UNARY.keys()):
    print(f"static void {TESTNAME}_test_{op}(void) {{")
    print(f"  union Fixed{BITS} got;")
    rnd = Random(i+1)
    for j in range(0, 16):
        a = C(rnd.randrange(0, 1<<(BITS-INTBITS//2)))
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
    print(f"static void {TESTNAME}_test_{op}(void) {{")
    print(f"  union Fixed{BITS} got;")
    rnd = Random(i+1)
    for j in range(0, 16):
        a = C(rnd.randrange(0, 1<<(BITS-INTBITS//2)))
        b = C(rnd.randrange(0, 1<<(BITS-INTBITS//2)))
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

print(f"static void {TESTNAME}_test_compare(void) {{")
print(f"  bool got;")
rnd = Random(i+1)
pairs = []
edges = [C(1<<(BITS-1)), C(1<<FRACBITS), C(0), C((1<<(BITS-1))-1)]
for a in edges:
    for b in edges:
        pairs.append((a, b))
for j in range(0, 2):
    a = C(rnd.randrange(0, 1<<BITS))
    pairs.append((a, a))
for j in range(0, 14):
    a = C(rnd.randrange(0, 1<<BITS))
    b = C(rnd.randrange(0, 1<<BITS))
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

print(f"static void {TESTNAME}_test_compare_unsigned(void) {{")
print(f"  bool got;")
for j in range(0, 16):
    a = C(rnd.randrange(0, 1<<BITS))
    if j < 2:
        b = a
    else:
        b = C(rnd.randrange(0, 1<<BITS))
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


print(f"static void {TESTNAME}_test_2str(void) {{")
print("  char buffer[1024];")
rnd = Random(1)
# TODO -ve values
values = [C(0),
        C(1<<FRACBITS),
        -C(1<<FRACBITS),
        C((1<<(BITS-1))-1),
        C(-((1<<(BITS-1)))),
        ] + [C(rnd.randrange(0, 1<<(BITS-INTBITS//2))) for i in range(0, 16)]
for a in values:
    print(f"  {{")
    print(f"    const union Fixed{BITS} a = {a.C()};")
    for base in [10, 16]:
        print(f"    {C.NAME}_2str(buffer, sizeof buffer, &a, {base});")
        print(f'    CHECK_STR(buffer, "{a.convert(base)}");')
    print(f"  }}")
print(f"}}")
print()

print(f"static void {TESTNAME}_test_str2(void) {{")
print(f"  union Fixed{BITS} got;")
rnd = Random(1)
# TODO -ve values
values = [C(0), C(1<<FRACBITS), -C(1<<FRACBITS)] + [C(rnd.randrange(0, 1<<(BITS-INTBITS//2))) for i in range(0, 16)]
for a in values:
    a = C(rnd.randrange(0, 1<<(BITS-INTBITS//2)))
    print(f"  {{")
    print(f"    const union Fixed{BITS} expect = {a.C()};")
    print(f'    {C.NAME}_str2(&got, "{a.convert()}", NULL);')
    print(f"    CHECK(got, expect);")
    print(f"  }}")
print(f"}}")
print()

print(f"static void {TESTNAME}_test_sqrt(void) {{")
print(f"  union Fixed{BITS} got;")
rnd = Random(1)
values = []
for i in range(0, 16):
    e = C(rnd.randrange(0, 1<<(BITS-INTBITS//2-1)))
    values.append((e*e, e))
for a,e in values:
    print(f"  {{")
    print(f"    const union Fixed{BITS} a = {a.C()};")
    print(f"    const union Fixed{BITS} expect = {e.C()};")
    print(f'    {C.NAME}_sqrt(&got, &a);')
    print(f"    CHECK(got, expect);")
    print(f"  }}")
print(f"}}")
print()

print(f"static void {TESTNAME}_test_double2(void) {{")
print(f"  union Fixed{BITS} got;")
rnd = Random(1)
for n in range(-FRACBITS, INTBITS, 8):
    if n < 0:
        a = rnd.uniform(-1.0 / (1<<-n), 1.0 / (1<<-n))
    else:
        a = rnd.uniform(-(1<<n), (1<<n)-1)
    e = C(a)
    print(f"  {{")
    print(f"    const union Fixed{BITS} expect = {e.C()};")
    print(f'    {C.NAME}_double2(&got, {a});')
    print(f"    CHECK(got, expect);")
    print(f"  }}")
print(f"}}")
print()

print(f"static void {TESTNAME}_test_2double(void) {{")
rnd = Random(1)
for n in range(-FRACBITS+64, INTBITS, 8):
    if n < 0:
        e = rnd.uniform(-1.0 / (1<<-n), 1.0 / (1<<-n))
    else:
        e = rnd.uniform(-(1<<n), (1<<n)-1)
    a = C(e)
    print(f"  {{")
    print(f"    const union Fixed{BITS} a = {a.C()};")
    print(f'    double got = {C.NAME}_2double(&a);')
    print(f"    CHECK_DOUBLE(got, {e});")
    print(f"  }}")
print(f"}}")
print()

