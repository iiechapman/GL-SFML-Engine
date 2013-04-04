//
//  Sprite.cpp
//  GLSFMLEngine
//
//  Created by Evan Chapman on 3/10/13.
//  Copyright (c) 2013 Evan Chapman. All rights reserved.
//

#include <cmath>
#include "Sprite.h"

/*
 ===================
 Constructor
 ===================
 */
Sprite::Sprite() {
    //Just used for testing
    isAnimated = false;
    isColliding = false;
    isFalling = false;
    isJumping = false;
    isTurning = false;
    isStopped = false;
    isStopping = false;
    isHurt = false;
    isDying = false;
    stoppedJumping = false;
    isLooping = false;
    isBoundary = false;
    isDead = false;
    isScrolling = false;
    scrollFactor = 1.0f;
    onGround = false;
    
    numberOfAnimations = 0;
    animationDelay = 0;
    animationDelayTotal = 0;
    SetAnimation(0);
    currentAnimation = 0;
    currentFrame = 0;
    currentFrameIndex = 0;
    
    topCollision = false;
    bottomCollision = false;
    rightCollision = false;
    leftCollision = false;
    
    topShift = false;
    bottomShift = false;
    leftShift = false;
    rightShift = false;
    
    acceleration.x = 0;
    acceleration.y = 0;
    friction.x  = 0.0f;
    friction.y = 0.0f ;
    maxFriction.x = 0.0f;
    maxFriction.y = 0.0f;
    
    jumpStrength.x = 0.0f;
    jumpStrength.y = 0.0f;
    
    maxVelocity.x = 0.0f;
    maxVelocity.y = 0.0f;
    
    minVelocity.x = -0.0f;
    minVelocity.y = -0.0f;
    
    maxGravity.x = 0.0f;
    maxGravity.y = 0.0f;
    
    jumpStrength.x = 0.0f;
    jumpStrength.x = 0.0f;
    
    velocity.x = 0;
    velocity.y = 0;
    
    direction = leftd;
    R=G=B= 1.5f;
    CalculateSides();
}


Sprite::~Sprite() {
    std::cout << "Deleted " << name << endl;
}


/*
 ========================
 Draw()
    Draw current frame of animation to quad area
 ========================
 */
void Sprite::Draw() {
    //Select Current Frame of animation texture
    glBindTexture( GL_TEXTURE_2D, currentFrame );
    
    //Begin drawing area
    glBegin( GL_QUADS );
    glColor3f( R, G, B );
    
    glTexCoord2f( 0, 0 );//top Left
    glVertex3f( position.x, position.y, position.z );
    glTexCoord2f( 1, 0 );//top Right
    glVertex3f( position.x + size.x, position.y, position.z );
    glTexCoord2f( 1, 1 );//bottom Right
    glVertex3f( position.x + size.x, position.y - size.y,position.z );
    glTexCoord2f( 0, 1 );//bottom left
    glVertex3f( position.x, position.y - size.y,position.z );
    glEnd();
}

void Sprite::SetColor(float red, float green, float blue ){
    R = red;
    G = green;
    B = blue;
}


//Animation
//-----------------------------------------

/*
 ====================
 AddFrame
 Add texture to new frame of animation
 ====================
 */
void Sprite::AddFrame(const std::string& filename, int animationIndex ) {
    int textureID = texture.Load(filename);
    if ( textureID == 0 ) {
        textureID = 1;
    }
    animIndex[animationIndex].push_back(textureID);
}



/*
 ===============
 AddAnimation
 Add chunk of animation frames
 ===============
 */
void Sprite::AddAnimation(vector<int> newAnimation) {
    animIndex.push_back( newAnimation );
    numberOfAnimations++;
}

/*
 Remove animation
 Remove chunk of animation frames
 */
void Sprite::RemoveAnimation(int indexToRemove) {
    animIndex.erase(animIndex.begin() + indexToRemove );
    numberOfAnimations--;
}

/*
 Set Animation
 Set animation to index
 */
void Sprite::SetAnimation( int index ) {
    if ( index <= numberOfAnimations) {
    currentAnimation = index;
    } else {
        currentAnimation =  0;
    }
}

/*
 ================
 Set animation delay
 Set the delay between frame changes
 ================
 */
void Sprite::SetAnimationDelay( float delay ) {
    animationDelay = delay;
}

/*
 ==========================
 Animate
 Advance frame, if frame is beyond boundaries
 of current animation, loop animation if animation is looped
 ==========================
 */
