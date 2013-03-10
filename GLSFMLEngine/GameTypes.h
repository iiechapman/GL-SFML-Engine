//
//  GameTypes.h
//  GLSFMLEngine
//
//  Created by Evan Chapman on 3/10/13.
//  Copyright (c) 2013 Evan Chapman. All rights reserved.
//

#ifndef __GLSFMLEngine__GameTypes__
#define __GLSFMLEngine__GameTypes__

#include <iostream>

#endif /* defined(__GLSFMLEngine__GameTypes__) */


struct range_t{
    int start;
    int middle;
    int end;
};

enum direction_t{
    left,right,up,down
};

struct dimensions_t{
    float x;
    float y;
    float z;
    float w;
};

struct buttons_t{
    bool up;
    bool down;
    bool right;
    bool left;
    bool plus;
    bool minus;
    bool space;
    bool rbracket;
    bool lbracket;
    bool w,a,s,d;
};