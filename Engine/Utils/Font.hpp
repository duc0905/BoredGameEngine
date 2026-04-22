#pragma once

#include <cstdint>
// #include <cstring>
// #include <ostream>
#include <fstream>
#include <unordered_map>
#include <vector>

namespace Bored {
struct FontTable {
  char *data;
};

// Required tables
struct cmapSubtable {
  uint16_t platformID = 0;
  uint16_t platformSpecificID = 0;
  uint32_t offset = 0;
};

struct cmapTable : public FontTable {
  uint16_t version = 0;
  uint16_t numberSubtables = 0;

  std::vector<cmapSubtable> subtables;
};

struct GlyphDesc {
  int16_t numberOfContours = 0;
  int16_t xMin = 0;
  int16_t yMin = 0;
  int16_t xMax = 0;
  int16_t yMax = 0;
};

struct GlyphPoint {
  int16_t x, y;
};

struct SimpleGlyph {
  GlyphDesc desc;
  std::vector<char> on_curves; // Seperate for space efficiency
  std::vector<GlyphPoint> points;
};

struct CompoundGlyph {};

struct glyfTable : public FontTable {
  std::unordered_map<uint16_t, CompoundGlyph> compoundGlyphs;
  std::unordered_map<uint16_t, SimpleGlyph> simpleGlyphs;
};

struct headTable : public FontTable {
  int32_t version = 0;
  int32_t fontRevision = 0;
  uint32_t checkSumAdjustment = 0;
  uint32_t magicNumber = 0;
  uint16_t flags = 0;
  uint16_t unitsPerEm = 0;
  int64_t created = 0;
  int64_t modified = 0;
  int16_t xMin = 0;
  int16_t yMin = 0;
  int16_t xMax = 0;
  int16_t yMax = 0;
  uint16_t macStyle = 0;
  uint16_t lowestRecPPEM = 0;
  int16_t fontDirectionHint = 0;
  int16_t indexToLocFormat = 0;
  int16_t glyphDataFormat = 0;
};

struct hheaTable : public FontTable {};

struct hmtxTable : public FontTable {};

struct locaTable : public FontTable {
  std::vector<std::pair<uint32_t, uint32_t>> offsets;
};

struct maxpTable : public FontTable {
  int32_t version = 0;
  uint16_t numGlyphs = 0;
  uint16_t maxPoints = 0;
  uint16_t maxContours = 0;
  uint16_t maxComponentPoints = 0;
  uint16_t maxComponentContours = 0;
  uint16_t maxZones = 0;
  uint16_t maxTwilightPoints = 0;
  uint16_t maxStorage = 0;
  uint16_t maxFunctionDefs = 0;
  uint16_t maxInstructionDefs = 0;
  uint16_t maxStackElements = 0;
  uint16_t maxSizeOfInstructions = 0;
  uint16_t maxComponentElements = 0;
  uint16_t maxComponentDepth = 0;
};

struct nameTable : public FontTable {};

struct postTable : public FontTable {};

// Optional tables

struct cvt_Table : public FontTable {};

struct fpgmTable : public FontTable {};

struct hdmxTable : public FontTable {};

struct kernTable : public FontTable {};

struct os_2Table : public FontTable {};

struct prepTable : public FontTable {};

/* Fonts */

struct FontDirectory {};

struct OffsetSubtable {
  uint32_t scaler_type = 0;   /*< Indicate the OFA scaler to be used. */
  uint16_t numTables = 0;     /*< Number of tables */
  uint16_t searchRange = 0;   /*< (Maximum power of 2 <= numTables) * 16 */
  uint16_t entrySelector = 0; /*< log_2(maximum power of 2 <= numTables) */
  uint16_t rangeShift = 0;    /*< numTables * 16 - searchRange */

  friend std::ostream &operator<<(std::ostream &os,
                                  const OffsetSubtable &subtable) {
    os << "\tScaler type: " << subtable.scaler_type << std::endl;
    os << "\tNumber of tables: " << subtable.numTables << std::endl;
    os << "\tSearch range: " << subtable.searchRange << std::endl;
    os << "\tEntry selector: " << subtable.entrySelector << std::endl;
    os << "\tRange shift: " << subtable.rangeShift << std::endl;

    return os;
  }
};

struct TableDirectoy {
  uint32_t tag = 0;      /*< Table name */
  uint32_t checkSum = 0; /*< Checksum of the table content */
  uint32_t offset = 0;   /*< Offset from beginning of sfnt */
  uint32_t length = 0;   /*< Length of the table in byte (excluding padding) */
};

class Font {
public:
  Font(const std::string &filepath);

private:
  void ParseHeadTable(TableDirectoy &table_dir, std::ifstream &file);
  void ParseCmapTable(TableDirectoy &table_dir, std::ifstream &file);
  void ParseMaxpTable(TableDirectoy &table_dir, std::ifstream &file);
  void ParseLocaTable(TableDirectoy &table_dir, std::ifstream &file);
  void ParseGlyfTable(TableDirectoy &table_dir, std::ifstream &file);

private:
  cmapTable cmap_table;
  glyfTable glyf_table;
  headTable head_table;
  hheaTable hhea_table;
  hmtxTable hmtx_table;
  locaTable loca_table;
  maxpTable maxp_table;
  nameTable name_table;
  postTable post_table;
};

}; // namespace Bored
