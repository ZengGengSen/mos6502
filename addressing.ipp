#pragma once

#ifndef MOS6502_ADDRESSING_ICC_
#define MOS6502_ADDRESSING_ICC_

namespace sen {

template<> auto MOS6502::addressing<MOS6502::AddressingCode::Accumulator>() -> uint16 { return PC; }
template<> auto MOS6502::addressing<MOS6502::AddressingCode::Implied>() -> uint16 { return PC; }
template<> auto MOS6502::addressing<MOS6502::AddressingCode::Immediate>() -> uint16 { return PC++; }
template<> auto MOS6502::addressing<MOS6502::AddressingCode::Relative>() -> uint16 { return PC++; }
template<> auto MOS6502::addressing<MOS6502::AddressingCode::Absolute>() -> uint16 {
  uint16 address;
  address = read(PC++);
  address |= read(PC++) << 8;
  return address;
}
template<> auto MOS6502::addressing<MOS6502::AddressingCode::AbsoluteXRead>() -> uint16 { return absoluteIndexed(X, false); }
template<> auto MOS6502::addressing<MOS6502::AddressingCode::AbsoluteXWrite>() -> uint16 { return absoluteIndexed(X, true); }
template<> auto MOS6502::addressing<MOS6502::AddressingCode::AbsoluteYRead>() -> uint16 { return absoluteIndexed(Y, false); }
template<> auto MOS6502::addressing<MOS6502::AddressingCode::AbsoluteYWrite>() -> uint16 { return absoluteIndexed(Y, true); }
template<> auto MOS6502::addressing<MOS6502::AddressingCode::Indirect>() -> uint16 {
  reg16 indirect{};

  indirect.w = addressing<AddressingCode::Absolute>();
  uint16 address = read(indirect.w);
  // MOS6502: $00ff wraps here to $0000; not $0100
  ++indirect.lo;
  address |= read(indirect.w) << 8;
  return address;
}
template<> auto MOS6502::addressing<MOS6502::AddressingCode::IndirectX>() -> uint16 {
  uint8 zeroPage = read(PC++);
  idleZeroPage(zeroPage);
  zeroPage += X;

  uint16 address = load(zeroPage);
  address |= load(zeroPage + 1) << 8;

  return address;
}
template<> auto MOS6502::addressing<MOS6502::AddressingCode::IndirectYRead>() -> uint16 {
  return indirectYIndexed(false);
}
template<> auto MOS6502::addressing<MOS6502::AddressingCode::IndirectYWrite>() -> uint16 {
  return indirectYIndexed(true);
}
template<> auto MOS6502::addressing<MOS6502::AddressingCode::ZeroPage>() -> uint16 { return read(PC++); }
template<> auto MOS6502::addressing<MOS6502::AddressingCode::ZeroPageX>() -> uint16 {
  uint8 zeroPage = read(PC++);
  idleZeroPage(zeroPage);
  zeroPage += X;
  return zeroPage;
}
template<> auto MOS6502::addressing<MOS6502::AddressingCode::ZeroPageY>() -> uint16 {
  uint8 zeroPage = read(PC++);
  idleZeroPage(zeroPage);
  zeroPage += Y;
  return zeroPage;
}

auto MOS6502::absoluteIndexed(uint8 index, bool is_write) -> uint16 {
  uint16 address = addressing<AddressingCode::Absolute>();
  idlePageCrossed(address, address + index, is_write);
  return address + index;
}
auto MOS6502::indirectYIndexed(bool is_write) -> uint16 {
  uint8 zeroPage = read(PC++);
  uint16 absolute = load(zeroPage++);
  absolute |= load(zeroPage) << 8;
  idlePageCrossed(absolute, absolute + Y, is_write);
  return absolute + Y;
}

}

#endif //MOS6502_ADDRESSING_ICC_
