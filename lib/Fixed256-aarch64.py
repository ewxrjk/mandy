#!/usr/bin/python3
import sys

def generic_header():
    print(f"""/* Generated by {sys.argv[0]} */

/* Copyright © Richard Kettlewell.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <config.h>
""")

def a64_load(ptr, r):
    assert ptr not in r[:7]
    rd = ":".join(reversed(r))
    print(f"  // {rd} <- [{ptr}]")
    print(f"  ldp {r[0]},{r[1]},[{ptr}]")
    print(f"  ldp {r[2]},{r[3]},[{ptr},16]")

def a64_store(ptr, r):
    rd = ":".join(reversed(r))
    print(f"  // [{ptr}] <- {rd}")
    print(f"  stp {r[0]},{r[1]},[{ptr}]")
    print(f"  stp {r[2]},{r[3]},[{ptr},16]")

def a64_neg(r):
    rd = ":".join(reversed(r))
    print(f"  // negate {rd}")
    print(f"  negs {r[0]},{r[0]}")
    print(f"  ngcs {r[1]},{r[1]}")
    print(f"  ngcs {r[2]},{r[2]}")
    print(f"  ngc {r[3]},{r[3]}")
    

def a64_header():
    print("""#if __aarch64__
#define SYMBOL(s) s
""")

def a64_footer():
    print("""#endif
""")

