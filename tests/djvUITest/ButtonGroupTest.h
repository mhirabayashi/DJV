// SPDX-License-Identifier: BSD-3-Clause
// Copyright (c) 2020 Darby Johnston
// All rights reserved.

#include <djvTestLib/TickTest.h>

namespace djv
{
    namespace UITest
    {
        class ButtonGroupTest : public Test::ITickTest
        {
        public:
            ButtonGroupTest(const std::shared_ptr<Core::Context>&);
            
            void run() override;
        };
        
    } // namespace UITest
} // namespace djv

