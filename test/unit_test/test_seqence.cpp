/**
 * ninja-wooki, is a BACnet stack C++ library
 *
 * Copyright (C) 2015 Daniel Friedrich
 *
 * This file is part of ninja-wooki.
 *
 * ninja-wooki is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * ninja-wooki is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details. You should have received a copy of the GNU
 * General Public License along with Mupen64PlusAE. If not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Daniel Friedrich
 */


#define BOOST_TEST_MODULE test spoon seqence
#include <boost/test/included/unit_test.hpp>

#include <boost/fusion/adapted/struct.hpp>
#include <iostream>
#include <vector>

#include <spoon.hpp>
#include <spoon/seq.hpp>
#include <spoon/binary.hpp>


struct pod_type {

  bool      my_bool;
  uint32_t  my_uint32;
  double    my_double;
};

BOOST_FUSION_ADAPT_STRUCT(pod_type, my_bool, my_uint32, my_double);


BOOST_AUTO_TEST_SUITE( test_spoon_seq )

BOOST_AUTO_TEST_CASE( test_spoon_seq_simple ) {

  constexpr auto engine = spoon::seq<pod_type>(spoon::big_endian::bool8, spoon::big_endian::uint32, spoon::big_endian::float64);
  std::vector<uint8_t> binary_data{};
  {
    pod_type var{true, 1337, 3.14};
    auto success = spoon::serialize(binary_data, engine, var);
    BOOST_TEST(success == true);

    BOOST_TEST( binary_data.size() == size_t{13} );
    BOOST_TEST(binary_data[0] == 0x01);

    BOOST_TEST(binary_data[1] == 0x00);
    BOOST_TEST(binary_data[2] == 0x00);
    BOOST_TEST(binary_data[3] == 0x05);
    BOOST_TEST(binary_data[4] == 0x39);

    BOOST_TEST(binary_data[5] == 0x40);
    BOOST_TEST(binary_data[6] == 0x09);
    BOOST_TEST(binary_data[7] == 0x1E);
    BOOST_TEST(binary_data[8] == 0xB8);
    BOOST_TEST(binary_data[9] == 0x51);
    BOOST_TEST(binary_data[10] == 0xEB);
    BOOST_TEST(binary_data[11] == 0x85);
    BOOST_TEST(binary_data[12] == 0x1F);
  }
  {
    pod_type pod;
    auto     start = binary_data.begin();
    const auto end = binary_data.end();
    auto  success = spoon::deserialize(start, end, engine, pod);
    BOOST_TEST(success == true);
    BOOST_TEST((start == end), "start != end");
    BOOST_TEST(pod.my_bool    == true);
    BOOST_TEST(pod.my_uint32  == uint32_t{1337});
    BOOST_TEST(pod.my_double  == 3.14);
  }
}





BOOST_AUTO_TEST_SUITE_END()
