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

// Must include this before ICU to avoid stdint redefinition issue.
#include "sfntly/port/type.h"

#include <unicode/ustring.h>
#include <unicode/unistr.h>

#include "gtest/gtest.h"
#include "sfntly/font.h"
#include "sfntly/font_factory.h"
#include "sfntly/name_table.h"
#include "sfntly/tag.h"
#include "sfntly/data/memory_byte_array.h"
#include "sfntly/port/memory_output_stream.h"
#include "test/test_data.h"
#include "test/test_font_utils.h"
#include "test/name_editing_test.h"

namespace sfntly {

static ByteVector input_buffer;
static ByteArrayPtr ba;

void loadTestFile(FontFactory* factory, FontBuilderArray* font_builders) {
  assert(factory);
  assert(font_builders);
  if (ba == NULL) {
    loadFile(SAMPLE_TTF_FILE, &input_buffer);
    ba = new MemoryByteArray(&(input_buffer[0]), input_buffer.size());
  }

  factory->loadFontsForBuilding(ba, font_builders);
}

bool testChangeOneName() {
  FontFactoryPtr factory = FontFactory::getInstance();
  FontBuilderArray font_builder_array;
  loadTestFile(factory, &font_builder_array);
  FontBuilderPtr font_builder = font_builder_array[0];

  NameTableBuilderPtr name_builder = down_cast<NameTable::Builder*>(
      font_builder->getTableBuilder(Tag::name));

  // Change the font name.
  NameEntryBuilderPtr neb =
      name_builder->nameBuilder(PlatformId::kWindows,
                                WindowsEncodingId::kUnicodeUCS2,
                                WindowsLanguageId::kEnglish_UnitedStates,
                                NameId::kFontFamilyName);
  U_STRING_DECL(new_name, "Timothy", 7);
  neb->setName(new_name);

  // Build the font.
  FontPtr font = font_builder->build();

  // Serialize and reload the serialized font.
  MemoryOutputStream os;
  factory->serializeFont(font, &os);
  FontArray font_array;
  ByteArrayPtr new_ba = new MemoryByteArray(os.get(), os.size());
  factory->loadFonts(new_ba, &font_array);
  FontPtr new_font = font_array[0];

  // Check the font name.
  NameTablePtr name_table = down_cast<NameTable*>(font->table(Tag::name));
  UChar* name = name_table->name(PlatformId::kWindows,
                                 WindowsEncodingId::kUnicodeUCS2,
                                 WindowsLanguageId::kEnglish_UnitedStates,
                                 NameId::kFontFamilyName);
  EXPECT_TRUE(name != NULL);
  EXPECT_EQ(u_strcmp(name, new_name), 0);
  delete[] name;
  return true;
}

bool testModifyNameTableAndRevert() {
  FontFactoryPtr factory = FontFactory::getInstance();
  FontBuilderArray font_builder_array;
  loadTestFile(factory, &font_builder_array);
  FontBuilderPtr font_builder = font_builder_array[0];

  NameTableBuilderPtr name_builder = down_cast<NameTable::Builder*>(
      font_builder->getTableBuilder(Tag::name));

  // Change the font name.
  NameEntryBuilderPtr neb =
      name_builder->nameBuilder(PlatformId::kWindows,
                                WindowsEncodingId::kUnicodeUCS2,
                                WindowsLanguageId::kEnglish_UnitedStates,
                                NameId::kFontFamilyName);
  NameTable::NameEntry* neb_entry = neb->entry();
  UChar* original_name = neb_entry->name();
  EXPECT_TRUE(original_name != NULL);

  U_STRING_DECL(new_name, "Timothy", 7);
  neb->setName(new_name);
  name_builder->revertNames();

  // Build the font.
  FontPtr font = font_builder->build();

  // Serialize and reload the serialized font.
  MemoryOutputStream os;
  factory->serializeFont(font, &os);
  FontArray font_array;
  ByteArrayPtr new_ba = new MemoryByteArray(os.get(), os.size());
  factory->loadFonts(new_ba, &font_array);
  FontPtr new_font = font_array[0];

  // Check the font name.
  NameTablePtr name_table = down_cast<NameTable*>(font->table(Tag::name));
  UChar* name = name_table->name(PlatformId::kWindows,
                                 WindowsEncodingId::kUnicodeUCS2,
                                 WindowsLanguageId::kEnglish_UnitedStates,
                                 NameId::kFontFamilyName);

  EXPECT_EQ(u_strcmp(name, original_name), 0);
  delete name;
  delete original_name;

  return true;
}

bool testRemoveOneName() {
  FontFactoryPtr factory = FontFactory::getInstance();
  FontBuilderArray font_builder_array;
  loadTestFile(factory, &font_builder_array);
  FontBuilderPtr font_builder = font_builder_array[0];

  NameTableBuilderPtr name_builder = down_cast<NameTable::Builder*>(
      font_builder->getTableBuilder(Tag::name));

  EXPECT_TRUE(name_builder->has(PlatformId::kWindows,
                                WindowsEncodingId::kUnicodeUCS2,
                                WindowsLanguageId::kEnglish_UnitedStates,
                                NameId::kFontFamilyName));
  EXPECT_TRUE(name_builder->remove(PlatformId::kWindows,
                                   WindowsEncodingId::kUnicodeUCS2,
                                   WindowsLanguageId::kEnglish_UnitedStates,
                                   NameId::kFontFamilyName));

  // Build the font.
  FontPtr font = font_builder->build();

  // Serialize and reload the serialized font.
  MemoryOutputStream os;
  factory->serializeFont(font, &os);
  FontArray font_array;
  ByteArrayPtr new_ba = new MemoryByteArray(os.get(), os.size());
  factory->loadFonts(new_ba, &font_array);
  FontPtr new_font = font_array[0];

  // Check the font name.
  NameTablePtr name_table = down_cast<NameTable*>(font->table(Tag::name));
  UChar* name = name_table->name(PlatformId::kWindows,
                                 WindowsEncodingId::kUnicodeUCS2,
                                 WindowsLanguageId::kEnglish_UnitedStates,
                                 NameId::kFontFamilyName);
  EXPECT_TRUE(name == NULL);

  return true;
}

// Note: Function is not implemented but the test case is built.  Uncomment
//       when NameTable::clear() is implemented.
/*
bool testClearAllNamesAndSetOne() {
  FontFactoryPtr factory = FontFactory::getInstance();
  FontBuilderArray font_builder_array;
  loadTestFile(factory, &font_builder_array);
  FontBuilderPtr font_builder = font_builder_array[0];

  NameTableBuilderPtr name_builder = down_cast<NameTable::Builder*>(
      font_builder->getTableBuilder(Tag::name));

  EXPECT_GT(name_builder->builderCount(), 0);
  name_builder->clear();
  EXPECT_EQ(name_builder->builderCount(), 0);

  // Change the font name.
  NameEntryBuilderPtr neb =
      name_builder->nameBuilder(PlatformId::kWindows,
                                WindowsEncodingId::kUnicodeUCS2,
                                WindowsLanguageId::kEnglish_UnitedStates,
                                NameId::kFontFamilyName);
  U_STRING_DECL(new_name, "Fred", 4);
  neb->setName(new_name);

  // Build the font.
  FontPtr font = font_builder->build();

  // Serialize and reload the serialized font.
  MemoryOutputStream os;
  factory->serializeFont(font, &os);
  FontArray font_array;
  ByteArrayPtr new_ba = new MemoryByteArray(os.get(), os.size());
  factory->loadFonts(new_ba, &font_array);
  FontPtr new_font = font_array[0];

  // Check the font name.
  NameTablePtr name_table = down_cast<NameTable*>(font->table(Tag::name));
  UChar* name = name_table->name(PlatformId::kWindows,
                                 WindowsEncodingId::kUnicodeUCS2,
                                 WindowsLanguageId::kEnglish_UnitedStates,
                                 NameId::kFontFamilyName);
  EXPECT_EQ(name_table->nameCount(), 1);
  EXPECT_EQ(u_strcmp(name, new_name), 0);
  return true;
}
*/

}  // namespace sfntly
