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
using Microsoft.VisualStudio.TestTools.UnitTesting;
using uk.org.greenend.mandy;

namespace tests
{
  /// <summary>
  /// Test for C# exposure of the underlying iteration function.
  /// </summary>
  /// <remarks>
  /// <para>Only the 64-bit configuration is testable.</para></remarks>
  [TestClass]
  public class IterationTest
  {
    [TestMethod]
    [DeploymentItem(@"..\..\..\..\x64\Debug\libmandy.dll")]
    public void IterateTest()
    {
      Fixed128 zx = 0, zy = 0;
      Fixed128 cx = new Fixed128(0, 0xa6aaaaaau, 0xaaaaaaaau, 0xaaaaaaabu);
      Fixed128 cy = new Fixed128(-1, 0xfd555555u, 0x55555555u, 0x55555555u);
      Assert.AreEqual("0.651041666666666666666666666670873924827845396295529219014841526558257100987248122692108154296875",
                      (string)cx);
      Assert.AreEqual("-0.010416666666666666666666666670873924827845396295529219014841526558257100987248122692108154296875",
                      (string)cy);
      double count = Fixed128.iterate(zx, zy, cx, cy, 255, Precision.Double);
      double expected = 1 + 5 - Math.Log(Math.Log(255.08471462316811, 2), 2);
      Assert.AreEqual(count, expected);
      count = Fixed128.iterate(zx, zy, cx, cy, 255, Precision.LongDouble);
      Assert.AreEqual(count, expected);
      count = Fixed128.iterate(zx, zy, cx, cy, 255, Precision.Fixed64);
      Assert.AreEqual(count, expected);
      count = Fixed128.iterate(zx, zy, cx, cy, 255, Precision.Fixed128);
      Assert.AreEqual(count, expected);
    }
  }
}
