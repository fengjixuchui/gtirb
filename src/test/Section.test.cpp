//===- Section.test.cpp -----------------------------------------*- C++ -*-===//
//
//  Copyright (C) 2018 GrammaTech, Inc.
//
//  This code is licensed under the MIT license. See the LICENSE file in the
//  project root for license terms.
//
//  This project is sponsored by the Office of Naval Research, One Liberty
//  Center, 875 N. Randolph Street, Arlington, VA 22203 under contract #
//  N68335-17-C-0700.  The content of the information does not necessarily
//  reflect the position or policy of the Government and no official
//  endorsement should be inferred.
//
//===----------------------------------------------------------------------===//
#include <gtirb/Context.hpp>
#include <gtirb/Section.hpp>
#include <proto/Section.pb.h>
#include <gtest/gtest.h>
#include <limits>

using namespace gtirb;

static Context Ctx;

TEST(Unit_Section, containsAddr) {
  Section& Good = *Section::Create(Ctx, "good", Addr{11}, 100);
  EXPECT_FALSE(containsAddr(Good, Addr(10)));
  EXPECT_TRUE(containsAddr(Good, Addr(11)));
  EXPECT_TRUE(containsAddr(Good, Addr(110)));
  EXPECT_FALSE(containsAddr(Good, Addr(111)));

  Section& Big = *Section::Create(Ctx, "big", Addr(0),
                                  std::numeric_limits<uint64_t>::max());
  EXPECT_TRUE(containsAddr(Big, Addr(0)));
  EXPECT_TRUE(
      containsAddr(Big, Addr(std::numeric_limits<uint64_t>::max() - 1)));
}

TEST(Unit_Section, protobufRoundTrip) {
  proto::Section Message;

  {
    Context InnerCtx;
    Section* Original = Section::Create(InnerCtx, "name", Addr(1), 1234);
    Original->toProtobuf(&Message);
  }
  Section* Result = Section::fromProtobuf(Ctx, Message);

  EXPECT_EQ(Result->getName(), "name");
  EXPECT_EQ(Result->getSize(), 1234);
  EXPECT_EQ(Result->getAddress(), Addr(1));
}
