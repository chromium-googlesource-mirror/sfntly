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

#include <stdio.h>

#include "gtest/gtest.h"
#include "sfntly/data/memory_byte_array.h"
#include "sfntly/port/file_input_stream.h"
#include "test/test_font_utils.h"

namespace sfntly {

void builderForFontFile(const char* font_path, FontFactory* factory,
                        FontBuilderArray* builders) {
  assert(factory);
  FileInputStream is;
  is.open(font_path);
  factory->loadFontsForBuilding(&is, builders);
  EXPECT_GT(builders->size(), static_cast<size_t>(0));
}

void serializeFont(const char* font_path, FontFactory* factory, Font* font) {
  assert(font_path);
  assert(factory);
  assert(font);
  MemoryOutputStream output_stream;
  factory->serializeFont(font, &output_stream);
  serializeToFile(&output_stream, font_path);
}

void loadFont(const char* font_path, FontFactory* factory, FontArray* fonts) {
  FileInputStream is;
  is.open(font_path);
  factory->loadFonts(&is, fonts);
  is.close();
}

void loadFile(const char* input_file_path, ByteVector* input_buffer) {
  assert(input_file_path);
  assert(input_buffer);

  FILE* input_file = NULL;
#if defined WIN32
  fopen_s(&input_file, input_file_path, "rb");
#else
  input_file = fopen(input_file_path, "rb");
#endif
  EXPECT_NE(input_file, reinterpret_cast<FILE*>(NULL));
  fseek(input_file, 0, SEEK_END);
  size_t file_size = ftell(input_file);
  fseek(input_file, 0, SEEK_SET);
  input_buffer->resize(file_size);
  fread(&((*input_buffer)[0]), 1, file_size, input_file);
  fclose(input_file);
}

void serializeToFile(MemoryOutputStream* output_stream, const char* file_path) {
  assert(file_path);
  assert(output_stream);

  FILE* output_file = NULL;
#if defined WIN32
  fopen_s(&output_file, file_path, "wb");
#else
  output_file = fopen(file_path, "wb");
#endif
  EXPECT_NE(output_file, reinterpret_cast<FILE*>(NULL));
  fwrite(output_stream->get(), 1, output_stream->size(), output_file);
  fflush(output_file);
  fclose(output_file);
}

void hexDump(const unsigned char* byte_data, size_t length) {
  if (byte_data == NULL || length == 0) {
    fprintf(stderr, "<NULL>\n");
    return;
  }

  fprintf(stderr, "data length = %ld (%lx)\n", length, length);
  for (size_t i = 0; i < length; ++i) {
    fprintf(stderr, "%02x ", byte_data[i]);
    if ((i & 0xf) == 0xf) {
      fprintf(stderr, "\n");
    }
  }
  fprintf(stderr, "\n");
}

}  // namespace sfntly
