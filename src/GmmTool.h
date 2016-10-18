/**
 * This file is not used for now
 * Might be useful for future architecture improvements
 */
#ifndef _GMMTOOL_H_
#define _GMMTOOL_H_

#include <cstdlib>
#include <iostream>
#include "XmmTool.h"

class GmmTool : public XmmTool<xmm::GMM> {
    
public:
  GmmTool() {};
  ~GmmTool() {};
};

#endif /* _GMM_TOOL_H_ */