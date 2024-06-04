#include <gtest/gtest.h>

#include "../bank.hpp"

namespace atm {

TEST(TestBank, Cons) {
  Bank bank{"abc", 123};
  ASSERT_EQ(bank.GetName(), "abc");
  ASSERT_EQ(bank.GetId(), 123);
}

}  // end of namespace atm
