#include "owninitshader2.h"
#include <cstdlib>
using namespace std;

void rotate(GLuint locate);

GLuint program;

GLfloat pit, yaw;
GLfloat scale = 1;
glm::vec3 translate;

GLuint vbo[2];
GLuint vao;
GLuint ebo;

GLfloat size=10;

GLfloat verticies[]={size,size,-size,     //0
                     size,-size,-size,    //1
                     -size,-size,-size,   //2
                     -size,size,-size,    //3
                     size,size,size,      //4
                     size,-size,size,     //5
                     -size,-size,size,    //6
                     -size,size,size,     //7
                     0,((7*size)/4),0     //8
                    };

GLfloat colors[]={1.0f,1.0f,0.0f,1.0f,          //0
                  0.816f,0.125f,0.565f,1.0f,    //1
                  1.0f,1.0f,0.0f,1.0f,          //2
                  0.816f,0.125f,0.565f,1.0f,    //3
                  0.816f,0.125f,0.565f,1.0f,    //4
                  1.0f,1.0f,0.0f,1.0f,          //5
                  0.816f,0.125f,0.565f,1.0f,    //6
                  1.0f,1.0f,0.0f,1.0f,          //7
                  1.0f,0.0f,0.0f,1.0f           //8
};

GLubyte elemsq[]={
        //sqback
        0,1,2,3,
        //sqfront
        7,4,5,6,
        //sqtop
        7,3,0,4,
        //sqbot
        5,6,2,1,
        //sqright
        5,4,0,1,
        //sqleft
        2,6,7,3,
        //pyramid
        8,0,8,4,
        8,7,8,3
};

void init(){
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, 600, 600);

    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);

    glGenBuffers(2, vbo);

// verticies
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(verticies),verticies,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
// colors
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(colors),colors,GL_STATIC_DRAW);
    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,0,(void*)0);

    glGenBuffers(1, &ebo);
// elements for house
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(elemsq),elemsq,GL_STATIC_DRAW);

    ShaderInfo shaders[]={
        { GL_VERTEX_SHADER , "vertexshader.glsl"},
        { GL_FRAGMENT_SHADER , "fragmentshader.glsl"},
        { GL_NONE , NULL}
    };

    program = initShaders(shaders);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
}


void display(SDL_Window* screen){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glm::mat4 trans;
    trans=glm::translate(trans, translate);//translate the house
    trans=glm::rotate(trans, pit, glm::vec3(1,0,0));//rotate the house around the x axis
    trans=glm::rotate(trans, yaw, glm::vec3(0,1,0));//rotate the house arround the y axis
    trans=glm::scale(trans, glm::vec3(scale));//scaling the house

    GLint tempLoc = glGetUniformLocation(program, "modelMatrix");//Matrix that handle the transformations
    glUniformMatrix4fv(tempLoc, 1, GL_FALSE, &trans[0][0]);
    glDrawElements(GL_POLYGON,32,GL_UNSIGNED_BYTE,NULL);

    glFlush();
    SDL_GL_SwapWindow(screen);
}

void input(SDL_Window* screen){
    SDL_Event event;
    while (SDL_PollEvent(&event)){//keyboard events
         switch (event.type){
             case SDL_QUIT:exit(0);break;
             case SDL_KEYDOWN:
             switch(event.key.keysym.sym){
                 case SDLK_ESCAPE:exit(0);
                 case SDLK_w:translate.y+=2;break;
                 case SDLK_s:translate.y-=2;break;
                 case SDLK_a:translate.x-=2;break;
                 case SDLK_d:translate.x+=2;break;
                 case SDLK_e:scale *= 0.9f;break;
                 case SDLK_q:scale *= 1.1f;break;
                 case SDLK_i:pit+=2;break;
                 case SDLK_k:pit-=2;break;
                 case SDLK_j:yaw+=2;break;
                 case SDLK_l:yaw-=2;break;
             }
         }
    }
}


int main(int argc, char **argv){

    //SDL window
    SDL_Window *window;
    if(SDL_Init(SDL_INIT_VIDEO)<0){//initilizes the SDL video subsystem
        fprintf(stderr,"Unable to create window: %s\n", SDL_GetError());
        SDL_Quit();
        exit(1);//error checking
    }
    //make a windo using SDL
    window = SDL_CreateWindow(
        "Sunset House", //Window title
        SDL_WINDOWPOS_UNDEFINED, //x position
        SDL_WINDOWPOS_UNDEFINED, //y position
        600,	//width, in pixels
        600,	//height, in pixels
        SDL_WINDOW_OPENGL	//# of flags
    );
    //error checking
    if(window==NULL){
        fprintf(stderr,"Unable to create window: %s\n",SDL_GetError());
    }
    //creates opengl context associated with the window
    SDL_GLContext glcontext=SDL_GL_CreateContext(window);
    
    glewExperimental=GL_TRUE;
    if(glewInit()){
        fprintf(stderr, "Unable to initalize GLEW");
        exit(EXIT_FAILURE);
    }
    init();
    while(true){
        input(window);//keyboard controls
        display(window);
    }
    SDL_GL_DeleteContext(glcontext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
