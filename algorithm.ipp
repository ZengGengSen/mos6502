#pragma once

#ifndef MOS6502_ALGORITHM_IPP_
#define MOS6502_ALGORITHM_IPP_

#include "utils/bit.hpp"

namespace sen {

// official algorithm
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::ADC>() -> void { A = addMemoryWithCarry(MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::AND>() -> void { A = loadMemory(MDR & A); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::ASL_A>() -> void { A = shiftLeft(A); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::ASL_M>() -> void { MDR = shiftLeft(MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::BCC>() -> void { branchOnStatusFlag(C == 0, MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::BCS>() -> void { branchOnStatusFlag(C == 1, MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::BEQ>() -> void { branchOnStatusFlag(Z == 1, MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::BIT>() -> void { bitMemory(MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::BMI>() -> void { branchOnStatusFlag(N == 1, MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::BNE>() -> void { branchOnStatusFlag(Z == 0, MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::BPL>() -> void { branchOnStatusFlag(N == 0, MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::BRK>() -> void {
  uint16 vector = 0xfffe;
  ++PC;
  push(PCH);
  push(PCL);
  nmi(vector);
  push(P | 0x30);
  I = 1;
  PCL = read(vector);
  PCH = read(vector + 1);

  // @test-file: nmi_and_brk test
  // Ensure we don't start an NMI right after running a BRK instruction
  // (first instruction in IRQ handler must run first)
  PNMI = false;
}
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::BVC>() -> void { branchOnStatusFlag(V == 0, MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::BVS>() -> void { branchOnStatusFlag(V == 1, MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::CLC>() -> void { C = 0; }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::CLD>() -> void { D = 0; }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::CLI>() -> void { I = 0; }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::CLV>() -> void { V = 0; }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::CMP>() -> void { compare(A, MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::CPX>() -> void { compare(X, MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::CPY>() -> void { compare(Y, MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::DEC>() -> void { MDR = loadMemory(MDR - 1); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::DEX>() -> void { X = loadMemory(X - 1); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::DEY>() -> void { Y = loadMemory(Y - 1); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::EOR>() -> void { A = loadMemory(A ^ MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::INC>() -> void { MDR = loadMemory(MDR + 1); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::INX>() -> void { X = loadMemory(X + 1); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::INY>() -> void { Y = loadMemory(Y + 1); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::JMP>() -> void { PC = MAR; }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::JSR>() -> void { idleStackPointer(); --PC; push(PCH); push(PCL); PC = MAR; }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::LDA>() -> void { A = loadMemory(MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::LDX>() -> void { X = loadMemory(MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::LDY>() -> void { Y = loadMemory(MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::LSR_A>() -> void { A = shiftRight(A); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::LSR_M>() -> void { MDR = shiftRight(MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::NOP>() -> void {}
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::ORA>() -> void { A = loadMemory(A | MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::PHA>() -> void { push(A); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::PHP>() -> void { push(P | 0x30); }  // B abd U set true
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::PLA>() -> void { idleStackPointer(); loadMemory(A = pull()); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::PLP>() -> void { idleStackPointer(); P = pull(); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::ROL_A>() -> void { A = rotateLeft(A); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::ROL_M>() -> void { MDR = rotateLeft(MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::ROR_A>() -> void { A = rotateRight(A); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::ROR_M>() -> void { MDR = rotateRight(MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::RTI>() -> void { idleStackPointer(); P = pull(); PCL = pull(); PCH = pull(); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::RTS>() -> void { idleStackPointer(); PCL = pull(); PCH = pull(); idleRead(); PC++; }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::SBC>() -> void { A = subtract(MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::SEC>() -> void { C = 1; }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::SED>() -> void { D = 1; }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::SEI>() -> void { I = 1; }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::STA>() -> void { MDR = A; }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::STX>() -> void { MDR = X; }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::STY>() -> void { MDR = Y; }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::TAX>() -> void { X = loadMemory(A); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::TAY>() -> void { Y = loadMemory(A); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::TSX>() -> void { X = loadMemory(S); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::TXA>() -> void { A = loadMemory(X); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::TXS>() -> void { S = X; }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::TYA>() -> void { A = loadMemory(Y); }

// unofficial algorithm
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::AAC>() -> void { A = loadMemory(A & MDR); C = bit<7>(A); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::AAX>() -> void { MDR = A & X; }
// This operation involves the adder:
// V-flag is set according to (A AND oper) + oper
// The carry is not set, but bit 7 (sign) is exchanged with the carry
template <> auto MOS6502::algorithm<MOS6502::AlgorithmCode::ARR>() -> void { A = loadMemory((A & MDR) >> 1 | C * 0x80); C = bool(A & 0x40); V = bool(C ^ ((A >> 5) & 0x01)); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::ASR>() -> void { A = shiftRight(A & MDR); }
// ATX(OAL) Is this(OR with $ee) correct? Blargg did some test
// and found the constant to be OR with is $ff for NES
// Store * AND oper in A and X
// Highly unstable, involves a 'magic' constant, see ANE
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::ATX>() -> void { X = A = loadMemory((A | 0xff) & MDR); }
// Stores A AND X AND (high-byte of addr. + 1) at addr.
// unstable: sometimes 'AND (H+1)' is dropped, page boundary
// crossings may not work (with the high-byte of the value
// used as the high-byte of the address)
// fceux: MDR = A & X & (((MAR - Y) >> 8) + 1)
// other: MDR = A & X & ((MAR >> 8) + 1)
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::AXA>() -> void { MDR = A & X & (((MAR - Y) >> 8) + 1); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::AXS>() -> void { X = compare(A & X, MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::DCP>() -> void { MDR = loadMemory(MDR - 1); compare(A, MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::ISB>() -> void { MDR = loadMemory(MDR + 1); A = subtract(MDR); }
template <> auto MOS6502::algorithm<MOS6502::AlgorithmCode::LAR>() -> void { S &= MDR; A = X = loadMemory(S); }
template <> auto MOS6502::algorithm<MOS6502::AlgorithmCode::LAX>() -> void { A = X = loadMemory(MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::RLA>() -> void { MDR = rotateLeft(MDR); A = loadMemory(A & MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::RRA>() -> void { MDR = rotateRight(MDR); A = addMemoryWithCarry(MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::SLO>() -> void { MDR = shiftLeft(MDR); A = loadMemory(A | MDR); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::SRE>() -> void { MDR = shiftRight(MDR); A = loadMemory(A ^ MDR); }
// Stores X AND (high-byte of addr. + 1) at addr.
// unstable: sometimes 'AND (H+1)' is dropped, page
// boundary crossings may not work (with the high-byte
// of the value used as the high-byte of the address)
// X AND (H+1) -> M
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::SXA>() -> void { MAR = ((X & ((MAR >> 8) + 1)) << 8) | (MAR & 0xff); MDR = MAR >> 8; }
// Stores Y AND (high-byte of addr. + 1) at addr.
// unstable: sometimes 'AND (H+1)' is dropped, page
// boundary crossings may not work (with the high-byte
// of the value used as the high-byte of the address)
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::SYA>() -> void { MAR = ((Y & ((MAR >> 8) + 1)) << 8) | (MAR & 0xff); MDR = MAR >> 8; }
// Puts A AND X in SP and stores A AND X AND (high-byte
// of addr. + 1) at addr.
// unstable: sometimes 'AND (H+1)' is dropped, page
// boundary crossings may not work (with the high-byte of
// the value used as the high-byte of the address)
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::XAS>() -> void { S = A & X; MDR = (S & (((MAR - Y) >> 8) + 1)); }
template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::XAA>() -> void { A = loadMemory((A | 0xee) & X & MDR); }

template<> auto MOS6502::algorithm<MOS6502::AlgorithmCode::KIL>() -> void { --PC; }

auto MOS6502::addMemoryWithCarry(uint8 in) -> uint8 {
  int16_t out;

  if (!BCD || !D) {
    out = A + in + C;
    V = bool(~(A ^ in) & (A ^ out) & 0x80);
  } else {
    idleRead();
    out = static_cast<int16_t>((A & 0x0f) + (in & 0x0f) + (C << 0));
    if (out > 0x09) out += 0x06;
    C = out > 0x0f;
    out = static_cast<int16_t>((A & 0xf0) + (in & 0xf0) + (C << 4) + (out & 0x0f));
    if (out > 0x9f) out += 0x60;
  }

  C = bit<8>(out);
  Z = (out & 0xff) == 0;
  N = bit<7>(out);
  return out & 0xff;
}
auto MOS6502::branchOnStatusFlag(bool take, uint8 in) -> void {
  if (take) {
    // @test-file: branch_delays_irq
    // "a taken non-page-crossing branch ignores IRQ/NMI during
    // its last clock, so that next instruction executes before the IRQ"
    if (IRQ && !PIRQ) IRQ = false;

    auto displacement = static_cast<int8_t>(in);
    idlePageCrossed(PC, PC + displacement, false);
    idleRead();
    PC = PC + displacement;
  }
}
auto MOS6502::bitMemory(uint8 in) -> void {
  uint8 out = A & in;
  Z = out == 0;
  V = bit<6>(in);
  N = bit<7>(in);
}
auto MOS6502::compare(uint8 reg, uint8 in) -> uint8 {
  int out = reg - in;
  C = !bit<8>(out);
  Z = uint8(out) == 0;
  N = bit<7>(out);
  return out;
}
auto MOS6502::loadMemory(uint8 in) -> uint8 {
  Z = in == 0;
  N = bit<7>(in);
  return in;
}
auto MOS6502::shiftLeft(uint8 in) -> uint8 {
  uint8 out;
  C = bit<7>(in);
  out = in << 1;
  Z = out == 0;
  N = bit<7>(out);
  return out;
}
auto MOS6502::shiftRight(uint8 in) -> uint8 {
  uint8 out;
  C = bit<0>(in);
  out = in >> 1;
  Z = out == 0;
  N = 0;
  return out;
}
auto MOS6502::rotateLeft(uint8 in) -> uint8 {
  uint8 c = C;
  C = bit<7>(in);
  in = in << 1 | c;
  Z = in == 0;
  N = bit<7>(in);
  return in;
}
auto MOS6502::rotateRight(uint8 in) -> uint8 {
  bool c = C;
  C = bit<0>(in);
  in = c << 7 | in >> 1;
  Z = in == 0;
  N = bit<7>(in);
  return in;
}
auto MOS6502::subtract(uint8 in) -> uint8 {
  in = ~in;
  int16_t out;
  if (!BCD || !D) {
    out = A + in + C;
    V = bool(~(A ^ in) & (A ^ out) & 0x80);
  } else {
    idleRead();
    out = static_cast<int16_t>((A & 0x0f) + (in & 0x0f) + (C << 0));
    if (out <= 0x0f) out -= 0x06;
    C = out > 0x0f;
    out = static_cast<int16_t>((A & 0xf0) + (in & 0xf0) + (C << 4) + (out & 0x0f));
    if (out <= 0xff) out -= 0x60;
  }
  C = bit<8>(out);
  Z = uint8(out) == 0;
  N = bit<7>(out);
  return out & 0xff;
}

}


#endif//MOS6502_ALGORITHM_IPP_
