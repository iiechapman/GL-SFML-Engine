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


typedef enum {
    stoppedl,
    stoppedr,
    runningl,
    runningr,
    jumpingl,
    jumpingr,
    fallingl,
    fallingr,
    turningl,
    turningr,
    hurtingl,
    hurtingr,
    dyingl,
    dyingr,
    swimmingl,
    swimmingr,
    climbingl,
    climbingr,
} actions_t;

class Sprite {
    public:
    
    Sprite();
    
    bool isAnimated;
    bool isFalling;
    bool isJumping;
    bool isTurning;
    bool isStopped;
    bool isStopping;
    bool isHurt;
    bool isDying;
    
    
    direction_t direction;
    
    void Update(float deltaTime);
    
    void SetVelocity(vector2d_t newVelocity);
    void SetMaxVelocity(vector2d_t newMaxVelocity);
    void SetMinVelocity(vector2d_t newMinVelocity);
    vector2d_t GetVelocity();
    void SetGravity(vector2d_t newGravity);
    void SetFriction(vector2d_t newFriction);
    void SetAcceleration(vector2d_t newAcceleration);
    
    void AddFrame(const std::string& filename , int animationIndex);
    void AddAnimation(vector<int> newAnimation);
    void SetAnimation(int index);
    void SetAnimationDelay(float delay);
    void StopAnimation();
    void RemoveAnimation(int indexToRemove);
    void Animate(float deltaTime);
    
    void Draw();
    void SetColor(float red, float green, float blue);
    void SetPosition ( float x,float y ,float z );
    void SetSize (float x, float y, float z);
    
    private:
    animation_t animation;
    unsigned long currentFrame;
    unsigned long currentFrameIndex;
    int currentAnimation;
    bool isLooping;
    float animationDelay;
    float animationDelayTotal;
    
    float R,G,B;
    
    //Physics
    vector2d_t velocity;
    vector2d_t maxVelocity;
    vector2d_t minVelocity;
    
    vector2d_t acceleration;
    
    vector2d_t gravity;
    vector2d_t maxGravity;
    
    vector2d_t friction;
    vector2d_t maxFriction;
    
    dimensions_t position;
    dimensions_t size;
    
    TextureHandler texture;
    
};

#endif /* defined(__GLSFMLEngine__Sprite__) */
















