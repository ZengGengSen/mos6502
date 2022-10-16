#pragma once

#ifndef TIMING_IPP_
#define TIMING_IPP_

namespace sen {

auto MOS6502::tick() -> void {
  // "it's really the status of the interrupt lines at the end of the
  // second-to-last cycle that matters." Keep the irq lines values from
  // the previous cycle.  The before-to-last cycle's values will be used
  PIRQ = IRQ;

  // "The internal signal goes high during φ1 of the cycle that follows
  // the one where the edge is detected, and stays high until the NMI
  // has been handled. "
  PNMI = NMI;

  // "level detector"
  IRQ = IRQL && !I;

  // "This edge detector polls the status of the NMI line during φ2
  // of each CPU cycle (i.e., during the second half of each cycle)
  // and raises an internal signal if the input goes from being high
  // during one cycle to being low during the next"
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

}

#endif//TIMING_IPP_
