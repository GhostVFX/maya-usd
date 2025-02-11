//
// Copyright 2020 Autodesk
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#pragma once

#include <mayaUsd/base/api.h>
#include <mayaUsd/undo/UsdUndoBlock.h>
#include <mayaUsd/undo/UsdUndoableItem.h>

#include <ufe/path.h>

namespace MAYAUSD_NS_DEF {
namespace ufe {

// Templated helper class to factor out common code for undoable commands.
//
// Implement the execute, undo and redo of the UFE command interfaces,
// declaring the UsdUndoBlock during the execution.
//
// Sub-classes only need to implement the executeUndoBlock() function.
// This function does the real work of modifying values, and these changes
// will be captured in the UsdUndoableItem via the UsdUndoBlock declared
// in execute().
template <typename Cmd> class UsdUndoableCommand : public Cmd
{
public:
    UsdUndoableCommand(const Ufe::Path& path)
        : Cmd(path)
    {
    }

    // Ufe::UndoableCommand overrides.

    // Declares a UsdUndoBlock and calls executeUndoBlock()
    void execute() override
    {
        UsdUndoBlock undoBlock(&_undoableItem);
        executeUndoBlock();
    }

    // Calls undo on the undoable item.
    void undo() override { _undoableItem.undo(); }

    // Calls redo on the undoable item.
    void redo() override { _undoableItem.redo(); }

protected:
    // Actual implementation of the execution of the command,
    // executed "within" a UsdUndoBlock to capture undo data,
    // to be implemented by the sub-class.
    virtual void executeUndoBlock() = 0;

private:
    UsdUndoableItem _undoableItem;
};

} // namespace ufe
} // namespace MAYAUSD_NS_DEF
