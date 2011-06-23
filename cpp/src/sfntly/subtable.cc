/*
 * Copyright 2011 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sfntly/subtable.h"

namespace sfntly {
/******************************************************************************
 * SubTable class
 ******************************************************************************/
SubTable::SubTable(ReadableFontData* data) : FontDataTable(data) {
}

SubTable::~SubTable() {}

/******************************************************************************
 * SubTable::Builder class
 ******************************************************************************/
SubTable::Builder::Builder(FontDataTableBuilderContainer* container,
                           WritableFontData* data) :
    FontDataTable::Builder(container, data) {
}

SubTable::Builder::Builder(FontDataTableBuilderContainer* container,
                           ReadableFontData* data) :
    FontDataTable::Builder(container, data) {
}

SubTable::Builder::~Builder() {}

}  // namespace sfntly
