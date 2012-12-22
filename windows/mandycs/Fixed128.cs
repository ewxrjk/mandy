/* Copyright © 2012 Richard Kettlewell.
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
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace uk.org.greenend.mandy
{
  /// <summary>
  /// Possible precisions
  /// </summary>
  public enum Precision
  {
    /// <summary>
    /// Perform calculations using C "double" type
    /// </summary>
    Double,

    /// <summary>
    /// Perform calculations using C "long double" type
    /// </summary>
    LongDouble,

    /// <summary>
    /// Perform calculations using 64-bit fixed point
    /// </summary>
    Fixed64,

    /// <summary>
    /// Perform calculations using 128-bit fixed point
    /// </summary>
    Fixed128,
  };

  /// <summary>
  /// 128-bit fixed point arithmetic
  /// </summary>
  /// <remarks><para>You get 32 bits of integer part and 96 bit of fractional part.</para></remarks>
  [StructLayout(LayoutKind.Sequential)]
  public struct Fixed128
  {
    /// <summary>
    /// Three fractional words and the integer part
    /// </summary>
    /// <remarks><para>Negative values are indicate using two's complement.</para></remarks>
    public uint f3, f2, f1, i;

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="i">Integer part</param>
    /// <param name="f1">Top 32 bits of fractional part</param>
    /// <param name="f2">Middle 32 bits of fractional part</param>
    /// <param name="f3">Bottom 32 bits of fractional part</param>
    public Fixed128(int i, uint f1, uint f2, uint f3)
    {
      this.i = (uint)i;
      this.f1 = f1;
      this.f2 = f2;
      this.f3 = f3;
    }

    #region Conversions

    /// <summary>
    /// Cast from an int.
    /// </summary>
    /// <param name="n">Integer value</param>
    /// <returns>Converted value</returns>
    public static implicit operator Fixed128(int n)
    {
      return new Fixed128(n, 0, 0, 0);
    }

    /// <summary>
    /// Convert string to fixed-point
    /// </summary>
    /// <param name="s">String containing decimal value to convert</param>
    /// <returns>Value</returns>
    /// <exception cref="FormatException">Thrown if the value is out of range or malformed.</exception>
    public static implicit operator Fixed128(string s)
    {
      Fixed128 r = 0;
      int error = Fixed128_str2_cs(ref r, s);
      switch(error)
      {
        case 0: // FIXED128_STR_OK
          break;
        case 1: // FIXED128_STR_RANGE
          throw new FormatException("number out of range"); // TODO better choice?
        case 2: // FIXED128_STR_FORMAT
          throw new FormatException("invalid numeric format"); // TODO better choice?
      }
      return r;
    }

    /// <summary>
    /// Convert fixed-point to string
    /// </summary>
    /// <param name="n">Value to convert</param>
    /// <returns>String containing decimal equivalent of value</returns>
    public unsafe static implicit operator string(Fixed128 n)
    {
      const int BUFSIZE = 256;
      byte* buffer = stackalloc byte[BUFSIZE];
      Fixed128_2str(buffer, (IntPtr)BUFSIZE, ref n, 10);
      return Marshal.PtrToStringAnsi((IntPtr)buffer);
    }

    /// <summary>
    /// Convert fixed-point to double
    /// </summary>
    /// <param name="n">Value to convert</param>
    /// <returns>Converted value</returns>
    public static implicit operator double(Fixed128 n)
    {
      return Fixed128_2double(ref n);
    }

    /// <summary>
    /// Convert double to fixed-point
    /// </summary>
    /// <param name="n">Value to convert</param>
    /// <returns>Converted value</returns>
    public static implicit operator Fixed128(double n)
    {
      Fixed128 r = 0;
      Fixed128_double2(ref r, n);
      return r;
    }

    #endregion

    #region Arithmetic Operations
    
    /// <summary>
    /// Add fixed-point values
    /// </summary>
    /// <param name="a">First value</param>
    /// <param name="b">Second value</param>
    /// <returns><code>a+b</code></returns>
    public static Fixed128 operator +(Fixed128 a, Fixed128 b)
    {
      Fixed128 r = 0;
      Fixed128_add(ref r, ref a, ref b);
      return r;
    }

    /// <summary>
    /// Subtract fixed-point values
    /// </summary>
    /// <param name="a">First value</param>
    /// <param name="b">Second value</param>
    /// <returns><code>a-b</code></returns>
    public static Fixed128 operator -(Fixed128 a, Fixed128 b)
    {
      Fixed128 r = 0;
      Fixed128_sub(ref r, ref a, ref b);
      return r;
    }

    /// <summary>
    /// Multiply fixed-point values
    /// </summary>
    /// <param name="a">First value</param>
    /// <param name="b">Second value</param>
    /// <returns><code>a*b</code></returns>
    public static Fixed128 operator *(Fixed128 a, Fixed128 b)
    {
      Fixed128 r = 0;
      Fixed128_mul(ref r, ref a, ref b);
      return r;
    }

    /// <summary>
    /// Divide fixed-point values
    /// </summary>
    /// <param name="a">First value</param>
    /// <param name="b">Second value</param>
    /// <returns><code>a/b</code></returns>
    public static Fixed128 operator /(Fixed128 a, Fixed128 b)
    {
      // TODO division by 0?
      Fixed128 r = 0;
      Fixed128_div(ref r, ref a, ref b);
      return r;
    }

    /// <summary>
    /// Negate fixed-point value
    /// </summary>
    /// <param name="a">Value</param>
    /// <returns><code>-a</code></returns>
    public static Fixed128 operator -(Fixed128 a)
    {
      Fixed128 r = 0;
      Fixed128_neg(ref r, ref a);
      return r;
    }

    /// <summary>
    /// Square root fixed-point value
    /// </summary>
    /// <returns>Square root of this</returns>
    public Fixed128 sqrt()
    {
      // TODO negative values
      Fixed128 r = 0;
      Fixed128_sqrt(ref r, ref this);
      return r;
    }

    /// <summary>
    /// Compare fixed-point values for equality
    /// </summary>
    /// <param name="a">First value</param>
    /// <param name="b">Second value</param>
    /// <returns>true if <code>a==b</code>, else false</returns>
    public static bool operator ==(Fixed128 a, Fixed128 b)
    {
      return Fixed128_eq(ref a, ref b) != 0;
    }

    /// <summary>
    /// Compare fixed-point values for inequality
    /// </summary>
    /// <param name="a">First value</param>
    /// <param name="b">Second value</param>
    /// <returns>true if <code>a!=b</code>, else false</returns>
    public static bool operator !=(Fixed128 a, Fixed128 b)
    {
      return Fixed128_eq(ref a, ref b) == 0;
    }

    /// <summary>
    /// Compare fixed-point values for order
    /// </summary>
    /// <param name="a">First value</param>
    /// <param name="b">Second value</param>
    /// <returns>true if <code>a&lt;b</code>, else false</returns>
    public static bool operator <(Fixed128 a, Fixed128 b)
    {
      return Fixed128_lt(ref a, ref b) != 0;
    }

    /// <summary>
    /// Compare fixed-point values for order
    /// </summary>
    /// <param name="a">First value</param>
    /// <param name="b">Second value</param>
    /// <returns>true if <code>a&gt;=b</code>, else false</returns>
    public static bool operator >=(Fixed128 a, Fixed128 b)
    {
      return Fixed128_lt(ref a, ref b) == 0;
    }

    /// <summary>
    /// Compare fixed-point values for order
    /// </summary>
    /// <param name="a">First value</param>
    /// <param name="b">Second value</param>
    /// <returns>true if <code>a&gt;b</code>, else false</returns>
    public static bool operator >(Fixed128 a, Fixed128 b)
    {
      return Fixed128_lt(ref b, ref a) != 0;
    }

    /// <summary>
    /// Compare fixed-point values for order
    /// </summary>
    /// <param name="a">First value</param>
    /// <param name="b">Second value</param>
    /// <returns>true if <code>a&lt;=b</code>, else false</returns>
    public static bool operator <=(Fixed128 a, Fixed128 b)
    {
      return Fixed128_lt(ref b, ref a) == 0;
    }

    #endregion

    #region Algorithms

    /// <summary>
    /// Calculate Mandelbrot set iteration
    /// </summary>
    /// <param name="zx">Starting real part</param>
    /// <param name="zy">Starting imaginary part</param>
    /// <param name="cx">Offset constant real part</param>
    /// <param name="cy">Offset constant imaginary part</param>
    /// <param name="maxiters">Maximum number of iterations</param>
    /// <param name="precision">Precision to use</param>
    /// <returns>Adjusted iteration count</returns>
    public static double iterate(Fixed128 zx, Fixed128 zy,
                                 Fixed128 cx, Fixed128 cy,
                                 int maxiters, Precision precision)
    {
      return iterate_cs(ref zx, ref zy, ref cx, ref cy, maxiters, (int)precision);
    }

    #endregion

    #region Unmanaged code

    [DllImport("libmandy.dll")]
    private static extern void Fixed128_add(ref Fixed128 r,
                                            ref Fixed128 a,
                                            ref Fixed128 b);

    [DllImport("libmandy.dll")]
    private static extern void Fixed128_sub(ref Fixed128 r,
                                            ref Fixed128 a,
                                            ref Fixed128 b);

    [DllImport("libmandy.dll")]
    private static extern void Fixed128_mul(ref Fixed128 r,
                                            ref Fixed128 a,
                                            ref Fixed128 b);

    [DllImport("libmandy.dll")]
    private static extern void Fixed128_div(ref Fixed128 r,
                                            ref Fixed128 a,
                                            ref Fixed128 b);

    [DllImport("libmandy.dll")]
    private static extern void Fixed128_neg(ref Fixed128 r,
                                            ref Fixed128 a);

    [DllImport("libmandy.dll")]
    private static extern void Fixed128_sqrt(ref Fixed128 r,
                                             ref Fixed128 a);

    [DllImport("libmandy.dll")]
    private static extern int Fixed128_eq(ref Fixed128 r,
                                          ref Fixed128 a);

    [DllImport("libmandy.dll")]
    private static extern int Fixed128_lt(ref Fixed128 r,
                                          ref Fixed128 a);

    [DllImport("libmandy.dll")]
    private static extern int Fixed128_str2_cs(ref Fixed128 r, string s);

    [DllImport("libmandy.dll")]
    private unsafe static extern IntPtr Fixed128_2str(byte *buffer, IntPtr bufsize,
                                                      ref Fixed128 a, int radix);

    [DllImport("libmandy.dll")]
    private static extern void Fixed128_double2(ref Fixed128 r, double n);

    [DllImport("libmandy.dll")]
    private static extern double Fixed128_2double(ref Fixed128 a);

    [DllImport("libmandy.dll")]
    private static extern double iterate_cs(ref Fixed128 zx, ref Fixed128 zy,
                                            ref Fixed128 cx, ref Fixed128 cy,
                                            int maxiters, int arith); 

    #endregion

  }
}
