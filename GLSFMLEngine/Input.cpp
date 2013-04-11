//
//  Input.cpp
//  GLSFMLEngine
//
//  Created by Evan Chapman on 3/13/13.
//  Copyright (c) 2013 Evan Chapman. All rights reserved.
//

#include "Input.h"
#include "KeyCodes.h"
#include <iostream>
#include <GLUT/GLUT.h>

buttons_t pressed;
buttons_t held;
buttons_t released;


void Keyboard( unsigned char key,int x, int y ) {

    switch ( key ) {
        case kesc:
            if (pressed.escape){
                held.escape = true;
            } else {
                pressed.escape = true;
            }
            break;
            
        case kspace:
            pressed.space = true;
            break;
            
        case '=':
            pressed.plus = true;
            break;
            
        case '-':
            pressed.minus = true;
            break;
            
        case ']':
            pressed.rbracket = true;
            break;
            
        case '[':
            pressed.lbracket = true;
            break;
            
        case 'w':
            if (pressed.w){
                held.w = true;
                released.w = false;
            } else {
                pressed.w = true;
            }
            break;
            
        case 'a':
            if (pressed.a) {
                held.a = true;
                released.a = false;
            } else {
                pressed.a = true;
            }
            break;
            
        case 's':
            pressed.s = true;
            break;
            
        case 'd':
            if (pressed.d) {
                held.d = true;
            } else {
                pressed.d = true;
            }
            break;
            
        case 'z':
            if (pressed.z) {
                held.z = true;
            } else {
            pressed.z = true;
            }
            break;
            
        case 'x':
            if (pressed.x) {
                held.x = true;
                //std::cout << "Held x \n";
            } else {
                pressed.x = true;
                //std::cout << "Pressed x \n";
            }

            break;
            
        case 'f':
            if (pressed.f) {
                held.f = true;
            } else {
                pressed.f = true;
            }
            break;
            
        case 'r':
            if (pressed.r) {
                held.r = true;
            } else {
                pressed.r = true;
            }
            break;
            
        case 'm':
            if (pressed.m) {
                held.m = true;
            } else {
                pressed.m = true;
            }
            break;
            
        default:
            break;
    }
    if ( glutGetModifiers() == GLUT_ACTIVE_SHIFT ) {
        pressed.shift = true;
    }
}

void KeyboardUp( unsigned char key, int x, int y ) {
    switch ( key ) {
        case kesc:
            pressed.escape = false;
            break;
            
        case kspace:
            pressed.space = false;
            break;
            
        case '=':
            pressed.plus = false;
            break;
            
        case '-':
            pressed.minus = false;
            break;
            
        case ']':
            pressed.rbracket = false;
            break;
            
        case '[':
            pressed.lbracket = false;
            break;
            
        case 'w':
            pressed.w = false;
            held.w = false;
            released.w = true;
            break;
            
        case 'a':
            pressed.a = false;
            held.a = false;
            released.a = true;
            break;
            
        case 's':
            pressed.s = false;
            break;
            
        case 'd':
            pressed.d = false;
            held.d    = false;
            break;
            
        case 'z':
            pressed.z = false;
            held.z = false;
            break;
            
        case 'x':
            pressed.x = false;
            held.x = false;
            break;
            
        case 'f':
            pressed.f = false;
            held.f = false;
            break;
            
        case 'r':
            pressed.r = false;
            held.r = false;
            break;
            
        case 'm':
            pressed.m = false;
            held.m = false;
            break;
        
        default:
            break;
            
    }
}

void SpecialKey( int key, int x, int y ) {
    /*
     Special Key codes:
     Up     = 101
     Down   = 103
     Left   = 100
     Right  = 102
     */
    //std::cout << "Special Key: " << key << "\n";
    switch ( key ) {
        case kup:
            pressed.kup = true;
            break;
            
        case kdown:
            pressed.kdown = true;
            break;
            
        case kleft:
            pressed.kleft = true;
            break;
            
        case kright:
            pressed.kright = true;
            break;
            
        default:
            break;
    }
    
    if ( glutGetModifiers() == GLUT_ACTIVE_SHIFT ) {
        pressed.shift = true;
    } else {
        pressed.shift = false;
    }
}

void SpecialKeyUp( int key, int x, int y ) {
    switch ( key ) {
        case kup:
            pressed.kup = false;
            break;
            
        case kdown:
            pressed.kdown = false;
            break;
            
        case kleft:
            pressed.kleft = false;
            break;
            
        case kright:
            pressed.kright = false;
            break;
            
        default:
            break;
    }
    if ( glutGetModifiers() == GLUT_ACTIVE_SHIFT ) {
        pressed.shift = true;
    } else {
        pressed.shift = false;
    }
}

void Joystick(unsigned int buttonMask,int x, int y, int z) {    
   
    if ( x > 3 ) {
        pressed.jright = true;
        pressed.jleft = false;
    }
    
    if (x < 0 ) {
        pressed.jleft = true;
        pressed.jright = false;
    }
    
    if ( x == 3 ) {
        pressed.jright = false;
        pressed.jleft = false;
    }
    
    if ( y > 3 ) {
        pressed.jdown = true;
        pressed.jup = false;
    }
    
    if ( y < 0 ) {
        pressed.jup = true;
        pressed.jdown = false;
    }
    
    if ( y == 3 ) {
        pressed.jdown = false;
        pressed.jup = false;
    }
    
    if ( buttonMask == 8 ) {
        if (pressed.jbutton1) {
            held.jbutton1 = true;
        }else {
            pressed.jbutton1 = true;
        }
    } else {
        pressed.jbutton1 = false;
        held.jbutton1 = false;
    }
    
    if ( buttonMask == 1 ) {
        if (pressed.jbutton2) {
            held.jbutton2  = true;
        } else {
            pressed.jbutton2 = true;
        }
    } else {
        pressed.jbutton2 = false;
        held.jbutton2 = false;
    }
    
}
































