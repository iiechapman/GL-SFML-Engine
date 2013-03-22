#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundSource.hpp>
#include <OpenGL/OpenGL.h>
#include <GLUT/GLUT.h>
#include <iostream>
#include <cmath>

#include "TextureHandler.h"
#include "GameTypes.h"
#include "Sprite.h"
#include "KeyCodes.h"
#include "Input.h"


/*
 * Basic OpenGL 2d mario engine
 * Evan Chapman 2013
 */

//Debug header
vector<Sprite*> sprite;
Sprite* marioSprite;
Sprite* testSprite;
Sprite* blockSprite;

vector2d_t tempVec;
vector2d_t secondVec;


GLuint totalTextures;

const int MAX_FPS = 60;
bool PLAYMUSIC = false;
bool DEPTH_ENABLED = false;
bool FULLSCREEN = false;
string windowTitle = "Super Mario Open - By Evan Chapman";
string title = "Super Mario Open - By Evan Chapman";
bool running;
bool falling;
bool jumping;
bool stopped;

dimensions_t    position;
dimensions_t    deltaPosition;
dimensions_t    size;

//tile variables
dimensions_t tilePos;
dimensions_t tileSize;

//animation variables
float marioAnimTime     = 0.0f;
float animDelayTotal    = 1.00f;
float animTick          = 0.068f;
float animDelta         = 0.0f;
float animLastTime      = 0.0f;

direction_t marioDirection;
range_t     marioAnim;

GLuint displayList;

//time variables
float   delta       = .0025f;
float   lastTime    =   0.0f;
float   curTime     =   0.0f;
float   deltaTime   =   0.0f;
float   timeOfLastDraw    = 0.0f;
float   deltaDraw         = 0.0f;

int     FPS;
int     framesPerSecond;
int     totalFramesRendered;

range_t leftAnim;
range_t jumpright;
range_t jumpleft;
range_t rightAnim;
range_t stoppedright;
range_t stoppedleft;


sf::Music titleSong;
sf::Sound soundPlayer;
sf::Sound skid;

sf::SoundBuffer hurtSound;
sf::SoundBuffer startSound;
sf::SoundBuffer jumpSound;
sf::SoundBuffer skidSound;

void Init();
void Display();
void Reshape( int w,int h );
void Keyboard( unsigned char key,int x, int y );
void KeyboardUp( unsigned char key, int x, int y );
void SpecialKey( int key, int x ,int y );
void SpecialKeyUp( int key,int x ,int y );
void SpriteState();
void SetScene();
void Animate();
void CalculateFPS();
void Timer();
void Idle();
void Debug();
void CalculateDrawTime();
void Quit();

int main ( int argc,char ** argv ) {
    Init();
    soundPlayer.setBuffer(startSound);
    skid.setBuffer(skidSound);
    
    //Create opengl context
    std::cout << "Initializing Opengl...\n";
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition( 200, 200 );
    glutInitWindowSize( 1024, 768 );
    glutGameModeString("1024x768:32");
    soundPlayer.play();
    if ( !FULLSCREEN ) {
        
        glutCreateWindow( windowTitle.c_str() );
    } else {
        glutCreateWindow( windowTitle.c_str() );
        glutFullScreen();
    }
    glClearColor( 0.0f, 0.37f, 0.71f, 0.0f );
    glutDisplayFunc( Display );
    glutReshapeFunc( Reshape );
    glutIdleFunc( Idle );
    glutKeyboardFunc( Keyboard );
    glutKeyboardUpFunc( KeyboardUp );
    glutSpecialFunc( SpecialKey );
    glutSpecialUpFunc( SpecialKeyUp );
    glutJoystickFunc(Joystick,1);
    //Created opengl Context
    
    //Play music
    if (PLAYMUSIC){
        titleSong.setVolume(50.0f);
        titleSong.play();
    }
    //Load jump sound
    soundPlayer.setBuffer(jumpSound);
    
    Debug();

    displayList = glGenLists( 1 );
    
    glutMainLoop();
    
    for (vector<Sprite*>::iterator i = sprite.begin() ; i < sprite.end() ; i++ ) {
        delete (*i);
        (*i) = 0;
    }
    
    testSprite = 0;
    marioSprite = 0;
    
	return EXIT_SUCCESS;
}

