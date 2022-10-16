#pragma once

#ifndef SERIALIZATION_IPP_
#define SERIALIZATION_IPP_

namespace sen {

auto MOS6502::serialize(serializer &s) -> void {
  s.integer(BCD);
  s.integer(A);
  s.integer(X);
  s.integer(Y);
  s.integer(S);
  s.integer(PC);
  s.integer(P.byte);
  s.integer(MDR);
  s.integer(MAR);

  s.integer(PNMI);
  s.integer(NMI);
  s.integer(PIRQ);
  s.integer(IRQ);
  s.integer(PNMIL);
  s.integer(NMIL);
  s.integer(IRQL);
}

}

#endif//SERIALIZATION_IPP_