void Sprite::Animate(float deltaTime) {
    animationDelayTotal += deltaTime;
    
    if ( animationDelayTotal >= animationDelay && isAnimated ) {
        animationDelayTotal = 0;
        currentFrameIndex++;
    }
    
    unsigned long lastFrame = animIndex[currentAnimation].size() -1;
    
    //if the current frame is outside the last frame 
    if (currentFrameIndex > lastFrame) {
        if ( isLooping ) {
            currentFrameIndex = 0;
        } else {
            currentFrameIndex = lastFrame ;
        }
    }
    //Current frame of animation is index selected in current animation
    currentFrame = animIndex[currentAnimation][currentFrameIndex];
}

void Sprite::StopAnimation() {
    isAnimated = false;
    currentFrameIndex = 0;
    Animate(0);
}

void Sprite::SetLooping( bool flag ){
    isLooping = flag;
}


//Dimensions
//-----------------------------
void Sprite::SetPosition(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;
    CalculateSides();
}


void Sprite::SetSize(float x, float y, float z) {
    size.x = x;
    size.y = y;
    size.z = z;
    CalculateSides();
}

//Updates + Physics
//--------------------------

void Sprite::Update(float deltaTime) {
    velocity.x += (acceleration.x * deltaTime);
    velocity.y += (acceleration.y * deltaTime);
    
    //if jumping, apply gravity
    if ( ((isJumping || isFalling))  ) {
        if ( velocity.y < 0 ) {
            isJumping = false;
            isFalling = true;
            stoppedJumping = false;
            
            if (gravity.y <= maxGravity.y) {
                gravity.y = maxGravity.y;
            }
        }
        velocity.y += (gravity.y * deltaTime);
    }
    
    if ( isStopping ) {
        if ( velocity.x > 0 ) {
            velocity.x -= (friction.x * deltaTime);
            if ( velocity.x <= 0 ) {
                velocity.x = 0;
                isStopping = false;
                if (!isJumping && !isFalling){
                    isStopped = true;
                    stoppedJumping = true;
                }
            }
        } else if ( velocity.x < 0 ) {
           velocity.x += (friction.x * deltaTime);
            if ( velocity.x >= 0 ) {
                velocity.x = 0;
                isStopping = false;
                if (!isJumping && !isFalling){
                    isStopped = true;
                    stoppedJumping = true;
                }
            }
        }
    }
    
    //stopped animation
    if (isStopped){
        switch (direction) {
            case leftd:
                SetAnimation(stoppedl);
                break;
                
            case rightd:
                SetAnimation(stoppedr);
                
            default:
                break;
        }
    }
    
    //running left and right
    if ( velocity.x != 0  && (!isJumping && !isFalling)) {
        switch (direction) {
            case leftd:
                SetAnimation(runningl);
                break;
                
            case rightd:
                SetAnimation(runningr);
                break;
                
            default:
                break;
        }
    }
    
    //Floor max and min vector
    if ( velocity.x > maxVelocity.x ) {
        velocity.x = maxVelocity.x;
    }
    
    if ( velocity.x < minVelocity.x ) {
        velocity.x = minVelocity.x;
    }
    
    if ( velocity.y > maxVelocity.y ) {
        velocity.y = maxVelocity.y;
    }
    
    if ( velocity.y < minVelocity.y ) {
        velocity.y = minVelocity.y;
    }
    
    if (isColliding && (velocity.x == 0 || velocity.y == 0)) {
        isColliding = false;
    }
    
    if ( (isJumping || isFalling) && !onGround) {
        switch (direction) {
            case leftd:
                SetAnimation(jumpingl);
                break;
                
            case rightd:
                SetAnimation(jumpingr);
                break;
                
            default:
                break;
        }
    }
    
    //turning animation
    if ( (direction == rightd) && (velocity.x < -.1 ) && velocity.y == 0 && onGround && !isJumping && !isFalling) {
        isTurning = true;
        SetAnimation(turningr);
    } else if ( (direction == leftd )  && (velocity.x > .1) && velocity.y == 0  && onGround  && !isJumping && !isFalling) {
        isTurning = true;
        SetAnimation(turningl);
    } else {
        isTurning = false;
    }

    
    //if is dead overide all animation
    if (isDead) {
        switch (direction) {
            case leftd:
                SetAnimation(dyingl);
                break;
                
            case rightd:
                SetAnimation(dyingr);
                break;
                
            default:
                break;
        }
    }
    
    if ( velocity.x != 0 || velocity.y !=0) {
        isStopped = false;
    }
    
    if ( velocity.x == 0 && velocity.y == 0 && (!isJumping && !isFalling)) {
        isStopped = true;
        stoppedJumping = true;
    }
    
    //Final Movement Vector
    position.x += (velocity.x) * deltaTime;
    position.y += (velocity.y) * deltaTime;
    CalculateSides();
}