void Idle() {
    glutForceJoystickFunc();
    
    //mario position
    //std::cout << "y : " << sprite[1]->GetPosition().y << " y size: " << sprite[1]->GetSize().y << endl;
    
    //if escaped
    if ( pressed.escape ) {
        if ( FULLSCREEN ) {
            glutLeaveGameMode();
        }
        soundPlayer.setBuffer(hurtSound);
        soundPlayer.play();
        titleSong.stop();
        Quit();
        exit(0);
        std::cout << "Exit \n";
    }
    
    if ( pressed.f && held.f != true ) {
        if (FULLSCREEN){
            glutReshapeWindow(1024, 768 );
            glutPositionWindow(600, 300 );
            FULLSCREEN = false;
        } else {
            FULLSCREEN = true;
            glutFullScreen();
        }
    
    }
    //press left or right
    if ( pressed.kleft || pressed.jleft ) {
        tempVec.x = -0.009f;
        sprite[1]->direction = leftd;
        sprite[1]->isAnimated = true;
        sprite[1]->isStopping = false;
        
    } else if( pressed.kright || pressed.jright) {
        tempVec.x = 0.009f;
        sprite[1]->direction = rightd;
        sprite[1]->isAnimated = true;
        sprite[1]->isStopping = false;
    } else {
        tempVec.x = 0.0f;
        sprite[1]->isStopping = true;
    }
    
    //Press run button
    if (pressed.shift || held.jbutton1 || pressed.z) {
        secondVec.x = 1.8f;
        sprite[1]->SetMaxVelocity(secondVec);
        secondVec.x = -1.8f;
        sprite[1]->SetMinVelocity(secondVec);
        //sprite[1]->SetAnimationDelay(.02f);
    } else {
        secondVec.x = 1.4f;
        sprite[1]->SetMaxVelocity(secondVec);
        secondVec.x = -1.4f;
        sprite[1]->SetMinVelocity(secondVec);
        //sprite[1]->SetAnimationDelay(.05f);
    }
    
    //Press Jump button make mario jump
    if ( (pressed.x || pressed.jbutton2)
        && !sprite[1]->isFalling
        && !sprite[1]->isJumping
        ) {
        //std::cout << "jumpin woah there...\n";
        soundPlayer.play();
        sprite[1]->Jump();
    } else {
        tempVec.y = 0.0f;
    }
    
    //change jump height based on length of jump button hold
    vector2d_t  tempGravity;
    if ( pressed.x|| held.jbutton2 ) {
        tempGravity.y = -2.4f;
        sprite[1]->SetMaxGravity(tempGravity);
        tempGravity.y = -0.008f;
        sprite[1]->SetGravity(tempGravity);
    } else {
        tempGravity.y = -2.4f;
        sprite[1]->SetMaxGravity(tempGravity);
        tempGravity.y = -0.015f;
        sprite[1]->SetGravity(tempGravity);
    }
    
    //std::cout << "Gravy : " << sprite[1]->GetGravity().y << "\n" ;
    
   // std::cout << "jump: " << sprite[1]->isJumping << " \n";
    
    //check collisions
    dimensions_t collision = sprite[1]->GetPosition();
     
    dimensions_t shift ;
   // dimensions_t final;
    
    shift = sprite[2]->GetPosition();
    
    sprite[1]->Colliding(*sprite[2]);
    /*
    std::cout << "top: "
    << sprite[1]->topCollision
    << " bottom: " << sprite[1]->bottomCollision
    << " left: " << sprite[1]->leftCollision
    << " right: " << sprite[1]->rightCollision
    << " collis: " << sprite[1]->isColliding << endl;
    */
    
    if (sprite[1]->isColliding == false)
    {
        
    }
    
    if (sprite[1]->isColliding){
        if (sprite[1]->bottomCollision) {
            sprite[1]->SetPosition(sprite[1]->GetPosition().x, sprite[2]->GetPosition().y - sprite[2]->GetSize().y , 0);
            
            sprite[1]->isJumping = false;
            sprite[1]->isFalling = true;
            tempVec.x = sprite[1]->GetVelocity().x;
            tempVec.y = sprite[1]->GetMaxGravity().y/2;
            sprite[1]->SetVelocity(tempVec);

        }
        
        if (sprite[1]->topCollision) {
            sprite[1]->SetPosition(sprite[1]->GetPosition().x, sprite[2]->GetPosition().y + sprite[1]->GetSize().y, 0);

            sprite[1]->isJumping = false;
            sprite[1]->isFalling = false;
            tempVec.x = sprite[1]->GetVelocity().x;
            tempVec.y = 0;
            sprite[1]->SetVelocity(tempVec);

        }
        
        if (sprite[1]->rightCollision) {
            sprite[1]->SetPosition(sprite[2]->GetPosition().x + sprite[2]->GetSize().x, sprite[1]->GetPosition().y, 0);
            tempVec.y = 0;

        }
        
        
        if (sprite[1]->leftCollision) {
            sprite[1]->SetPosition(sprite[2]->GetPosition().x - sprite[1]->GetSize().x , sprite[1]->GetPosition().y, 0);
            tempVec.y = 0;
        }
        
    }
    
    if (sprite[1]->isColliding == false) {
        //sprite[1]->isFalling = true;
    }
    
    if ( collision.y < -0.62f ) {
        sprite[1]->isColliding = true;
        sprite[1]->isJumping = false;
        sprite[1]->isFalling = false;
        tempVec.x = sprite[1]->GetVelocity().x;
        tempVec.y = 0;
        sprite[1]->SetVelocity(tempVec);
        sprite[1]->SetPosition(collision.x, -0.62f, collision.z);
    } else if (collision.y >= -0.62f ){
        sprite[1]->isColliding = false;
    }
    
    //std::cout << "x: " << collision.x << " y: " << collision.y << " \n";

    //end collisions--------------------
    
    
    if ( sprite[1]->Turning() == true ) {
        if ( skid.getStatus() == 0 ) {
            skid.play();
        }
    }
    

    sprite[1]->SetAcceleration( tempVec );
    //std::cout << 1.5 - abs(sprite[1]->GetVelocity().x) << endl;
    sprite[1]->SetAnimationDelay(1.5 - abs(sprite[1]->GetVelocity().x));

    
    //Rotate camera using WASD and parentheses
    if ( pressed.rbracket ) {
        glRotatef(1, 0.0f,0.0f, delta/1000);
    }
    
    if ( pressed.lbracket ) {
        glRotatef(1, 0.0f,0.0f, -delta/1000);
    }
    
    if ( pressed.w ) {
        glRotatef(1, -delta/1000,0.0f, 0.0f);
    }
    
    if ( pressed.a ) {
        glRotatef(1, 0.0f,-delta/1000, 0.0f);
    }
    
    if ( pressed.s ) {
        glRotatef(1, delta/1000,0.0f, 0.0f);
    }
    
    if ( pressed.d ) {
        glRotatef(1, 0.0f,delta/1000, 0.0f);
    }
    
    for (vector<Sprite*>::iterator i = sprite.begin() ; i != sprite.end() ; ++i ){
        (*i)->Update(deltaDraw);
    }
    
    Animate();
    
    SetScene();
    
    CalculateFPS();
    glutPostRedisplay();
}

