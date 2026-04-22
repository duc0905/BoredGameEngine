#include "Font.hpp"
#include <bitset>
#include <cstring>
#include <filesystem>
// #include <fstream>
#include <ios>
#include <iostream>
#include <unordered_map>
#include <vector>

namespace Bored {

/**
 * Converting from little endian to big endian and vice-versa.
 *
 * Reversing the bytes of the variable.
 */
template <typename T> void ReverseBytes(T &value) {
  uint8_t numBytes = sizeof(T);
  char *temp = new char[numBytes];
  memcpy(temp, &value, numBytes);

  for (int i = 0; i < numBytes / 2; i++) {
    temp[i] += temp[numBytes - 1 - i];
    temp[numBytes - 1 - i] = temp[i] - temp[numBytes - 1 - i];
    temp[i] = temp[i] - temp[numBytes - 1 - i];
  }

  value = *(T *)temp;
  delete[] temp;
}

void Font::ParseCmapTable(TableDirectoy &table_dir, std::ifstream &file) {
  file.seekg(table_dir.offset, std::ios::beg);

  file.read((char *)&cmap_table.version, sizeof(uint16_t));
  ReverseBytes(cmap_table.version);

  file.read((char *)&cmap_table.numberSubtables, sizeof(uint16_t));
  ReverseBytes(cmap_table.numberSubtables);

  for (int i = 0; i < cmap_table.numberSubtables; i++) {
    cmapSubtable sub;

    file.read((char *)&sub.platformID, sizeof(uint16_t));
    ReverseBytes(sub.platformID);

    file.read((char *)&sub.platformSpecificID, sizeof(uint16_t));
    ReverseBytes(sub.platformSpecificID);

    file.read((char *)&sub.offset, sizeof(uint32_t));
    ReverseBytes(sub.offset);

    cmap_table.subtables.push_back(sub);

    std::cout << "Subtable: " << sub.platformID << " " << sub.platformSpecificID
              << " " << sub.offset << std::endl;
  }
}

void Font::ParseMaxpTable(TableDirectoy &table_dir, std::ifstream &file) {
  file.seekg(table_dir.offset, std::ios::beg);

  file.read((char *)&maxp_table.version, sizeof(int32_t));
  ReverseBytes(maxp_table.version);

  file.read((char *)&maxp_table.numGlyphs, sizeof(uint16_t));
  ReverseBytes(maxp_table.numGlyphs);

  file.read((char *)&maxp_table.maxPoints, sizeof(uint16_t));
  ReverseBytes(maxp_table.maxPoints);

  file.read((char *)&maxp_table.maxContours, sizeof(uint16_t));
  ReverseBytes(maxp_table.maxContours);

  file.read((char *)&maxp_table.maxComponentPoints, sizeof(uint16_t));
  ReverseBytes(maxp_table.maxComponentPoints);

  file.read((char *)&maxp_table.maxComponentContours, sizeof(uint16_t));
  ReverseBytes(maxp_table.maxComponentContours);

  file.read((char *)&maxp_table.maxZones, sizeof(uint16_t));
  ReverseBytes(maxp_table.maxZones);

  file.read((char *)&maxp_table.maxTwilightPoints, sizeof(uint16_t));
  ReverseBytes(maxp_table.maxTwilightPoints);

  file.read((char *)&maxp_table.maxStorage, sizeof(uint16_t));
  ReverseBytes(maxp_table.maxStorage);

  file.read((char *)&maxp_table.maxFunctionDefs, sizeof(uint16_t));
  ReverseBytes(maxp_table.maxFunctionDefs);

  file.read((char *)&maxp_table.maxInstructionDefs, sizeof(uint16_t));
  ReverseBytes(maxp_table.maxInstructionDefs);

  file.read((char *)&maxp_table.maxStackElements, sizeof(uint16_t));
  ReverseBytes(maxp_table.maxStackElements);

  file.read((char *)&maxp_table.maxSizeOfInstructions, sizeof(uint16_t));
  ReverseBytes(maxp_table.maxSizeOfInstructions);

  file.read((char *)&maxp_table.maxComponentElements, sizeof(uint16_t));
  ReverseBytes(maxp_table.maxComponentElements);

  file.read((char *)&maxp_table.maxComponentDepth, sizeof(uint16_t));
  ReverseBytes(maxp_table.maxComponentDepth);
}

void Font::ParseHeadTable(TableDirectoy &table_dir, std::ifstream &file) {
  file.seekg(table_dir.offset, std::ios::beg);

  file.read((char *)&head_table.version, sizeof(int32_t));
  ReverseBytes(head_table.version);

  file.read((char *)&head_table.fontRevision, sizeof(int32_t));
  ReverseBytes(head_table.fontRevision);

  file.read((char *)&head_table.checkSumAdjustment, sizeof(uint32_t));
  ReverseBytes(head_table.checkSumAdjustment);

  file.read((char *)&head_table.magicNumber, sizeof(uint32_t));
  ReverseBytes(head_table.magicNumber);

  file.read((char *)&head_table.flags, sizeof(uint16_t));
  ReverseBytes(head_table.flags);

  file.read((char *)&head_table.unitsPerEm, sizeof(uint16_t));
  ReverseBytes(head_table.unitsPerEm);

  file.read((char *)&head_table.created, sizeof(int64_t));
  ReverseBytes(head_table.created);

  file.read((char *)&head_table.modified, sizeof(int64_t));
  ReverseBytes(head_table.modified);

  file.read((char *)&head_table.xMin, sizeof(int16_t));
  ReverseBytes(head_table.xMin);

  file.read((char *)&head_table.yMin, sizeof(int16_t));
  ReverseBytes(head_table.yMin);

  file.read((char *)&head_table.xMax, sizeof(int16_t));
  ReverseBytes(head_table.xMax);

  file.read((char *)&head_table.yMax, sizeof(int16_t));
  ReverseBytes(head_table.yMax);

  file.read((char *)&head_table.macStyle, sizeof(uint16_t));
  ReverseBytes(head_table.macStyle);

  file.read((char *)&head_table.lowestRecPPEM, sizeof(uint16_t));
  ReverseBytes(head_table.lowestRecPPEM);

  file.read((char *)&head_table.fontDirectionHint, sizeof(int16_t));
  ReverseBytes(head_table.fontDirectionHint);

  file.read((char *)&head_table.indexToLocFormat, sizeof(int16_t));
  ReverseBytes(head_table.indexToLocFormat);

  file.read((char *)&head_table.glyphDataFormat, sizeof(int16_t));
  ReverseBytes(head_table.glyphDataFormat);
}

void Font::ParseLocaTable(TableDirectoy &table_dir, std::ifstream &file) {
  file.seekg(table_dir.offset, std::ios::beg);

  int n = maxp_table.numGlyphs + 1;
  loca_table.offsets.resize(n);

  for (int i = 0; i < n; i++) {
    if (head_table.indexToLocFormat == 0) { // Short version
      uint16_t offset;
      file.read((char *)&offset, sizeof(uint16_t));
      ReverseBytes(offset);
      loca_table.offsets[i] = {offset * 2, 0};
    } else { // Long version
      uint32_t offset;
      file.read((char *)&offset, sizeof(uint32_t));
      ReverseBytes(offset);
      loca_table.offsets[i] = {offset, 0};
    }
  }

  for (int i = 0; i < n - 1; i++) {
    loca_table.offsets[i].second =
        loca_table.offsets[i + 1].first - loca_table.offsets[i].first;
  }
}

struct TempSimpleGlyph {
  GlyphDesc glyphDesc;
  uint16_t n = 0;
  std::vector<uint16_t> endPtsOfContours;
  uint16_t instructionLength = 0;
  std::vector<uint8_t> instructions;
  std::vector<uint8_t> flags;
  std::vector<int16_t> x_coords, y_coords;
};

struct TempGlyphComponent {
  uint16_t flag = 0;
  uint16_t glyphIndex = 0;
  int16_t a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;
  uint16_t compound_point = 0, component_point = 0;
};

struct TempCompoundGlyph {
  GlyphDesc glyphDesc;
  std::vector<TempGlyphComponent> components;
};

void Font::ParseGlyfTable(TableDirectoy &table_dir, std::ifstream &file) {
  file.seekg(table_dir.offset, std::ios::beg);

  std::unordered_map<uint16_t, TempCompoundGlyph> temp_compoundGlyphs;
  std::unordered_map<uint16_t, TempSimpleGlyph> temp_simpleGlyphs;

  uint16_t numGlyphs = maxp_table.numGlyphs;
  // uint16_t numGlyphs = 140;
  for (int i = 0; i < numGlyphs; i++) {
    GlyphDesc desc;

    if (loca_table.offsets[i].second == 0)
      continue;

    file.seekg(table_dir.offset + loca_table.offsets[i].first, std::ios::beg);

    file.read((char *)&desc.numberOfContours, sizeof(int16_t));
    ReverseBytes(desc.numberOfContours);

    file.read((char *)&desc.xMin, sizeof(int16_t));
    ReverseBytes(desc.xMin);

    file.read((char *)&desc.yMin, sizeof(int16_t));
    ReverseBytes(desc.yMin);

    file.read((char *)&desc.xMax, sizeof(int16_t));
    ReverseBytes(desc.xMax);

    file.read((char *)&desc.xMax, sizeof(int16_t));
    ReverseBytes(desc.yMax);

    if (desc.numberOfContours > 0) { // Simple glyph
      TempSimpleGlyph g;
      uint16_t end = 0;
      uint8_t instruction;

      for (int _ = 0; _ < desc.numberOfContours; _++) {
        file.read((char *)&end, sizeof(uint16_t));
        ReverseBytes(end);

        g.endPtsOfContours.push_back(end);

        g.n = end + 1;
      }

      file.read((char *)&g.instructionLength, sizeof(uint16_t));
      ReverseBytes(g.instructionLength);

      for (int _ = 0; _ < g.instructionLength; _++) {
        file.read((char *)&instruction, sizeof(uint8_t));
        g.instructions.push_back(instruction);
      }

      for (int j = 0; j < g.n; j++) {
        uint8_t flag;
        uint8_t repeat;
        file.read((char *)&flag, sizeof(uint8_t));
        g.flags.push_back(flag);

        if (flag & 0b00001000) {
          file.read((char *)&repeat, sizeof(uint8_t));
          for (int k = 0; k < repeat; k++)
            g.flags.push_back(flag);
          j += repeat;
        }
      }

      for (int j = 0; j < g.n; j++) {
        bool x_short = g.flags[j] & 0b00000010;
        bool x_is_same = g.flags[j] & 0b00010000;
        int16_t x_final;

        if (x_short) {
          uint8_t x;
          file.read((char *)&x, sizeof(uint8_t));

          if (x_is_same) {
            x_final = x;
          } else {
            x_final = -x;
          }
        } else {
          if (x_is_same) {
            x_final = 0;
          } else {
            file.read((char *)&x_final, sizeof(int16_t));
            ReverseBytes(x_final);
          }
        }

        g.x_coords.push_back(x_final);
      }

      for (int j = 0; j < g.n; j++) {
        bool y_short = g.flags[j] & 0b00000100;
        bool y_is_same = g.flags[j] & 0b00100000;
        int16_t y_final;

        if (y_short) {
          uint8_t x;
          file.read((char *)&x, sizeof(uint8_t));

          if (y_is_same) {
            y_final = x;
          } else {
            y_final = -x;
          }
        } else {
          if (y_is_same) {
            y_final = 0;
          } else {
            file.read((char *)&y_final, sizeof(int16_t));
            ReverseBytes(y_final);
          }
        }

        g.y_coords.push_back(y_final);
      }

      for (int j = 1; j < g.n; j++) {
        g.x_coords[j] = g.x_coords[j - 1] + g.x_coords[j];
        g.y_coords[j] = g.y_coords[j - 1] + g.y_coords[j];
      }

      temp_simpleGlyphs[i] = g;
    } else { // Compound glyph
      TempCompoundGlyph g;
      g.glyphDesc = desc;

      while (true) {
        TempGlyphComponent comp;

        file.read((char *)&comp.flag, sizeof(uint16_t));
        ReverseBytes(comp.flag);

        file.read((char *)&comp.glyphIndex, sizeof(uint16_t));
        ReverseBytes(comp.glyphIndex);

        // std::cout << i << " flag: " << std::bitset<16>(comp.flag) <<
        // std::endl;

        bool arg_1_and_2_are_words = comp.flag & (1 << 0);
        bool args_are_xy_values = comp.flag & (1 << 1);
        bool we_have_a_scale = comp.flag & (1 << 3);
        bool more_component = comp.flag & (1 << 5);
        bool we_have_an_x_and_y_scale = comp.flag & (1 << 6);
        bool we_have_a_2_by_2 = comp.flag & (1 << 7);

        if (arg_1_and_2_are_words && args_are_xy_values) {
          file.read((char *)&comp.e, sizeof(int16_t));
          ReverseBytes(comp.e);

          file.read((char *)&comp.f, sizeof(int16_t));
          ReverseBytes(comp.f);
        } else if (!arg_1_and_2_are_words && args_are_xy_values) {
          int8_t x, y;
          file.read((char *)&x, sizeof(int8_t));
          comp.e = x;

          file.read((char *)&y, sizeof(int8_t));
          comp.f = y;
        } else if (arg_1_and_2_are_words && !args_are_xy_values) {
          file.read((char *)&comp.compound_point, sizeof(uint16_t));
          ReverseBytes(comp.compound_point);

          file.read((char *)&comp.component_point, sizeof(uint16_t));
          ReverseBytes(comp.component_point);
        } else {
          uint8_t x, y;
          file.read((char *)&x, sizeof(int8_t));
          comp.compound_point = x;

          file.read((char *)&y, sizeof(int8_t));
          comp.component_point = y;
        }

        if (we_have_a_scale) {
          int16_t s;
          file.read((char *)&s, sizeof(int16_t));
          ReverseBytes(s);
          comp.a = s;
          comp.b = 0;
          comp.c = 0;
          comp.d = s;
        } else if (we_have_an_x_and_y_scale) {
          file.read((char *)&comp.a, sizeof(int16_t));
          ReverseBytes(comp.a);

          file.read((char *)&comp.d, sizeof(int16_t));
          ReverseBytes(comp.d);

          comp.b = 0;
          comp.c = 0;
        } else if (we_have_a_2_by_2) {
          file.read((char *)&comp.a, sizeof(int16_t));
          ReverseBytes(comp.a);

          file.read((char *)&comp.b, sizeof(int16_t));
          ReverseBytes(comp.b);

          file.read((char *)&comp.c, sizeof(int16_t));
          ReverseBytes(comp.c);

          file.read((char *)&comp.d, sizeof(int16_t));
          ReverseBytes(comp.d);
        } else {
          comp.a = 1;
          comp.b = 0;
          comp.c = 0;
          comp.d = 1;
        }

        g.components.push_back(comp);

        if (!more_component)
          break;
      }

      temp_compoundGlyphs[i] = g;
    }
  }

  // NOTE: Find the implicit points and convert to easier data structure
  for (auto &&[idx, g] : temp_simpleGlyphs) {
    std::vector<bool> temp_on_curves;
    std::vector<GlyphPoint> points;

    for (int i = 0; i < g.n - 1; i++) {
      auto &flag = g.flags[i];
      bool on_curve = flag & (1 << 0);
      auto x = g.x_coords[i];
      auto y = g.y_coords[i];

      auto &next_flag = g.flags[i + 1];
      bool next_on_curve = next_flag & (1 << 0);
      auto next_x = g.x_coords[i + 1];
      auto next_y = g.y_coords[i + 1];

      temp_on_curves.push_back(on_curve);
      points.push_back({x, y});

      if (on_curve == next_on_curve) {
        bool inter_on_curve = !on_curve;
        int16_t inter_x = (x + next_x) / 2;
        int16_t inter_y = (y + next_y) / 2;

        temp_on_curves.push_back(inter_on_curve);
        points.push_back({inter_x, inter_y});
      }
    }

    {
      auto &flag = g.flags[g.n - 1];
      bool on_curve = flag & (1 << 0);
      auto x = g.x_coords[g.n - 1];
      auto y = g.y_coords[g.n - 1];

      auto &next_flag = g.flags[0];
      bool next_on_curve = next_flag & (1 << 0);
      auto next_x = g.x_coords[0];
      auto next_y = g.y_coords[0];

      temp_on_curves.push_back(on_curve);
      points.push_back({x, y});

      if (on_curve == next_on_curve) {
        bool inter_on_curve = !on_curve;
        int16_t inter_x = (x + next_x) / 2;
        int16_t inter_y = (y + next_y) / 2;

        temp_on_curves.push_back(inter_on_curve);
        points.push_back({inter_x, inter_y});
      }
    }

    std::vector<char> on_curves((temp_on_curves.size() + 7) / 8, 0);
    for (int i = 0; i < temp_on_curves.size(); i++) {
      size_t byte_idx = i / 8;
      size_t bit_offset = i % 8;

      if (temp_on_curves[i]) {
        on_curves[byte_idx] |= (1 << bit_offset);
      }
    }

    glyf_table.simpleGlyphs[idx] = {g.glyphDesc, on_curves, points};
  }

  // TODO: Convert compound glyphs too

}

std::ostream &operator<<(std::ostream &os, const TableDirectoy &directory) {
  char name[5];
  memcpy(name, &directory.tag, 4);
  name[4] = '\0';

  os << "\t\tTable name: " << name << std::endl;
  os << "\t\tCheck sum: " << directory.checkSum << std::endl;
  os << "\t\tOffset: " << directory.offset << std::endl;
  os << "\t\tLength: " << directory.length << std::endl;

  return os;
}

Bored::Font::Font(const std::string &filepath) {
  // TODO: implement
  std::ifstream file(filepath, std::ios::binary);
  std::filesystem::path path(filepath);

  if (!file) {
    throw std::runtime_error(
        std::format("Error while opening file {}", filepath));
  }

  OffsetSubtable offset_subtable;
  std::unordered_map<std::string, TableDirectoy> table_dirs;

  // Big-endian to little endian
  file.read((char *)&offset_subtable.scaler_type, sizeof(uint32_t));
  ReverseBytes(offset_subtable.scaler_type);

  file.read((char *)&offset_subtable.numTables, sizeof(uint16_t));
  ReverseBytes(offset_subtable.numTables);

  file.read((char *)&offset_subtable.searchRange, sizeof(uint16_t));
  ReverseBytes(offset_subtable.searchRange);

  file.read((char *)&offset_subtable.entrySelector, sizeof(uint16_t));
  ReverseBytes(offset_subtable.entrySelector);

  file.read((char *)&offset_subtable.rangeShift, sizeof(uint16_t));
  ReverseBytes(offset_subtable.rangeShift);

  // std::cout << offset_subtable << std::endl;

  for (int i = 0; i < offset_subtable.numTables; i++) {
    TableDirectoy table_dir;
    char name[5];
    // Read table entries
    file.read((char *)&table_dir.tag, sizeof(uint32_t));
    name[4] = '\0';
    memcpy(name, &table_dir.tag, 4);

    file.read((char *)&table_dir.checkSum, sizeof(uint32_t));
    ReverseBytes(table_dir.checkSum);
    file.read((char *)&table_dir.offset, sizeof(uint32_t));
    ReverseBytes(table_dir.offset);
    file.read((char *)&table_dir.length, sizeof(uint32_t));
    ReverseBytes(table_dir.length);

    table_dirs[name] = table_dir;

    // std::cout << table_dir << std::endl;
  }

  ParseMaxpTable(table_dirs["maxp"], file);
  ParseHeadTable(table_dirs["head"], file);
  ParseLocaTable(table_dirs["loca"], file);
  ParseGlyfTable(table_dirs["glyf"], file);

  file.close();
}

} // namespace Bored
