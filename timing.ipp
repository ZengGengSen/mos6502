#pragma once

#ifndef TIMING_IPP_
#define TIMING_IPP_

namespace sen {

auto MOS6502::tick() -> void {
  PIRQ = IRQ;
  PNMI = NMI;

  IRQ = IRQL && !I;

  if (NMIL && !PNMIL)
    NMI  = true;
  PNMIL = NMIL;
}

auto MOS6502::nmi(uint16 &vector) -> void {
  if (NMI) {
    NMI = false;
    vector = 0xfffa;
  }
}

void MOS6502::cancelNmi() { PNMI = false; }
void MOS6502::delayIrq() { if (IRQ && !PIRQ) IRQ = false; }


}

#endif//TIMING_IPP_
