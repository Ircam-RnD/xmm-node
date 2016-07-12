#ifndef _GMM_TOOL_H_
#define _GMM_TOOL_H_

#include <cstdlib>
#include <iostream>

#include "XmmToolBase.h"

class GmmTool : public XmmTool<xmm::GMM> {
    
public:
    GmmTool() {};
    ~GmmTool() {};
};

#endif /* _GMM_TOOL_H_ */