void Init(){
    // Load a song to play
    if ( !titleSong.openFromFile( "overworld.ogg" ) ) {
        std::cout << "Failed to load song...\n";
    }
    
    if ( !hurtSound.loadFromFile("hurt.wav")) {
        std::cout << "Failed to load sound ... \n";
    }
    
    if (!startSound.loadFromFile("start.wav")) {
        std::cout << "Failed to load sound... \n";
    }
    
    if (!jumpSound.loadFromFile("jump.wav")) {
        std::cout << "Could not load sound...\n";
    }
    
    if (!skidSound.loadFromFile("skid.wav")) {
        std::cout << "Could not load file \n";
    }
    
};


void Display() {
    timeOfLastDraw = glutGet( GLUT_ELAPSED_TIME ) * .001;
    
    glClear( GL_COLOR_BUFFER_BIT );

    glCallList( displayList );

    glutSwapBuffers();
    totalFramesRendered++;
    CalculateDrawTime();
    
    
}

void Reshape(int w, int h) {
    glViewport( 0, 0, (GLsizei)w, (GLsizei)h );
    glMatrixMode( GL_PROJECTION );
    glOrtho( 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 );
}

void SetScene() {
    glNewList( displayList, GL_COMPILE );
    
    //set line drawing width for axes
    glLineWidth( 1 );
    
    
    glColor3f( 0.1f, 0.2f, 1.0f );
    //glutWireTeapot( 0.5f );
    
    //TERRIBLE 60 frame cap code
    //glutWireSphere( 0.5f, 200000, 200000 );
    
    //start textured drawing
    glEnable( GL_TEXTURE_2D );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    
    //Start Debug Drawing
    for ( std::vector<Sprite*>::iterator i = sprite.begin() ; i != sprite.end() ; ++i ) {
        (*i)->Draw();
    }
    
    glDisable( GL_TEXTURE_2D );
    glDisable( GL_BLEND );

    if ( DEPTH_ENABLED ) {
        //Set line width for axes
        glLineWidth( 4 );
        glBegin( GL_LINES );
    
        //X Coordinate line
        glColor3f( 1.0f, 0.0f, 0.0f );
        glVertex3f( 0, 0, 0);
        glVertex3f( 0.8, 0, 0 );
        
        //Y Coordinate line
        glColor3f( 0.0f, 1.0f, 0.0f );
        glVertex3f( 0.0f, 0.0f, 0.0f );
        glVertex3f( 0.0f, 0.8, 0.0 );
        
        //Z Coordinate line
        glColor3f( 0.0f, 0.0f, 1.0f);
        glVertex3f( 0, 0, 0 );
        glVertex3f( 0, 0, 0.8 );
        
        glEnd();
    }
    
    glEndList();
}

