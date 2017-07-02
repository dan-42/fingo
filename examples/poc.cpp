/*
 * poc.cpp
 *
 *  Created on: May 27, 2017
 *      Author: dan
 */


#include <mapbox/variant.hpp>
#include <mapbox/variant_io.hpp>
#include <spoon.hpp>

#include <boost/fusion/adapted.hpp>

#include <cstdint>
#include <vector>
#include <iterator>
#include <iostream>
#include <iomanip>
#include <type_traits>




using binary_data                 = std::vector<uint8_t>;
using binary_data_iterator        = binary_data::iterator;
using binary_data_const_iterator  = binary_data::const_iterator;




struct foo {
  uint32_t a;
  uint32_t b;
  uint32_t c;
};

BOOST_FUSION_ADAPT_STRUCT(foo, a, b, c);

struct my_seq {
  uint32_t d;
  uint32_t e;
  foo f;
};

BOOST_FUSION_ADAPT_STRUCT(my_seq, d, e, f);

auto do_any() -> void {
  std::cout << "\n------------------ANY-------------------------" << std::endl;
  using my_any_t = mapbox::util::variant<uint32_t, double>;
  constexpr auto any_engine = spoon::any<my_any_t>(spoon::float64, spoon::uint32);
  binary_data data;

  {
     my_any_t var;
     //var = double{-3.145};
     var = uint32_t{3145};
     auto success = spoon::serialize(data, var, any_engine );

     std::cout << " 0x" ;
     for ( const auto& c : data ) {
       std::cout << std::hex <<  std::setw(2) << std::setfill('0') << (int)c << "";
     }
     std::cout << std::endl;
  }

  {
    auto start = data.begin();
   auto end   = data.end();
   my_any_t var;
   auto success = spoon::deserialize(start, end, var, any_engine);
   std::cout << std::dec << "deserialize " << var << std::endl;
  }
}


auto do_seq() -> void {
  std::cout << "\n------------------SEQ-------------------------" << std::endl;
  binary_data data;
  constexpr auto foo_sequence = spoon::seq<foo>(spoon::uint32, spoon::uint32, spoon::uint32);
  constexpr auto my_sequence = spoon::seq<my_seq>(spoon::uint32, spoon::uint32, foo_sequence);
  {
    //uint32_t var = 1337;
    foo f{1337, 42, 23};
    my_seq var{1984, 314, f};


    auto success = spoon::serialize(data, var, my_sequence );
   // auto success = spoon::serialize(data, var, spoon::uint32 );

    std::cout << " 0x" ;
    for ( const auto& c : data ) {
      std::cout << std::hex <<  std::setw(2) << std::setfill('0') << (int)c << "";
    }
    std::cout << std::endl;
  }

  {
    auto start = data.begin();
    auto end   = data.end();
    my_seq var;
    auto success = spoon::deserialize(start, end, var, my_sequence);
    std::cout << std::dec << "deserialize " << var.d << " " << var.e << " " << var.f.a << " " << var.f.b << " " << var.f.c << std::endl;
  }

}



auto combined() {





}

int main(int argc, char **argv) {

 do_seq();
 do_any();



  return 0;
}