def a64_op(op):
    print(f".text")
    print(f".align 4")
    print(f".globl SYMBOL(Fixed256_{op})")
    print(f"SYMBOL(Fixed256_{op}):")
    # x0 = r, x1 = a, x2 = b

    # Register assignment:
    #
    # x0    r
    # x1    a
    # x2    b
    # x3
    # x4
    # x5
    # x6
    # x7
    # x8
    # x9
    # x10
    # x11
    # x12
    # x13
    # x14
    # x15
    # x17
    # x18
    # x19
    # x20
    # x21
    # x22
    # x23
    # x24
    # x25
    # x26
    # x27
    # x28
    # x29
    # x30

    a = ["x3", "x4", "x5", "x6"]
    r = ["x7", "x8", "x9", "x10"]
    if op == "mul":
        b = ["x11", "x12", "x13", "x14"]
    else:
        b = a
    t0 = "x1"
    t1 = "x2"

    # Registers that we touch
    SMASH = set(a + b + r + [t0, t1])

    # Registers that must be saved
    SAVE = [f"x{n}" for n in range(16, 31)]

    # Limit to those that we touch
    SAVE = [reg for reg in SAVE if reg in SMASH]

    # Push callee-saved registers
    saved_bytes = 8 * len(SAVE)
    saved_bytes += saved_bytes % 16
    if saved_bytes > 0:
        print(f"  sub sp,sp,#{saved_bytes}")
    for i in range(0, len(SAVE), 2):
        if i+1 < len(SAVE):
            print(f"  stp {SAVE[i]},{SAVE[i+1]},[sp,{8*i}]")
        else:
            print(f"  str {SAVE[i]},[sp,{8*i}]")

    print(f"// Load operands")
    a64_load("x1", a)
    if op == "mul":
        a64_load("x2", b)
    print(f"// Absolute value of a")
    print(f"  tbz {a[3]},#63,1f")
    a64_neg(a)
    if op == "mul":
        print(f"  eor x0,x0,#1")
    print(f"1:")
    if op == "mul":
        print(f"// Absolute value of b")
        print(f"  tbz {b[3]},#63,2f")
        a64_neg(b)
        print(f"  eor x0,x0,#1")
        print(f"2:")
    
    # We calculate the full product so we can round correctly
    #
    # We will do 64x64->128 multiplies and use three 64-bit registers
    # to accumulate the sum for each column, including the carry
    # up from previous columns. 
    #
    # We will 'rotate' the accumulator through different registers
    # to achieve this.

    MAXDEPTH=-3
    DEPTH=-3

    assert DEPTH >= MAXDEPTH

    # Moving accumulator
    c = r[-3:] + r + [None, None]

    # State cache
    state = {}

    def row(n, c, state, add=True):
        cname = ":".join([r for r in reversed(c) if r != None])
        print(f"// Compute r[{n}] using {cname}")
        assert len(c) == 3
        # New top word starts out 0
        state[c[2]] = "xzr"
        for x in range(0, 4):
            y = n + 3 - x
            if x < 0 or x > 7 or y < 0 or y > 3:
                continue
            if op == "square" and x < y:
                continue
            if add:
                if c[1] != None:
                    print(f"  // {t1}:{t0} <- a[{x}] * b[{y}]")
                    print(f"  mul {t0},{a[x]},{b[y]}")
                    print(f"  umulh {t1},{a[x]},{b[y]}")
                    # mul needs one add, squaring needs two
                    # (we optimize out a repeated multiply)
                    adds = [f"a[{x}] * b[{y}]"]
                    if op == "square" and x > y:
                        adds.append(f"a[{y}] * b[{x}]")
                    # Final add in each change doesn't need the 's' to carry out
                    if c[2] != None:
                        s = ["s", "s", ""]
                    elif c[1] != None:
                        s = ["s", "", ""]
                    else:
                        s = ["", "", ""]
                    for add in adds:
                        print(f"  // {cname} += {add}")
                        print(f"  add{s[0]} {c[0]},{c[0]},{t0}")
                        if c[1] != None:
                            if c[1] in state:
                                print(f"  adc{s[1]} {c[1]},xzr,{t1}")
                                del state[c[1]]
                            else:
                                print(f"  adc{s[1]} {c[1]},{c[1]},{t1}")
                        if c[2] != None:
                            if c[2] in state:
                                print(f"  adc{s[2]} {c[2]},xzr,xzr")
                                del state[c[2]]
                            else:
                                print(f"  adc{s[2]} {c[2]},{c[2]},xzr")
                else:
                    # Top word; we can go straight to madd
                    #
                    # (We can't use madd elsewhere as it never sets flags)
                    assert n == 3
                    print(f"  // {cname} += a[{x}] * b[{y}]")
                    print(f"  madd {c[0]},{a[x]},{b[y]},{c[0]}")
            else:
                # Bottom underflow word; we only need the overflow up into
                # the next underflow word.
                assert n == MAXDEPTH
                print(f"  // {c[1]}:<ignored> <- a[{x}] * b[{y}]")
                print(f"  umulh {c[1]},{a[x]},{b[y]}")
                add = True

    for i in range(MAXDEPTH, 0):
        if i >= DEPTH:
            row(i, c[:3], state, i != DEPTH)
        if i == -1:
            print("// Round up")
            print(f"  adds xzr,{c[0]},{c[0]}") 
            print(f"  adcs {c[1]},{c[1]},xzr")
            print(f"  adc {c[2]},{c[2]},xzr")
        # Move the accumulator
        c = c[1:]

    for n in range(0, 4):
        row(n, c[:3], state)
        # Move the accumulator
        c = c[1:]

    if op == "mul":
        print("// Set sign")
        print(f"  tbz x0,#0,3f")
        a64_neg(r)
        print(f"  eor x0,x0,#1")
        print("3:")
    
    print("// Store result")
    a64_store("x0", r)


    # Pop callee-saved registers
    for i in range(0, len(SAVE), 2):
        if i+1 < len(SAVE):
            print(f"  ldp {SAVE[i]},{SAVE[i+1]},[sp,{16*i}]")
        else:
            print(f"  ldr {SAVE[i]},[sp,{8*i}]")

    if saved_bytes > 0:
        print(f"  add sp,sp,#{saved_bytes}")
    print(f"  ret")


generic_header()
a64_header()
a64_op("mul")
a64_op("square")
a64_footer()
