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

typedef struct range_s {
    int start;
    int middle;
    int end;
} range_t;

typedef enum {
    leftd,rightd,upd,downd
} direction_t;

typedef struct dimensions_s{
    float x;
    float y;
    float z;
} dimensions_t;

typedef struct buttons_s{
    bool kup;
    bool kdown;
    bool kright;
    bool kleft;
    bool jup;
    bool jdown;
    bool jleft;
    bool jright;
    bool jbutton1;
    bool jbutton2;
    bool plus;
    bool minus;
    bool space;
    bool rbracket;
    bool lbracket;
    bool escape;
    bool shift;
    bool cntrl;
    bool a,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z;
} buttons_t;

typedef struct vector2d_s {
    float x;
    float y;
} vector2d_t;

#endif /* defined(__GLSFMLEngine__GameTypes__) */