void Sprite::Jump() {
        isJumping = true;
        isFalling = false;
        stoppedJumping = false;
        velocity.y = jumpStrength.y;
        onGround = false;
        position.y += .1;
}

void Sprite::Fall() {
    if (!isJumping && !onGround){
        isFalling = true;
    }
}

void Sprite::StopFall() {
    isFalling = false;
    isJumping = false;
    onGround = true;
    velocity.y = 0;
}

void Sprite::SavePosition() {
    lastPosition = position;
}

void Sprite::SetSavePosition(dimensions_t position) {
    lastPosition = position;
}

void Sprite::Dead() {
    isDead = true;
    velocity.x = 0;
    velocity.y = jumpStrength.y;
}

void Sprite::Reset() {
    isDead = false;
    position = lastPosition;
    velocity.x = 0;
    velocity.y = 0;
    gravity.x = 0;
    gravity.y = 0;
}

//Setters
//------------------------------------------------------
void Sprite::SetVelocity(vector2d_t newVelocity) {
    velocity = newVelocity;
}

void Sprite::SetMaxVelocity(vector2d_t newMaxVelocity) {
    maxVelocity = newMaxVelocity;
}

void Sprite::SetMinVelocity(vector2d_t newMinVelocity) {
    minVelocity = newMinVelocity;
}

void Sprite::SetAcceleration(vector2d_t newAcceleration) {
    acceleration = newAcceleration;
}

void Sprite::SetFriction(vector2d_t newFriction) {
    friction = newFriction;
}

void Sprite::SetGravity(vector2d_t newGravity ) {
    gravity = newGravity;
}

void Sprite::SetMaxGravity( vector2d_t newMaxGravity ) {
    maxGravity = newMaxGravity;
}

void Sprite::SetJumpStrength(vector2d_t newJumpStrength) {
    jumpStrength = newJumpStrength;
}

//Getters
vector2d_t Sprite::GetVelocity() {
    return velocity;
}

vector2d_t Sprite::GetGravity() {
    return gravity;
}

vector2d_t Sprite::GetMaxGravity() {
    return maxGravity;
}

vector2d_t Sprite::GetMaxVelocity() {
    return maxVelocity;
}

vector2d_t Sprite::GetMinVelocity() {
    return minVelocity;
}

vector2d_t Sprite::GetJumpStrength() {
    return jumpStrength;
}

dimensions_t Sprite::GetPosition() {
    return position;
}

dimensions_t Sprite::GetSize() {
    return size;
}

dimensions_t Sprite::GetOverlapSize() {
    return overlapSize;
}

dimensions_t Sprite::GetOverlapPos() {
    return overlapPos;
}

bool Sprite::Turning() {
    return isTurning;
}

void Sprite::IsBoundary(bool flag) {
    isBoundary = flag;
}

bool Sprite::IsBoundary() {
    return isBoundary;
}

bool Sprite::Colliding(Sprite& rhs) {
    isColliding = false;
    if ( rhs.IsBoundary() && !isDead) {
        if (   right >= rhs.left
            &&  left <= rhs.right
            &&  top >= rhs.bottom
            &&  bottom <= rhs.top ) {
            
            isColliding = true;
            overlapPos = rhs.GetPosition();
            overlapSize = rhs.GetSize();
            
            float penTop,penBottom,penRight,penLeft;
            penTop      = rhs.top - bottom;
            penBottom   = top - rhs.bottom;
            penLeft     = right - rhs.left;
            penRight    = rhs.right - left;
            
            topShift    = (penTop < penBottom) && (penTop < penLeft) && (penTop < penRight);
            bottomShift = (penBottom < penTop) && (penBottom < penLeft) && (penBottom < penRight);
            leftShift   = (penLeft < penTop) && (penLeft < penBottom) && (penLeft < penRight);
            rightShift  = (penRight < penTop) && (penRight < penBottom) && (penRight < penLeft);
            
            if (bottomShift ) {
                position.y = rhs.bottom;
                bottomCollision = true;
                topCollision = rightCollision = leftCollision = false;
            }
            
            if (topShift ) {
                position.y = rhs.top + size.y;
                topCollision = true;
                onGround = true;
                bottomCollision = rightCollision = leftCollision = false;
            }
            
            if (leftShift) {
                position.x = rhs.left - size.x;
                leftCollision = true;
                topCollision = bottomCollision = rightCollision = false;
            }
        
            if (rightShift) {
                position.x = rhs.right;
                rightCollision = true;
                topCollision = bottomCollision = leftCollision = false;
            }
        } 
    }
    return isColliding;
}

void Sprite::CalculateSides(){
    top     = position.y;
    bottom  = top - size.y;
    left    = position.x;
    right   = left + size.x;
}











