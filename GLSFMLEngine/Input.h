//
//  Input.h
//  GLSFMLEngine
//
//  Created by Evan Chapman on 3/13/13.
//  Copyright (c) 2013 Evan Chapman. All rights reserved.
//

#ifndef __GLSFMLEngine__Input__
#define __GLSFMLEngine__Input__

#include <iostream>
#include "GameTypes.h"

extern buttons_t pressed;
void Keyboard( unsigned char key,int x, int y );
void KeyboardUp( unsigned char key, int x, int y );
void SpecialKey( int key, int x, int y );
void SpecialKeyUp( int key, int x, int y );
void Joystick(unsigned int buttonMask,int x, int y, int z);



#endif /* defined(__GLSFMLEngine__Input__) */
