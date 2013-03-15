//
//  Sprite.cpp
//  GLSFMLEngine
//
//  Created by Evan Chapman on 3/10/13.
//  Copyright (c) 2013 Evan Chapman. All rights reserved.
//

#include "Sprite.h"

/*
 ===================
 Constructor
 ===================
 */
Sprite::Sprite() {
    //Just used for testing
    isLooping = true;
    isJumping = false;
    isAnimated = true;
    isStopped = false;
    isStopping = true;
    animationDelay = .50f;
    SetAnimation(stoppedr);
    acceleration.x = 0;
    acceleration.y = 0;
    friction.x  = 0.0f;
    friction.y = 0.0f ;
    
    maxVelocity.x = 0.0f;
    maxVelocity.y = 0.0f;
    
    minVelocity.x = -0.0f;
    minVelocity.y = -0.0f;
    
    
    R=G=B= 1.5f;
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
    
    glTexCoord2f( 0, 0 );//Bottom Left
    glVertex3f( position.x, position.y, position.z );
    glTexCoord2f( 1, 0 );//Bottom Right
    glVertex3f( position.x + size.x, position.y, position.z );
    glTexCoord2f( 1, 1 );//Top Right
    glVertex3f( position.x + size.x, position.y - size.y,position.z );
    glTexCoord2f( 0, 1 );//Top left
    glVertex3f( position.x, position.y - size.y,position.z );
    glEnd();
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
    animation.index[animationIndex].push_back(textureID);
}



/*
 ===============
 AddAnimation
 Add chunk of animation frames
 ===============
 */
void Sprite::AddAnimation(vector<int> newAnimation) {
    animation.index.push_back( newAnimation );
}

/*
 Remove animation
 Remove chunk of animation frames
 */
void Sprite::RemoveAnimation(int indexToRemove) {
    animation.index.erase(animation.index.begin() + indexToRemove );
}

/*
 Set Animation
 Set animation to index
 */
void Sprite::SetAnimation( int index ) {
    currentAnimation = index;
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
    
    unsigned long lastFrame = animation.index[currentAnimation].size() -1;
    
    //if the current frame is outside the last frame 
    if (currentFrameIndex > lastFrame) {
        if ( isLooping ) {
            currentFrameIndex = 0;
        } else {
            currentFrameIndex = lastFrame ;
        }
    }
    //Current frame of animation is index selected in current animation
    currentFrame = animation.index[currentAnimation][currentFrameIndex];
}

void Sprite::StopAnimation() {
    isAnimated = false;
    currentFrameIndex = 0;
    Animate(0);
}

//Dimensions
//-----------------------------
void Sprite::SetPosition(float x, float y, float z) {
    position.x = x;
    position.y = y;
    position.z = z;
}


void Sprite::SetSize(float x, float y, float z) {
    size.x = x;
    size.y = y;
    size.z = z;
}




//Updates + Physics
//--------------------------

void Sprite::Update(float deltaTime) {
    velocity.x += acceleration.x;
    velocity.y += acceleration.y;
    
    if (velocity.x !=0 && velocity.y != 0)
    {
        isStopped = false;
       // std::cout << "HELLO";
    } else {
        isStopped = true;
    }
    //std::cout << isStopped;
    
    if ( isJumping ) {
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
    
    if ( isStopping ) {
        if ( velocity.x > 0 ) {
        velocity.x -= friction.x;
            //isStopped = false;
            if ( velocity.x <= 0 ) {
                velocity.x = 0;
               // isStopped = true;
                StopAnimation();
            }
        } else if ( velocity.x < 0 ) {
           velocity.x += friction.x;
            //isStopped = false;
            if ( velocity.x >= 0 ) {
                velocity.x = 0;
                //isStopped = true;
                StopAnimation();
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
    
    //turning animation
    if ( direction == rightd && velocity.x < 0 ) {
        SetAnimation(turningr);
    }
    if ( direction == leftd && velocity.x > 0 ) {
        SetAnimation(turningl);
    }

    
    //Floor max and min vector
    if ( velocity.x > maxVelocity.x ) {
        velocity.x = maxVelocity.x;
    }
    
    if ( velocity.x < minVelocity.x ) {
        velocity.x = minVelocity.x;
    }
    
    
    //Final Movement Vector
    position.x += (velocity.x) * deltaTime;
    position.y += (velocity.y) * deltaTime;
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

vector2d_t Sprite::GetVelocity() {
    return velocity;
}


