void CalculateFPS() {
   // std::cout << "FPS: " << FPS << "\n";
    curTime = glutGet( GLUT_ELAPSED_TIME ) * .001;
    
    deltaTime = curTime - lastTime;
    
    if ( deltaTime >= 1.0f ) {
        lastTime = curTime;
        FPS = totalFramesRendered;
        totalFramesRendered = 0;
        char buffer[100];
        sprintf(buffer, "Super Mario Open - By Evan Chapman - FPS: %d", FPS);
        windowTitle = buffer;
        glutSetWindowTitle(windowTitle.c_str());
    }
}

void CalculateDrawTime() {
    deltaDraw = glutGet( GLUT_ELAPSED_TIME ) *.001 - timeOfLastDraw;
    //std::cout << "Last Draw: "<< timeOfLastDraw << " current time: " << glutGet( GLUT_ELAPSED_TIME ) *.001 <<" Delta: " << deltaDraw << "\n";
}

void Animate(){
    for (vector<Sprite*>::iterator i = sprite.begin() ; i != sprite.end() ; i++ ){
        (*i)->Animate(deltaDraw);
    }
}


void Quit() {
    delete marioSprite;
    marioSprite = 0;
    delete testSprite;
    testSprite = 0 ;
    /*
    for (vector<Sprite*>::iterator i = sprite.begin() ; i < sprite.end() ; i++ ) {
        delete (*i);
        (*i) = 0;
    }
    */
    sprite.clear();
    
}

