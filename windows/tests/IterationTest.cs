using System;
using Microsoft.VisualStudio.TestTools.UnitTesting;
using uk.org.greenend.mandy;

namespace tests
{
  [TestClass]
  public class IterationTest
  {
    /// <summary>
    /// Tests for Fix128
    /// </summary>
    /// <remarks>
    /// <para>Only the 64-bit configuration is testable.</para></remarks>
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
