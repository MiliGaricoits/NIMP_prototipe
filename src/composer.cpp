//
//  composer.cpp
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/28/15.
//
//

#include "composer.h"

composer::composer() : ofxComposer() { }

void composer::setLinkType (nodeLinkType type) {
    
    for(map<int,ofxPatch*>::iterator it = this->patches.begin(); it != this->patches.end(); it++ ){
        it->second->setLinkType(type);
    }
}