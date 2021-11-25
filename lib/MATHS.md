# Mathematical Details

## Algorithm

We operate in ℂ, or rather an approximation to ℂ that can be represented in a computer.

The basic algorithm is:
1. Let C be any point in ℂ.
2. Let Z = 0, iterations=0
3. If |Z|≥8 or iterations≥maxiters, go to 6
4. Let Z+1 = Z^2 + C, maxiters=maxiters+1
5. Go to 3
6. Plot a color related to iterations,Z at position C.x, C.y

The iterative step is expanded as follows:

```
  (Z.x + iZ.y)^2 + (C.x + iC.y)
= Z.x^2 - Z.y^2 + C.x + i(2Z.xZ.y + C.y)
```

## Optimizations

* For 64-bit and 128-bit fixed point the iterative loop is in hand-coded assembler.
* 128-bit squarings don't calculate the sign of the result, since it's always positive.

## SIMD

We need to, in parallel:
* Detect columns that have escaped (i.e. |Z|≥8)
* Preserve the iteration count and Z at the escape point
* Compute the iterative step for other values
* Stop when all columns have escape

