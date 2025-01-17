// Copyright (C) 2018-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include <gtest/gtest.h>

#include <memory>
#include <openvino/core/model.hpp>
#include <openvino/opsets/opset1.hpp>
#include <openvino/opsets/opset3.hpp>
#include <openvino/pass/manager.hpp>
#include <queue>
#include <string>
#include <transformations/init_node_info.hpp>
#include <transformations/op_conversions/convert_broadcast_to_tiles.hpp>
#include <transformations/utils/utils.hpp>

#include "common_test_utils/ngraph_test_utils.hpp"
using namespace ov;
using namespace testing;

TEST(TransformationTests, ConvertBroadcastToTilesDynamic) {
    {
        auto input1 = std::make_shared<opset1::Parameter>(element::f32, Shape{3, 1, 2});
        auto target_shape = opset1::Constant::create(element::i64, Shape{3}, std::vector<int64_t>{3, 5, 2});
        auto broadcast = std::make_shared<opset1::Broadcast>(input1, target_shape);
        broadcast->set_friendly_name("broadcast");

        auto f = std::make_shared<ov::Model>(NodeVector{broadcast}, ParameterVector{input1});

        pass::Manager manager;
        manager.register_pass<ov::pass::InitNodeInfo>();
        manager.register_pass<ov::pass::ConvertBroadcastToTiles>();
        ASSERT_NO_THROW(manager.run_passes(f));
        ASSERT_NO_THROW(check_rt_info(f));
    }
    // TODO: construct reference graph and use TEST_F
}
