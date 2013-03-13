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
#include "GameTypes.h"
#include <cstdlib>

using namespace std;

typedef struct animation_s{
    vector<vector<int>> index;
} animation_t ;

class Sprite {
    public:
    
    Sprite();
    
    void Update();
    void AddAnimation(vector<int> newAnimation);
    void RemoveAnimation(int indexToRemove);
    void Animate();
    void Draw();
    void SetPosition ( float x,float y );
    
    private:
    animation_t animation;
    int currentFrame;
    int currentFrameIndex;
    int currentAnimation;
    bool isLooping;
    float animationDelay;
    float R,G,B;
    
    dimensions_t position;
    dimensions_t size;
    
    TextureHandler texture;
    
};

#endif /* defined(__GLSFMLEngine__Sprite__) */