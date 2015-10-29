//
//  composer.h
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/28/15.
//
//

#ifndef __NIMP_Prototipe__composer__
#define __NIMP_Prototipe__composer__

#include <stdio.h>
#include "ofxComposer.h"
#include "enumerations.h"

class composer : public ofxComposer
{
    
public:
    
    composer();
    
    void setLinkType (nodeLinkType type);
    
};

#endif /* defined(__NIMP_Prototipe__composer__) */
