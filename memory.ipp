#pragma once
#ifndef MOS6502_MEMORY_ICC_
#define MOS6502_MEMORY_ICC_

#include "mos6502.hpp"

namespace sen {

auto MOS6502::opcode() -> uint8 { return read(PC++); }
auto MOS6502::operand() -> uint8 { return read(PC++); }

// zero page
auto MOS6502::load(uint8 addr) -> uint8 { return read(addr); }
auto MOS6502::store(uint8 addr, uint8 data) -> void { write(addr, data); }

// stack page
auto MOS6502::pull() -> uint8 { return read(0x100 | ++S); }
auto MOS6502::push(uint8 data) -> void { write(0x100 | S--, data); }

// dummy read
auto MOS6502::idleRead() -> void { read(PC); }
auto MOS6502::idleZeroPage(uint8 addr) -> void { read(addr); }
auto MOS6502::idleStackPointer() -> void { read(0x100 | S); }
auto MOS6502::idlePageCrossed(uint16 x, uint16 y, bool isWrite) -> void {
  if (!isWrite && x >> 8 == y >> 8) return;
  read((x & 0xff00) | (y & 0x00ff));
}

}

#endif //MOS6502_MEMORY_ICC_
