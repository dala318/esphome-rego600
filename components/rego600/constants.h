#pragma once

#include <cstdint>

namespace esphome {
namespace rego {

static const std::int16_t registers = 0x0000;
/*
600-635      636-         Register */ /*
//Sensor values
0x0209         0x020B         "Radiator return [GT1]"
0x020A         0x020C         "Outdoor [GT2]"
0x020B         0x020D         "Hot water [GT3]"
0x020C         0x020E         "Forward [GT4]"
0x020D         0x020F         "Room [GT5]"
0x020E         0x0210         "Compressor [GT6]"
0x020F         0x0211         "Heat fluid out [GT8]"
0x0210         0x0212         "Heat fluid in [GT9]"
0x0211         0x0213         "Cold fluid in [GT10]"
0x0212         0x0214         "Cold fluid out [GT11]"
0x0213         0x0215         "External hot water [GT3x]"
//Device values
0x01FD         0x01FF         "Ground loop pump [P3]"
0x01FE         0x0200         "Compresor"
0x01FF         0x0201         "Additional heat 3kW"
0x0200         0x0202         "Additional heat 6kW"
0x0203         0x0205         "Radiator pump [P1]"
0x0204         0x0206         "Heat carrier pump [P2]"
0x0205         0x0207         "Tree-way valve [VXV]"
0x0206         0x0208         "Alarm"
//Control data
0x006E         0x006E         "GT1 Target value"
0x006F         0x006F         "GT1 On value"
0x0070         0x0070         "GT1 Off value"
0x002B         0x002B         "GT3 Target value"
0x0073         0x0073         "GT3 On value"
0x0074         0x0074         "GT3 Off value"
0x006D         0x006D         "GT4 Target value"
0x006C         0x006C         "Add heat power in %"
//Settings
0x0000         0x0000         "Heat curve"
0x0001         0x0001         "Heat curve fine adj."
0x0021         0x0021         "Indoor temp setting"
0x0022         0x0022         "Curve infl. by in-temp."
0x001E         0x001E         "Adj. curve at 20° out"
0x001C         0x001C         "Adj. curve at 15° out"
0x001A         0x001A         "Adj. curve at 10° out"
0x0018         0x0018         "Adj. curve at 5° out"
0x0016         0x0016         "Adj. curve at 0° out"
0x0014         0x0014         "Adj. curve at -5° out"
0x0012         0x0012         "Adj. curve at -10° out"
0x0010         0x0010         "Adj. curve at -15° out"
0x000E         0x000E         "Adj. curve at -20° out"
0x000C         0x000C         "Adj. curve at -25° out"
0x000A         0x000A         "Adj. curve at -30° out"
0x0008         0x0008         "Adj. curve at -35° out"
0x0002         0x0002         "Heat curve coupling diff."
*/

}  // namespace rego
}  // namespace esphome
