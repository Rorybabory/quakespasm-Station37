#include "quakedef.h"

unsigned int post_framebuffer;
unsigned int light_framebuffer;
unsigned int textureColorbuffer;
unsigned int lightbuffer;
unsigned int rbo;
unsigned int lightrbo;
unsigned int quadVAO, quadVBO;

unsigned int screenshader;

unsigned int depthshader;

unsigned int depthTexture;


unsigned int screenLoc;
unsigned int lightLoc;
unsigned int depthLoc;

void Post_Init() {
    extern vrect_t scr_vrect;

    float quadVertices[] = { 
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    GLchar* vertSource = "";
    GLchar* fragSource = "";
    unsigned int path_id;
    fragSource = (GLchar*)COM_LoadHunkFile("shaders/post.fs", &path_id);
    vertSource = (GLchar*)COM_LoadHunkFile("shaders/post.vs", &path_id);

	GL_GenFramebuffersFunc(1, &post_framebuffer);
	GL_BindFramebufferFunc(GL_FRAMEBUFFER, post_framebuffer);


    glGenTextures(1, &textureColorbuffer);
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scr_vrect.width, scr_vrect.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GL_FramebufferTexture2DFunc(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

    GL_GenRenderbuffersFunc(1, &rbo);
    GL_BindRenderbufferFunc(GL_RENDERBUFFER, rbo);
    GL_RenderbufferStorageFunc(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, scr_vrect.width, scr_vrect.height);
    GL_FramebufferRenderbufferFunc(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if (GL_CheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        printf("FRAMEBUFFER CHECK FAILED\n");
    else
        printf("FRAMEBUFFER CHECK SUCCESSFUL\n");

    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, scr_vrect.width, scr_vrect.height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    GL_FramebufferTextureFunc(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

    GL_GenVertexArraysFunc(1, &quadVAO);
    GL_GenBuffersFunc(1, &quadVBO);
    GL_BindVertexArrayFunc(quadVAO);
    GL_BindBufferFunc(GL_ARRAY_BUFFER, quadVBO);
    GL_BufferDataFunc(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    GL_EnableVertexAttribArrayFunc(0);
    GL_VertexAttribPointerFunc(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    GL_EnableVertexAttribArrayFunc(1);
    GL_VertexAttribPointerFunc(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    screenshader = GL_CreateProgram(vertSource, fragSource, 0, NULL);

    depthshader = GL_CreateProgram((GLchar*)COM_LoadHunkFile("shaders/depth.vs", &path_id), (GLchar*)COM_LoadHunkFile("shaders/depth.fs", &path_id), 0, NULL);

    screenLoc = GL_GetUniformLocationFunc(screenshader, "screenTexture");
    depthLoc = GL_GetUniformLocationFunc(screenshader, "depthTexture");
    lightLoc = GL_GetUniformLocationFunc(screenshader, "lightTexture");


    GL_BindFramebufferFunc(GL_FRAMEBUFFER, 0);

    GL_UseProgramFunc(screenshader);
    GL_Uniform1iFunc(screenLoc, 0);
    GL_Uniform1iFunc(depthLoc, 1);
    GL_Uniform1iFunc(lightLoc, 2);
    
    GL_GenFramebuffersFunc(1, &light_framebuffer);
    GL_BindFramebufferFunc(GL_FRAMEBUFFER, light_framebuffer);

    glGenTextures(1, &lightbuffer);
    glBindTexture(GL_TEXTURE_2D, lightbuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, scr_vrect.width, scr_vrect.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GL_FramebufferTexture2DFunc(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, lightbuffer, 0);

    GL_GenRenderbuffersFunc(1, &lightrbo);
    GL_BindRenderbufferFunc(GL_RENDERBUFFER, lightrbo);
    GL_RenderbufferStorageFunc(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, scr_vrect.width, scr_vrect.height);
    GL_FramebufferRenderbufferFunc(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, lightrbo);

    GL_BindFramebufferFunc(GL_FRAMEBUFFER, 0);

}

void Post_Bind() {
    GL_BindFramebufferFunc(GL_FRAMEBUFFER, post_framebuffer);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Post_BindLight() {
    GL_BindFramebufferFunc(GL_FRAMEBUFFER, light_framebuffer);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Post_BindDepth() {
    GL_UseProgramFunc(depthshader);
}
void Post_Render() {

    GL_BindFramebufferFunc(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);

    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessary actually, since we won't be able to see behind the quad anyways)
    //glClear(GL_COLOR_BUFFER_BIT);

    GL_UseProgramFunc(screenshader);

    glEnable(GL_TEXTURE_2D);
    GL_ActiveTextureFunc(GL_TEXTURE0 + 0); // Texture unit 0
    glBindTexture(GL_TEXTURE_2D, textureColorbuffer);

    glEnable(GL_TEXTURE_2D);
    GL_ActiveTextureFunc(GL_TEXTURE1); // Texture unit 1
    glBindTexture(GL_TEXTURE_2D, depthTexture);

    glEnable(GL_TEXTURE_2D);
    GL_ActiveTextureFunc(GL_TEXTURE2); // Texture unit 2
    glBindTexture(GL_TEXTURE_2D, lightbuffer);



    GL_BindVertexArrayFunc(quadVAO);



    glDrawArrays(GL_TRIANGLES, 0, 6);
    glEnable(GL_DEPTH_TEST);
    GL_UseProgramFunc(0);
    GL_BindVertexArrayFunc(0);
    GL_ActiveTextureFunc(GL_TEXTURE0);


 }