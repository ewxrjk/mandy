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
  /// Tests for Fix128
  /// </summary>
  /// <remarks><para>These tests aren't particularly exhaustive;
  /// use fixed128-test for more exhaustive (and portable) testing.</para>
  /// <para>Only the 64-bit configuration is testable.</para></remarks>
  [TestClass]
  [DeploymentItem(@"..\..\..\..\x64\Debug\libmandy.dll")]
  public class Fixed128Test
  {
    [TestMethod]
    public void AddTest()
    {
      Fixed128 a = 1;
      Fixed128 b = 2;
      Fixed128 r = a + b;
      Assert.AreEqual(3u, r.i);
      Assert.AreEqual(0u, r.f1);
      Assert.AreEqual(0u, r.f2);
      Assert.AreEqual(0u, r.f3);
    }

    [TestMethod]
    public void SubTest()
    {
      Fixed128 a = 1;
      Fixed128 b = 2;
      Fixed128 r = a - b;
      Assert.AreEqual(0xFFFFFFFFu, r.i);
      Assert.AreEqual(0u, r.f1);
      Assert.AreEqual(0u, r.f2);
      Assert.AreEqual(0u, r.f3);
    }

    [TestMethod]
    public void MulTest()
    {
      Fixed128 a = 3;
      Fixed128 b = 5;
      Fixed128 r = a * b;
      Assert.AreEqual(15u, r.i);
      Assert.AreEqual(0u, r.f1);
      Assert.AreEqual(0u, r.f2);
      Assert.AreEqual(0u, r.f3);
      a = -3;
      b = -5;
      r = a * b;
      Assert.AreEqual(15u, r.i);
      Assert.AreEqual(0u, r.f1);
      Assert.AreEqual(0u, r.f2);
      Assert.AreEqual(0u, r.f3);
      a = 3;
      b = -5;
      r = a * b;
      Assert.AreEqual(0xFFFFFFF1u, r.i);
      Assert.AreEqual(0u, r.f1);
      Assert.AreEqual(0u, r.f2);
      Assert.AreEqual(0u, r.f3);
    }

    [TestMethod]
    public void DivTest()
    {
      Fixed128 a = 3;
      Fixed128 b = 2;
      Fixed128 r = a / b;
      Assert.AreEqual(1u, r.i);
      Assert.AreEqual(0x80000000u, r.f1);
      Assert.AreEqual(0u, r.f2);
      Assert.AreEqual(0u, r.f3);
    }

    [TestMethod]
    public void NegTest()
    {
      Fixed128 a = 100;
      Fixed128 r = -a;
      Assert.AreEqual(0xFFFFFF9Cu, r.i);
      Assert.AreEqual(0u, r.f1);
      Assert.AreEqual(0u, r.f2);
      Assert.AreEqual(0u, r.f3);
      r = -r;
      Assert.AreEqual(100u, r.i);
      Assert.AreEqual(0u, r.f1);
      Assert.AreEqual(0u, r.f2);
      Assert.AreEqual(0u, r.f3);
    }

    [TestMethod]
    public void SqrtTest()
    {
      Fixed128 a = 100;
      Fixed128 r = a.sqrt();
      Assert.AreEqual(10u, r.i);
      Assert.AreEqual(0u, r.f1);
      Assert.AreEqual(0u, r.f2);
      Assert.AreEqual(0u, r.f3);
      a = 2;
      r = a.sqrt();
      Assert.AreEqual(1u, r.i);
      Assert.AreEqual(0x6a09e667u, r.f1);
      Assert.AreEqual(0xf3bcc908u, r.f2);
      Assert.AreEqual(0xb2fb1367u, r.f3);
    }

    [TestMethod]
    public void FromDecimalTest()
    {
      Fixed128 a = "1";
      Assert.AreEqual(1u, a.i);
      Assert.AreEqual(0u, a.f1);
      Assert.AreEqual(0u, a.f2);
      Assert.AreEqual(0u, a.f3);
      a = "400.125";
      Assert.AreEqual(400u, a.i);
      Assert.AreEqual(0x20000000u, a.f1);
      Assert.AreEqual(0u, a.f2);
      Assert.AreEqual(0u, a.f3);
    }

    [TestMethod]
    public void ToDecimalTest()
    {
      Fixed128 a = 3;
      string s = (string)a;
      Assert.AreEqual("3", s);
      a /= (Fixed128)2;
      s = (string)a;
      Assert.AreEqual("1.5", s);
    }

    [TestMethod]
    public void FromDoubleTest()
    {
      Fixed128 a = 1.5;
      Assert.AreEqual(1u, a.i);
      Assert.AreEqual(0x80000000u, a.f1);
      Assert.AreEqual(0u, a.f2);
      Assert.AreEqual(0u, a.f3);
      a = -100.125;
      Assert.AreEqual(0xFFFFFF9Bu, a.i);
      Assert.AreEqual(0xE0000000u, a.f1);
      Assert.AreEqual(0u, a.f2);
      Assert.AreEqual(0u, a.f3);
    }

    [TestMethod]
    public void ToDoubleTest()
    {
      Fixed128 a = new Fixed128(1, 0x80000000u, 0u, 0u);
      double n = a;
      Assert.AreEqual(1.5, n);
      a = new Fixed128(-101, 0xE0000000u, 0u, 0u);
      n = a;
      Assert.AreEqual(-100.125, n);
    }
  }
}
