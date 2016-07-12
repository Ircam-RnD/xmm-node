#ifndef _HHMM_TOOL_H_
#define _HHMM_TOOL_H_

#include <cstdlib>
#include <iostream>

#include "XmmToolBase.h"

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
};

#endif /* _HHMM_TOOL_H_ */
