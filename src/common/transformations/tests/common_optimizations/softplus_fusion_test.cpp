// Copyright (C) 2018-2023 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

#include <gtest/gtest.h>

#include <memory>
#include <openvino/core/model.hpp>
#include <openvino/opsets/opset4.hpp>
#include <openvino/pass/manager.hpp>
#include <string>
#include <transformations/common_optimizations/softplus_fusion.hpp>
#include <transformations/init_node_info.hpp>
#include <transformations/utils/utils.hpp>

#include "common_test_utils/ngraph_test_utils.hpp"

using namespace ov;
using namespace testing;

TEST_F(TransformationTestsF, SoftPlusFusing) {
    {
        auto input0 = std::make_shared<opset4::Parameter>(element::f32, Shape{3, 1, 2});
        auto exp = std::make_shared<opset4::Exp>(input0);
        auto input_const = opset4::Constant::create(element::f32, Shape{1}, {1.0});
        auto add = std::make_shared<opset4::Add>(exp, input_const);
        auto log = std::make_shared<opset4::Log>(add);

        model = std::make_shared<ov::Model>(NodeVector{log}, ParameterVector{input0});

        manager.register_pass<ov::pass::SoftPlusFusion>();
    }

    {
        auto data = std::make_shared<opset4::Parameter>(element::f32, Shape{3, 1, 2});
        auto softplus = std::make_shared<opset4::SoftPlus>(data);

        model_ref = std::make_shared<ov::Model>(NodeVector{softplus}, ParameterVector{data});
    }
}

TEST_F(TransformationTestsF, SoftPlusFusingDynamic) {
    {
        auto input0 = std::make_shared<opset4::Parameter>(element::f32, PartialShape::dynamic(1));
        auto exp = std::make_shared<opset4::Exp>(input0);
        auto input_const = opset4::Constant::create(element::f32, Shape{1}, {1.0});
        auto add = std::make_shared<opset4::Add>(exp, input_const);
        auto log = std::make_shared<opset4::Log>(add);

        model = std::make_shared<ov::Model>(NodeVector{log}, ParameterVector{input0});

        manager.register_pass<ov::pass::SoftPlusFusion>();
    }

    {
        auto data = std::make_shared<opset4::Parameter>(element::f32, PartialShape::dynamic(1));
        auto softplus = std::make_shared<opset4::SoftPlus>(data);

        model_ref = std::make_shared<ov::Model>(NodeVector{softplus}, ParameterVector{data});
    }
}

TEST_F(TransformationTestsF, SoftPlusFusingNegative) {
    {
        auto input0 = std::make_shared<opset4::Parameter>(element::f32, PartialShape::dynamic(1));
        auto exp = std::make_shared<opset4::Exp>(input0);
        auto input_const = opset4::Constant::create(element::f32, Shape{1}, {-1.0});
        auto add = std::make_shared<opset4::Add>(exp, input_const);
        auto log = std::make_shared<opset4::Log>(add);

        model = std::make_shared<ov::Model>(NodeVector{log}, ParameterVector{input0});

        manager.register_pass<ov::pass::SoftPlusFusion>();
    }

    {
        auto input0 = std::make_shared<opset4::Parameter>(element::f32, PartialShape::dynamic(1));
        auto exp = std::make_shared<opset4::Exp>(input0);
        auto input_const = opset4::Constant::create(element::f32, Shape{1}, {-1.0});
        auto add = std::make_shared<opset4::Add>(exp, input_const);
        auto log = std::make_shared<opset4::Log>(add);

        model_ref = std::make_shared<ov::Model>(NodeVector{log}, ParameterVector{input0});
    }
}
