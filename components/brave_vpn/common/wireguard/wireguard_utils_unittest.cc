/* Copyright (c) 2024 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at https://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_vpn/common/wireguard/wireguard_utils.h"

#include "base/json/json_reader.h"
#include "base/logging.h"
#include "base/test/scoped_feature_list.h"
#include "testing/gmock/include/gmock/gmock.h"
#include "testing/gtest/include/gtest/gtest.h"

TEST(BraveVPNWireGuardUtilsUnitTest, ValidateKey) {
  std::string output;
  EXPECT_FALSE(brave_vpn::wireguard::ValidateKey(L"", &output, "public_key"));

  EXPECT_TRUE(
      brave_vpn::wireguard::ValidateKey(L"abcdefghi", &output, "public_key"));
}

TEST(BraveVPNWireGuardUtilsUnitTest, ValidateAddress) {
  std::string output;
  // Invalid.
  EXPECT_FALSE(brave_vpn::wireguard::ValidateAddress(L"", &output));
  EXPECT_FALSE(brave_vpn::wireguard::ValidateAddress(L"a.b.c.d", &output));
  // IPv6 not allowed.
  EXPECT_FALSE(brave_vpn::wireguard::ValidateAddress(
      L"fe80::1ff:fe23:4567:890a", &output));

  // Valid.
  EXPECT_TRUE(brave_vpn::wireguard::ValidateAddress(L"192.168.1.1", &output));
  // Spaces are stripped out.
  EXPECT_TRUE(
      brave_vpn::wireguard::ValidateAddress(L"  192.168.1.1   ", &output));
  // Verify parsing worked.
  EXPECT_EQ(output, "192.168.1.1");
}

TEST(BraveVPNWireGuardUtilsUnitTest, ValidateEndpoint) {
  std::string output;
  EXPECT_FALSE(brave_vpn::wireguard::ValidateEndpoint(L"", &output));
  EXPECT_FALSE(brave_vpn::wireguard::ValidateEndpoint(L"192.168.1.1", &output));
  EXPECT_FALSE(brave_vpn::wireguard::ValidateEndpoint(
      L"toronto-ipsec-8.not-guardianapp.com", &output));

  EXPECT_TRUE(brave_vpn::wireguard::ValidateEndpoint(
      L"toronto-ipsec-8.guardianapp.com", &output));
  EXPECT_TRUE(brave_vpn::wireguard::ValidateEndpoint(
      L"france-ipsec-1.sudosecuritygroup.com", &output));
}
