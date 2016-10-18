/**
 * This file is not used for now
 * Might be useful for future architecture improvements
 */
#ifndef _HHMMTOOL_H_
#define _HHMMTOOL_H_

#include <cstdlib>
#include <iostream>
#include "XmmTool.h"

class HhmmTool : public XmmTool<xmm::HierarchicalHMM> {
    
public:
  HhmmTool() {};
  ~HhmmTool() {};
    
  void setNbStates(std::size_t nbStates) {
    if(nbStates > 0) {
      model.configuration.states.set(nbStates, 1);
      model.configuration.changed = true;
    }
  }

  // etc ...
};

#endif /* _HHMMTOOL_H_ */