void Debug() {
    std::cout << "Debug Section: " << "\n";
    
    vector<int> blankAnimation;

    testSprite = new Sprite();
    testSprite->name = "Background";
    testSprite->AddAnimation(blankAnimation);
    //testSprite->SetAnimation(0);
    testSprite->AddFrame("background1.png", 0);
    totalTextures++;
    testSprite->SetPosition(-1, 1, 0);
    testSprite->SetSize(2, 2, 0);
    sprite.push_back(testSprite);
    
    marioSprite = new Sprite();
    marioSprite->SetSize(.23, .25, 0);
    marioSprite->SetPosition(-0.9f, -0.62f, 0.0f);
    marioSprite->AddAnimation(blankAnimation);
    //marioSprite->SetAnimation(0);
    marioSprite->SetAnimationDelay(0.04f);
    
 
    for (int i = 0 ; i < 18 ; i++ ) {
        marioSprite->AddAnimation(blankAnimation);
    }
    
    marioSprite->name = "Mario";
    //running/stopped left animation
    marioSprite->AddFrame("marioleft.png", stoppedl);
    totalTextures++;
    marioSprite->AddFrame("marioleft.png", runningl);
    totalTextures++;
    marioSprite->AddFrame("marioleft2.png", runningl);
    totalTextures++;
    marioSprite->AddFrame("marioleft3.png", runningl);
    totalTextures++;
    
    //running stoppped right animation
    marioSprite->AddFrame("marioright.png", stoppedr);
    totalTextures++;
    
    marioSprite->AddFrame("marioright.png", runningr);
    totalTextures++;
    marioSprite->AddFrame("marioright2.png", runningr);
    totalTextures++;
    marioSprite->AddFrame("marioright3.png", runningr);
    totalTextures++;
    
    //jumping left animation
    marioSprite->AddFrame("mariojumpleft.png", jumpingl);
    totalTextures++;
    
    //jumping right animation
    marioSprite->AddFrame("mariojumpright.png", jumpingr);
    totalTextures++;
    
    //falling left animation
    marioSprite->AddFrame("mariojumpleft.png", fallingl);
    totalTextures++;
    
    //falling right animation
    marioSprite->AddFrame("mariojumpright.png", fallingr);
    totalTextures++;
    
    //turning left animation
    marioSprite->AddFrame("marioturnright.png", turningl);
    totalTextures++;
    
    //turning right animation
    marioSprite->AddFrame("marioturnleft.png", turningr);
    totalTextures++;
    //dying left animation
    marioSprite->AddFrame("mariodying.png", dyingl);
    totalTextures++;
    
    //dying right animation
    marioSprite->AddFrame("mariodying.png", dyingr);
    totalTextures++;
    marioSprite->SetLooping(true);
    
    vector2d_t tVec;
    
    tVec.x = 0.004f;
    marioSprite->SetFriction(tVec);
    
    tVec.x = 1.3f;
    marioSprite->SetMaxVelocity(tVec);
    
    tVec.x = -1.5f;
    marioSprite->SetMinVelocity(tVec);
    
    tVec.y = 3.5f;
    marioSprite->SetJumpStrength(tVec);
    
    //marioSprite->SetAnimation(1);
    marioSprite->StopAnimation();
    marioSprite->direction = rightd;
    marioSprite->SetAnimation(stoppedr);
    sprite.push_back(marioSprite);
    marioSprite = 0;
    
    blockSprite = new Sprite();
    blockSprite->name = "Block";
    blockSprite->AddAnimation(blankAnimation);
    //blockSprite->SetAnimation(0);
    blockSprite->SetAnimationDelay(0.10f);
    blockSprite->AddFrame("block1.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block2.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block3.png", 0);
    totalTextures++;
    blockSprite->AddFrame("block4.png", 0);
    totalTextures++;
    blockSprite->SetPosition(-.2, -.2, 0);
    blockSprite->SetSize(.2, .22, 0.0f);
    blockSprite->SetLooping(true);
    blockSprite->isAnimated = true;
    sprite.push_back(blockSprite);
    
    
    for (int i = 0 ; i < 10 ; i++) {
        blockSprite = new Sprite();
        char buffer[20]  ;
        sprintf(buffer, "tile %d",i);
        blockSprite->name = buffer;
        blockSprite->AddAnimation(blankAnimation);
        //blockSprite->SetAnimation(0);
        blockSprite->SetAnimationDelay(0.10f);
        blockSprite->AddFrame("tile1.png", 0);
        totalTextures++;
        blockSprite->SetPosition(-.8 + i *.1, -.85, 0);
        blockSprite->SetSize(.1, 0.1, 0.0f);
        sprite.push_back(blockSprite);
        }
    }















