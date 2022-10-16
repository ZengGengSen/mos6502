#include "mos6502.hpp"

#define A register_.a
#define X register_.x
#define Y register_.y
#define S register_.s
#define P register_.p
#define PC register_.pc.w
#define PCH register_.pc.hi
#define PCL register_.pc.lo
#define MAR register_.mar.w
#define MDR register_.mdr
#define C register_.p.c
#define Z register_.p.z
#define I register_.p.i
#define D register_.p.d
#define B register_.p.b
#define U register_.p.u
#define V register_.p.v
#define N register_.p.n
#define ADDRESS (this->*addr)
#define ALGORITHM (this->*alg)

#define PNMI signal_.prev_nmi_pending
#define NMI signal_.nmi_pending
#define PIRQ signal_.prev_irq_pending
#define IRQ signal_.irq_pending
#define PNMIL line_.prev_nmi_line
#define NMIL line_.nmi_line
#define IRQL line_.irq_line

namespace sen {
auto MOS6502::power() -> void {
  A = 0x00;
  X = 0x00;
  Y = 0x00;
  S = 0xff;
  P = 0x04;
  MDR = 0x00;

  signal_ = {};
  line_ = {};
}

auto MOS6502::mdr() const -> uint8 {
  return MDR;
}

auto MOS6502::interrupt() -> void {
  uint16 vector = 0xfffe;
  idleRead();
  idleRead();

  push(PCH);
  push(PCL);
  nmi(vector);
  push(P | 0x20);// irq and nmi just set U to 1, brk also set B to 1
  I = 1;
  PCL = read(vector);
  PCH = read(vector + 1);
}
}// namespace sen

#include "addressing.ipp"
#include "algorithm.ipp"
#include "instruction.ipp"
#include "memory.ipp"
#include "serialization.ipp"
#include "timing.ipp"

#undef IRQL
#undef NMIL
#undef PNMIL
#undef IRQ
#undef PIRQ
#undef NMI
#undef PNMI

#undef ALGORITHM
#undef ADDRESS
#undef N
#undef V
#undef U
#undef B
#undef D
#undef I
#undef Z
#undef C
#undef MDR
#undef MAR
#undef PCL
#undef PCH
#undef PC
#undef P
#undef S
#undef Y
#undef X
#undef A