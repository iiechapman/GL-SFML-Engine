//
//  Sprite.h
//  GLSFMLEngine
//
//  Created by Evan Chapman on 3/10/13.
//  Copyright (c) 2013 Evan Chapman. All rights reserved.
//

#ifndef __GLSFMLEngine__Sprite__
#define __GLSFMLEngine__Sprite__

#include <iostream>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics.hpp>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include "TextureHandler.h"

#endif /* defined(__GLSFMLEngine__Sprite__) */


class Sprite {
    public:
    
    void Update();
    void Animate();
    void Draw();
    void SetPosition ( float x,float y );
    
    private:
};