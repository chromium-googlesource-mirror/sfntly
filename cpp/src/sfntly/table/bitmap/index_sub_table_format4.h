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

#ifndef SFNTLY_CPP_SRC_SFNTLY_TABLE_BITMAP_INDEX_SUBTABLE_FORMAT4_H_
#define SFNTLY_CPP_SRC_SFNTLY_TABLE_BITMAP_INDEX_SUBTABLE_FORMAT4_H_

#include "sfntly/table/bitmap/index_sub_table.h"

namespace sfntly {

class IndexSubTableFormat4 : public IndexSubTable,
                             public RefCounted<IndexSubTableFormat4> {
 public:
  static int32_t GetDataLength(ReadableFontData* data,
                               int32_t offset,
                               int32_t first,
                               int32_t last);

  // Note: the constructor does not implement offset/length form provided in
  //       Java to avoid heavy lifting in constructors.  Callers to call
  //       GetDataLength() static method of the derived class to get proper
  //       length and slice ahead.
  IndexSubTableFormat4(ReadableFontData* data, int32_t first, int32_t last);
  virtual ~IndexSubTableFormat4();

  virtual int32_t NumGlyphs();
  virtual int32_t GlyphOffset(int32_t glyph_id);
  virtual int32_t GlyphLength(int32_t glyph_id);

 private:
  int32_t FindCodeOffsetPair(int32_t glyph_id);
};

}  // namespace sfntly

#endif  // SFNTLY_CPP_SRC_SFNTLY_TABLE_BITMAP_INDEX_SUBTABLE_FORMAT4_H_
