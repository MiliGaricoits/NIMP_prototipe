//
//  enumerations.h
//  NIMP_Prototipe
//
//  Created by Mili Garicoits on 10/28/15.
//
//

#ifndef NIMP_Prototipe_enumerations_h
#define NIMP_Prototipe_enumerations_h

#define MENU_HEIGHT 50
#define MENU_TOP_PADDING -17
#define RIGHT_MENU_WIDTH 35
#define RIGHT_MENU_LONG_WIDTH 250

#define COMPOSER_EVENT_PRIORITY 202
#define PATCH_EVENT_PRIORITY 203
#define SCROLL_BAR_EVENT_PRIORITY 201

#define SCALE_SENSITIVITY 0.001f
#define ZOOM_SENSITIVITY .001f
#define ZOOM_DIFF .5f
#define ZOOM_UNIT 1.f

enum nodeLinkType
{
    STRAIGHT_LINKS = 0,
    CURVE_LINKS = 1,
    PATH_LINKS = 2
};

#endif
