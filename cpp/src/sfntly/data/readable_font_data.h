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

#ifndef TYPOGRAPHY_FONT_SFNTLY_SRC_SFNTLY_DATA_READABLE_FONT_DATA_H_
#define TYPOGRAPHY_FONT_SFNTLY_SRC_SFNTLY_DATA_READABLE_FONT_DATA_H_

#include "sfntly/port/refcount.h"
#include "sfntly/data/font_data.h"

namespace sfntly {

class WritableFontData;
class OutputStream;
class ReadableFontData : public FontData,
                         public RefCounted<ReadableFontData> {
 public:
  explicit ReadableFontData(ByteArray* array);
  virtual ~ReadableFontData();

 protected:
  // Constructor. Creates a bounded wrapper of another ReadableFontData from the
  // given offset until the end of the original ReadableFontData.
  // @param data data to wrap
  // @param offset the start of this data's view of the original data
  ReadableFontData(ReadableFontData* data, int32_t offset);

  // Constructor. Creates a bounded wrapper of another ReadableFontData from the
  // given offset until the end of the original ReadableFontData.
  // @param data data to wrap
  // @param offset the start of this data's view of the original data
  ReadableFontData(ReadableFontData* data, int32_t offset, int32_t length);

 private:
  void computeChecksum();
  int64_t computeCheckSum(int32_t low_bound, int32_t high_bound);

 public:
  // Get a computed checksum for the data. This checksum uses the OpenType spec
  // calculation. Every ULong value (32 bit unsigned) in the data is summed and
  // the resulting value is truncated to 32 bits. If the data length in bytes is
  // not an integral multiple of 4 then any remaining bytes are treated as the
  // start of a 4 byte sequence whose remaining bytes are zero.
  // @return the checksum
  int64_t checksum();

  // Sets the bounds to use for computing the checksum. These bounds are in
  // begin and end pairs. If an odd number is given then the final range is
  // assumed to extend to the end of the data. The lengths of each range must be
  // a multiple of 4.
  // @param ranges the range bounds to use for the checksum
  void setCheckSumRanges(const IntegerList& ranges);

 public:
  virtual int32_t readUByte(int32_t index);
  virtual int32_t readByte(int32_t index);
  virtual int32_t readBytes(int32_t index, ByteVector* b, int32_t offset,
                            int32_t length);
  virtual int32_t readChar(int32_t index);
  virtual int32_t readUShort(int32_t index);
  virtual int32_t readShort(int32_t index);
  virtual int32_t readUInt24(int32_t index);
  virtual int64_t readULong(int32_t index);
  virtual int32_t readULongAsInt(int32_t index);
  virtual int32_t readLong(int32_t index);
  virtual int32_t readFixed(int32_t index);
  virtual int64_t readDateTimeAsLong(int32_t index);

  virtual int32_t readFWord(int32_t index);
  virtual int32_t readFUFWord(int32_t index);

  virtual int32_t copyTo(OutputStream* os);
  virtual int32_t copyTo(WritableFontData* wfd);
  virtual int32_t copyTo(ByteArray* ba);

  // TODO(arthurhsu): IMPLEMENT
  /*
  virtual int32_t readFUnit(int32_t index);
  virtual int64_t readF2Dot14(int32_t index);
  virtual int64_t readLongDateTime(int32_t index);
  virtual int32_t searchUShort(int32_t start, int32_t length, int32_t key);
  virtual int32_t searchUShort(int32_t start_index, int32_t start_offset,
                               int32_t count_index, int32_t count_offset,
                               int32_t length, int32_t key);
  virtual int32_t searchULong(int32_t start_index, int32_t start_offset,
                              int32_t end_index, int32_t end_offset,
                              int32_t length, int32_t key);
  */

  // Makes a slice of this FontData. The returned slice will share the data with
  // the original FontData.
  // @param offset the start of the slice
  // @param length the number of bytes in the slice
  // @return a slice of the original FontData
  virtual CALLER_ATTACH FontData* slice(int32_t offset, int32_t length);

  // Makes a bottom bound only slice of this array. The returned slice will
  // share the data with the original FontData.
  // @param offset the start of the slice
  // @return a slice of the original FontData
  virtual CALLER_ATTACH FontData* slice(int32_t offset);

 private:
  bool checksum_set_;  // TODO(arthurhsu): IMPLEMENT: must be set atomically
  int64_t checksum_;
  IntegerList checksum_range_;
};
typedef Ptr<ReadableFontData> ReadableFontDataPtr;

}  // namespace sfntly

#endif  // TYPOGRAPHY_FONT_SFNTLY_SRC_SFNTLY_DATA_READABLE_FONT_DATA_H_
