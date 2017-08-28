/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <gtest/gtest.h>

#define IGNITION_UNITTEST_EXPECTDATA_ACCESS

#include "ignition/physics/SpecifyData.hh"
#include "ignition/physics/CanReadData.hh"
#include "ignition/physics/CanWriteData.hh"

#include "utils/TestDataTypes.hh"

template <typename ReadSpec>
class SomeClass
    : public ignition::physics::CanReadRequiredData<SomeClass<ReadSpec>, ReadSpec>,
      public ignition::physics::CanReadExpectedData<SomeClass<ReadSpec>, ReadSpec>,
      public ignition::physics::CanWriteExpectedData<SomeClass<ReadSpec>, RequireIntDouble>
{
  public: StringData sdata;
  public: std::size_t scount;

  public: BoolData bdata;
  public: std::size_t bcount;

  public: CharData cdata;
  public: std::size_t ccount;

  public: IntData idata;
  public: std::size_t icount;

  public: FloatData fdata;
  public: std::size_t fcount;

  SomeClass()
    : scount(0),
      bcount(0),
      ccount(0),
      icount(0),
      fcount(0)
  {
    // Do nothing
  }

  public: void Read(const StringData& _sdata)
  {
    sdata = _sdata;
    ++scount;
  }

  public: void Read(const BoolData& _bdata)
  {
    bdata = _bdata;
    ++bcount;
  }

  public: void Read(const CharData& _cdata)
  {
    cdata = _cdata;
    ++ccount;
  }

  public: void Read(const IntData& _idata)
  {
    idata = _idata;
    ++icount;
  }

  public: void Read(const FloatData& _fdata)
  {
    fdata = _fdata;
    ++fcount;
  }

  public: void Write(IntData& _idata) const
  {
    _idata.myInt = 67;
  }

  public: void Write(DoubleData& _ddata) const
  {
    _ddata.myDouble = 7.2;
  }

  public: void Write(StringData& _sdata) const
  {
    _sdata.myString = "seventy-seven";
  }

  public: void Write(CharData& _cdata) const
  {
    _cdata.myChar = '8';
  }
};

TEST(CanReadWrite, ReadWriteData)
{
  ignition::physics::CompositeData input;
  input.Get<StringData>().myString = "89";
  input.Get<BoolData>().myBool = false;
  input.Get<CharData>().myChar = 'd';
  input.Get<IntData>().myInt = 92;
  input.Get<FloatData>().myFloat = 93.5;
  input.ResetQueries();

  SomeClass<RequireStringBoolChar> something;
  something.ReadRequiredData(input);
  EXPECT_EQ("89", something.sdata.myString);
  EXPECT_FALSE(something.bdata.myBool);
  EXPECT_EQ('d', something.cdata.myChar);
  EXPECT_EQ(55, something.idata.myInt);
  EXPECT_NEAR(9.5, something.fdata.myFloat, 1e-8);

  something.ReadExpectedData(input);
  EXPECT_EQ(92, something.idata.myInt);
  EXPECT_NEAR(93.5, something.fdata.myFloat, 1e-8);

  EXPECT_EQ(1u, something.scount);
  EXPECT_EQ(1u, something.bcount);
  EXPECT_EQ(1u, something.ccount);
  EXPECT_EQ(1u, something.icount);
  EXPECT_EQ(1u, something.fcount);

  ignition::physics::CompositeData output;
  something.WriteExpectedData(output);
  EXPECT_EQ(67, output.Get<IntData>().myInt);
  EXPECT_NEAR(7.2, output.Get<DoubleData>().myDouble, 1e-8);
  EXPECT_EQ("seventy-seven", output.Get<StringData>().myString);
  EXPECT_EQ('8', output.Get<CharData>().myChar);
}

TEST(CanReadWrite, OnlyReadOnce)
{
  ignition::physics::CompositeData input;
  input.Get<StringData>().myString = "89";
  input.Get<BoolData>().myBool = false;
  input.Get<CharData>().myChar = 'd';
  input.Get<IntData>().myInt = 92;
  input.Get<FloatData>().myFloat = 93.5;
  input.ResetQueries();

  SomeClass<RedundantSpec> redundant;
  redundant.ReadRequiredData(input, ignition::physics::ReadOptions(false));
  redundant.ReadRequiredData(input);
  EXPECT_EQ("89", redundant.sdata.myString);
  EXPECT_FALSE(redundant.bdata.myBool);
  EXPECT_EQ('d', redundant.cdata.myChar);
  EXPECT_EQ(55, redundant.idata.myInt);
  EXPECT_NEAR(9.5, redundant.fdata.myFloat, 1e-8);

  redundant.ReadExpectedData(input);
  EXPECT_EQ(92, redundant.idata.myInt);
  EXPECT_NEAR(93.5, redundant.fdata.myFloat, 1e-8);

  EXPECT_EQ(1u, redundant.scount);
  EXPECT_EQ(1u, redundant.bcount);
  EXPECT_EQ(1u, redundant.ccount);
  EXPECT_EQ(1u, redundant.icount);
  EXPECT_EQ(1u, redundant.fcount);
}

/////////////////////////////////////////////////
int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
