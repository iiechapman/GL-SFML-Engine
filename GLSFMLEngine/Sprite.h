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
    ~Sprite();
    
    string name;
    
    bool isAnimated;
    bool isColliding;
    bool isFalling;
    bool isJumping;
    bool isTurning;
    bool isStopped;
    bool isStopping;
    bool isHurt;
    bool isDying;
    bool stoppedJumping;
    bool topCollision;
    bool bottomCollision;
    bool leftCollision;
    bool rightCollision;
    
    unsigned int numberOfAnimations;
    
    direction_t direction;
    
    void Update(float deltaTime);
    
    void Jump();
    
    void SetVelocity(vector2d_t newVelocity);
    void SetMaxVelocity(vector2d_t newMaxVelocity);
    void SetMinVelocity(vector2d_t newMinVelocity);
    void SetGravity(vector2d_t newGravity);
    void SetMaxGravity( vector2d_t newMaxGravity );
    void SetJumpStrength (vector2d_t newJumpStrength);
    void SetFriction(vector2d_t newFriction);
    void SetAcceleration(vector2d_t newAcceleration);

    
    vector2d_t GetVelocity();
    dimensions_t GetPosition();
    dimensions_t GetSize();
    dimensions_t GetOverlapPos();
    dimensions_t GetOverlapSize();
    vector2d_t  GetGravity();
    vector2d_t  GetMaxGravity();
    vector2d_t GetJumpStrength();
    vector2d_t GetMaxVelocity();
    vector2d_t GetMinVelocity();
    
    bool Turning();
    
    
    void AddFrame(const std::string& filename , int animationIndex);
    void AddAnimation(vector<int> newAnimation);
    void SetAnimation(int index);
    void SetAnimationDelay(float delay);
    void StopAnimation();
    void RemoveAnimation(int indexToRemove);
    void Animate(float deltaTime);
    void SetLooping( bool flag );
    
    void Draw();
    void SetColor(float red, float green, float blue);
    void SetPosition ( float x,float y ,float z );
    void SetSize (float x, float y, float z);
    
    void IsBoundary(bool flag);
    bool IsBoundary();
    bool Colliding(Sprite& rhs);
    
    private:
    vector<vector<int>> animIndex;
    GLuint currentFrame;
    unsigned long currentFrameIndex;
    int currentAnimation;
    bool isLooping;
    float animationDelay;
    float animationDelayTotal;
    
    bool isBoundary;
    float top,bottom,left,right;
    
    float R,G,B;
    
    //Physics
    vector2d_t velocity;
    vector2d_t maxVelocity;
    vector2d_t minVelocity;
    
    vector2d_t acceleration;
    
    vector2d_t gravity;
    vector2d_t maxGravity;
    vector2d_t jumpStrength;
    
    vector2d_t friction;
    vector2d_t maxFriction;
    
    dimensions_t position;
    dimensions_t size;
    dimensions_t overlapPos;
    dimensions_t overlapSize;
    
    TextureHandler texture;
    
    void CalculateSides();
    
};

#endif /* defined(__GLSFMLEngine__Sprite__) */
















