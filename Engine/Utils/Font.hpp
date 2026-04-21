#pragma once

#include <cstdint>
#include <cstring>
#include <ostream>
#include <unordered_map>
#include <vector>

namespace Bored {
struct FontTable {
  char *data;
};

// Required tables
struct cmapSubtable {
  uint16_t platformID;
  uint16_t platformSpecificID;
  uint32_t offset;
};

struct cmapTable : public FontTable {
  uint16_t version;
  uint16_t numberSubtables;

  std::vector<cmapSubtable> subtables;
};

struct glyphDesc {
  int16_t numberOfContours;
  int16_t xMin;
  int16_t yMin;
  int16_t xMax;
  int16_t yMax;
};

struct simpleGlyph {
  uint16_t n;
  std::vector<uint16_t> endPtsOfContours;
  uint16_t instructionLength;
  std::vector<uint8_t> instructions;
  std::vector<uint8_t> flags;
  std::vector<int16_t> x_coords, y_coords;
};

struct glyfTable : public FontTable {
  glyphDesc glyphDesc;
  std::unordered_map<uint16_t, simpleGlyph> simpleGlyphs;
};

struct headTable : public FontTable {
  int32_t version;
  int32_t fontRevision;
  uint32_t checkSumAdjustment;
  uint32_t magicNumber;
  uint16_t flags;
  uint16_t unitsPerEm;
  int64_t created;
  int64_t modified;
  int16_t xMin;
  int16_t yMin;
  int16_t xMax;
  int16_t yMax;
  uint16_t macStyle;
  uint16_t lowestRecPPEM;
  int16_t fontDirectionHint;
  int16_t indexToLocFormat;
  int16_t glyphDataFormat;
};

struct hheaTable : public FontTable {};

struct hmtxTable : public FontTable {};

struct locaTable : public FontTable {
  std::vector<std::pair<uint32_t, uint32_t>> offsets;
};

struct maxpTable : public FontTable {
  int32_t version;
  uint16_t numGlyphs;
  uint16_t maxPoints;
  uint16_t maxContours;
  uint16_t maxComponentPoints;
  uint16_t maxComponentContours;
  uint16_t maxZones;
  uint16_t maxTwilightPoints;
  uint16_t maxStorage;
  uint16_t maxFunctionDefs;
  uint16_t maxInstructionDefs;
  uint16_t maxStackElements;
  uint16_t maxSizeOfInstructions;
  uint16_t maxComponentElements;
  uint16_t maxComponentDepth;
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
  uint32_t scaler_type;   /*< Indicate the OFA scaler to be used. */
  uint16_t numTables;     /*< Number of tables */
  uint16_t searchRange;   /*< (Maximum power of 2 <= numTables) * 16 */
  uint16_t entrySelector; /*< log_2(maximum power of 2 <= numTables) */
  uint16_t rangeShift;    /*< numTables * 16 - searchRange */

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
  uint32_t tag;      /*< Table name */
  uint32_t checkSum; /*< Checksum of the table content */
  uint32_t offset;   /*< Offset from beginning of sfnt */
  uint32_t length;   /*< Length of the table in byte (not including padding) */

  friend std::ostream &operator<<(std::ostream &os,
                                  const TableDirectoy &directory) {
    char name[5];
    memcpy(name, &directory.tag, 4);

    os << "\t\tTable name: " << name << std::endl;
    os << "\t\tCheck sum: " << directory.checkSum << std::endl;
    os << "\t\tOffset: " << directory.offset << std::endl;
    os << "\t\tLength: " << directory.length << std::endl;

    return os;
  }
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
