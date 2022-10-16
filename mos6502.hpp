#pragma once
#ifndef MOS6502_HPP_
#define MOS6502_HPP_

#include "utils/object.hpp"
#include "utils/reg-type.hpp"
#include "utils/types.hpp"

namespace sen {

struct MOS6502 : public Object {
  union Flags {
    uint8 byte;
    EmbedBitRange<0> c;// carry
    EmbedBitRange<1> z;// zero
    EmbedBitRange<2> i;// interrupt disable
    EmbedBitRange<3> d;// decimal mode
    EmbedBitRange<4> b;// break
    EmbedBitRange<5> u;// unused
    EmbedBitRange<6> v;// overflow
    EmbedBitRange<7> n;// negative

    inline operator uint8() const { return byte; }// NOLINT(google-explicit-constructor)
    inline auto operator=(uint8 data) -> Flags & {
      byte &= 0b00110000;
      data &= 0b11001111;
      byte |= data;
      return *this;
    }
  };
  struct Register {
    uint8 a, x, y, s;
    reg16 pc;
    Flags p;

    uint8 mdr;
    reg16 mar;
  };

  struct Signal {
    bool prev_nmi_pending{false};
    bool prev_irq_pending{false};

    bool nmi_pending{false};
    bool irq_pending{false};

    // todo: add the halt_pending
  };

  struct Line {
    bool prev_nmi_line;
    bool nmi_line;
    bool irq_line;

    // todo: add the rdy line
  };

  virtual auto read(uint16) -> uint8 = 0;
  virtual auto write(uint16, uint8) -> void = 0;

  // @file: mos6502.cpp
  [[nodiscard]] auto mdr() const -> uint8;
  auto power() -> void;
  auto interrupt() -> void;

  // @file: timing.cpp
  auto tick() -> void;
  auto nmi(uint16 &vector) -> void;

  // @file: serialization.ipp
  auto serialize(serializer &serializer) -> void override;

  // @file: memory.icc
  auto opcode() -> uint8;
  auto load(uint8 addr) -> uint8;
  auto push(uint8 data) -> void;
  auto pull() -> uint8;

  auto idleRead() -> void;
  auto idleZeroPage(uint8 addr) -> void;
  auto idlePageCrossed(uint16 x, uint16 y, bool isWrite) -> void;
  auto idleStackPointer() -> void;

  // @file: addressing.icc
  using Addressing = auto(MOS6502::*)() -> uint16;
  enum class AddressingCode {
    Absolute,
    AbsoluteXRead,
    AbsoluteXWrite,
    AbsoluteYRead,
    AbsoluteYWrite,
    Accumulator,
    Immediate,
    Implied,
    Indirect,
    IndirectX,
    IndirectYRead,
    IndirectYWrite,
    Relative,
    ZeroPage,
    ZeroPageX,
    ZeroPageY,
  };

  auto absoluteIndexed(uint8 index, bool is_write) -> uint16;
  auto indirectYIndexed(bool is_write) -> uint16;
  // default addressing is nothing to do
  template<AddressingCode mode>
  auto addressing() -> uint16 { return 0; }

  // @file: algorithm.icc
  using Algorithm = auto(MOS6502::*)() -> void;
  enum class AlgorithmCode {
    // official algorithm code
    ADC,
    AND,
    ASL_A,
    ASL_M,
    BCC,
    BCS,
    BEQ,
    BIT,
    BMI,
    BNE,
    BPL,
    BRK,
    BVC,
    BVS,
    CLC,
    CLD,
    CLI,
    CLV,
    CMP,
    CPX,
    CPY,
    DEC,
    DEX,
    DEY,
    EOR,
    INC,
    INX,
    INY,
    JMP,
    JSR,
    LDA,
    LDX,
    LDY,
    LSR_A,
    LSR_M,
    NOP,
    ORA,
    PHA,
    PHP,
    PLA,
    PLP,
    ROL_A,
    ROL_M,
    ROR_A,
    ROR_M,
    RTI,
    RTS,
    SBC,
    SEC,
    SED,
    SEI,
    STA,
    STX,
    STY,
    TAX,
    TAY,
    TSX,
    TXA,
    TXS,
    TYA,
    // unofficial algorithm code
    AAC,
    AXA,
    AAX,
    ARR,
    ASR,
    ATX,
    AXS,
    DCP,
    ISB,
    LAR,
    LAX,
    RLA,
    RRA,
    SLO,
    SRE,
    SXA,
    SYA,
    XAS,
    XAA,
    KIL,
    // Instructions effecting in 'no operations' in various address modes. Operands are ignored.
    DOP = NOP,
    TOP = NOP,
    USBC = SBC,
  };
  template<AlgorithmCode code>
  auto algorithm() -> void {}
  auto addMemoryWithCarry(uint8 in) -> uint8;
  auto branchOnStatusFlag(bool take, uint8 in) -> void;
  auto bitMemory(uint8 in) -> void;
  auto compare(uint8 reg, uint8 in) -> uint8;
  auto loadMemory(uint8) -> uint8;
  auto shiftLeft(uint8) -> uint8;
  auto shiftRight(uint8) -> uint8;
  auto rotateLeft(uint8) -> uint8;
  auto rotateRight(uint8) -> uint8;
  auto subtract(uint8 in) -> uint8;

  // @file: instruction.icc
  enum class InstructionCode { Load,
                               Write,
                               Modify,
                               Jump };
  using Instruction = auto(MOS6502::*)(Addressing, Algorithm) -> void;
  template<InstructionCode code>
  auto instruction(Addressing, Algorithm) -> void {}
  auto runInstruction() -> void;

 protected:
  bool BCD = false;
  Register register_;
  Signal signal_;
  Line line_;
};

}// namespace sen

#endif//MOS6502_HPP_
