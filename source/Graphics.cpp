#include "../include/Graphics.h"

namespace GGE
{
    Graphics* Graphics::instance = 0;

	Graphics::Graphics()
	{
	    uiObjects.clear();
	    texts.clear();
	    sprites.clear();
	    animationsPaused = false;
	    vbo = ebo = 0;
	}

	void Graphics::destroy()
	{

		glFinish();

        if (vbo) glDeleteBuffers(1, &vbo);
        if (ebo) glDeleteBuffers(1, &ebo);

	    delete instance;

        instance = NULL;
	}

	void Graphics::initGraphics()
	{
	     setupGL();
	     OS::getInstance()->resizeWindow();

        float fixedVertices[12] = {
             0.5f,  0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        };

        unsigned int fixedIndexes[6] = {
            0, 1, 3,
            1, 2, 3
        };

        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

//        glBindVertexArray(0);
        glBindVertexArray(vao);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(fixedVertices), fixedVertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(fixedIndexes), fixedIndexes, GL_STATIC_DRAW);

	}

	void Graphics::renderResize(Point windowSize)
	{

        viewportSize = windowSize;
//        viewportWidth = _width;
//        viewportHeight = _height;
        GraphicsUtils::calculateViewportSize(viewportSize);
//        glMatrixMode(GL_PROJECTION);
//        glLoadIdentity();
        projectionMatrix = glm::perspective(45.f, FIXED_ASPECT_RATIO, 0.1f, 100.0f);
//        glMatrixMode(GL_MODELVIEW);
//        glLoadIdentity();
        viewportPosition.x = windowSize.x / 2 - viewportSize.x/2;
        viewportPosition.y = windowSize.y /2 - viewportSize.y/2;
        glViewport(viewportPosition.x, viewportPosition.y, (GLsizei)viewportSize.x, (GLsizei)viewportSize.y);

//        viewportSize = windowSize;
//        GraphicsUtils::calculateViewportSize(viewportSize);
//
//        viewportPosition.x = windowSize.x / 2 - viewportSize.x/2;
//        viewportPosition.y = windowSize.y /2 - viewportSize.y/2;
//        glViewport(viewportPosition.x, viewportPosition.y, (GLsizei)viewportSize.x, (GLsizei)viewportSize.y);
//        glEnable(GL_SCISSOR_TEST);
//        glScissor(viewportPosition.x, viewportPosition.y, (GLsizei)viewportSize.x, (GLsizei)viewportSize.y);
//        projectionMatrix = glm::ortho((float)- viewportSize.x/2, (float)viewportSize.x/2, (float)- viewportSize.y/2, (float)viewportSize.y/2, -1.0f, 1.0f);
//        viewMatrix = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));


        viewMatrix = glm::lookAt(
								glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                    );
	}

	glm::mat4 Graphics::getProjectionMatrix()
	{
	    return projectionMatrix;
	}

    glm::mat4 Graphics::getViewMatrix()
	{
	    return viewMatrix;
	}

	void Graphics::onRenderFinish()
	{
		glFlush();
	}
	void Graphics::onSceneFinish()
	{
	}

	void Graphics::setupGL()
	{
        glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

	void Graphics::doGraphics(float deltaTime)
	{
        onRender3D(deltaTime);
        onRender2D(deltaTime);
        onRenderUI();
        onRenderFinish();
	}

	void Graphics::onRender3D(float deltaTime)
    {
        std::map<std::string, GraphicsObjectGL*>::iterator iter;

        projectionMatrix = glm::perspective(45.f, FIXED_ASPECT_RATIO, 0.1f, 100.0f);
        viewMatrix = glm::lookAt(
								glm::vec3(4,3,-3), // Camera is at (4,3,-3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                                    );

        // Render to our framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(viewportPosition.x, viewportPosition.y, (GLsizei)viewportSize.x, (GLsizei)viewportSize.y);
//		glViewport(0,0,width,height); // Render on the whole framebuffer, complete from the lower left corner to the upper right

		// We don't use bias in the shader, but instead we draw back faces,
		// which are already separated from the front faces by a small distance
		// (if your geometry is made this way)
//		glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
//        glClearDepth(1.0f);
        // Enable depth test
        glDisable(GL_DEPTH_TEST);

//        glEnable(GL_CULL_FACE);
//        glDepthMask(GL_TRUE);
        // Accept fragment if it closer to the camera than the former one
//        glDepthFunc(GL_LESS);

//        glCullFace(GL_BACK);
//glFrontFace(GL_CCW);
//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//        glDepthMask(GL_FALSE);
//        glDisable(GL_CULL_FACE);
//        glDepthMask(GL_FALSE);

        glUseProgram(0);

        for (iter = graphicsObjects.begin(); iter != graphicsObjects.end(); ++iter)
        {
            GraphicsObjectGL* go = (static_cast<GraphicsObjectGL*>(iter->second));
            if (go->isVisible()) {

                glUseProgram(go->getShader()->getShaderID());
                go->draw();

            }
        }

        glUseProgram(0);

        GLenum err;
        if ((err = glGetError()) != GL_NO_ERROR) {
            OS::getInstance()->alert("OpenGL ERR", to_string(err).c_str());
        }
    }

    void Graphics::onRender2D(float deltaTime)
    {
//        glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT);

//        glViewport(viewportPosition.x, viewportPosition.y, (GLsizei)viewportSize.x, (GLsizei)viewportSize.y);
        projectionMatrix = glm::ortho((float)- viewportSize.x/2, (float)viewportSize.x/2, (float)- viewportSize.y/2, (float)viewportSize.y/2, -1.0f, 1.0f);
//        glEnable(GL_SCISSOR_TEST);
//        glScissor(viewportPosition.x, viewportPosition.y, (GLsizei)viewportSize.x, (GLsizei)viewportSize.y);
        viewMatrix = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));


        GLenum err;


        std::map<std::string, Sprite*>::iterator iter;
        for (iter = sprites.begin(); iter != sprites.end(); ++iter)
        {
            Sprite *sprite = static_cast<Sprite*>(iter->second);
            if (sprite->isVisible()) {


                Drawable *drawable;
                if (!sprite->getCurrentAnimationName().empty())
                {
                    if (!animationsPaused) {
                        sprite->getCurrentAnimation()->update(deltaTime);
                    }
                    drawable = sprite->getCurrentAnimation()->getCurrentDrawable(sprite->getAnimationPlayMode());
                }
                else
                {
                    drawable = reinterpret_cast<Drawable*>(sprite);
                }

                glm::mat4 modelMatrix = glm::translate(glm::mat4(), glm::vec3((float) sprite->getX() * viewportSize.x / SCREEN_X,
                                                                              (float) sprite->getY() * viewportSize.y / SCREEN_Y,
                                                                              0))
                * glm::rotate((float)0.0, glm::vec3(0,0,1))
                * glm::scale(glm::vec3((float) (sprite->isFlipedX() ? -1 : 1) * sprite->getScaleX() * drawable->getAtlasRegion()->width * viewportSize.x / SCREEN_X,
                                       (float) (sprite->isFlipedY() ? -1 : 1) * sprite->getScaleY() * drawable->getAtlasRegion()->height * viewportSize.y / SCREEN_Y,
                                       0));
                glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

                glUseProgram(drawable->getShader()->getShaderID());
                GLuint mvpID = glGetUniformLocation(drawable->getShader()->getShaderID(), "MVP");
                glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

                if (drawable->getTextureAtlas()->textureID)
                {

                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, drawable->getTextureAtlas()->textureID);
                    unsigned int uniformID = glGetUniformLocation(drawable->getShader()->getShaderID(), "texture0");
                    glUniform1i(uniformID, 0);


                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                }

                glBindVertexArray(vao);
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glVertexAttribPointer(
                   0,
                   3,
                   GL_FLOAT,
                   GL_FALSE,
                   0,
                   (void*)0
                );


                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, drawable->getUVBuffer());
                glVertexAttribPointer(
                    1,
                    2,
                    GL_FLOAT,
                    GL_FALSE,
                    0,
                    (void*)0
                );

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);

                glUseProgram(0);

            }
        }


        std::map<std::string, Text*>::iterator iterText;
        for (iterText = texts.begin(); iterText != texts.end(); ++iterText)
        {
            Text *text = static_cast<Text*>(iterText->second);
            if (text->isVisible())
            {

                glm::mat4 modelMatrix = glm::translate(glm::mat4(1), glm::vec3(text->getPosition().x  * viewportSize.x / SCREEN_X,
                                                                               text->getPosition().y  * viewportSize.y / SCREEN_Y,
                                                                               0))
                * glm::rotate(glm::mat4(1), 0.0f, glm::vec3(0,0,1))
                * glm::scale(glm::mat4(1.0), glm::vec3(text->getFont()->getOriginalSize() * viewportSize.x / SCREEN_X,
                                                       text->getFont()->getOriginalSize() * viewportSize.y/SCREEN_Y,
                                                       0))
                * glm::scale(glm::mat4(1.0), glm::vec3(0.075f, 0.075f,0));
                glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

                glUseProgram(text->getShader()->getShaderID());
                GLuint mvpID = glGetUniformLocation(text->getShader()->getShaderID(), "MVP");
                glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

                text->printText();
                glUseProgram(0);
            }

        }

        if ((err = glGetError()) != GL_NO_ERROR) {
            OS::getInstance()->alert("OpenGL ERR", to_string(err).c_str());
        }

    }

    void Graphics::onRenderUI()
    {
        GLenum err;
//        glEnable(GL_DEPTH_TEST);

//        glViewport(viewportPosition.x, viewportPosition.y, (GLsizei)viewportSize.x, (GLsizei)viewportSize.y);
        projectionMatrix = glm::ortho((float)- viewportSize.x/2, (float)viewportSize.x/2, (float)- viewportSize.y/2, (float)viewportSize.y/2, -1.0f, 1.0f);
//        glEnable(GL_SCISSOR_TEST);
//        glScissor(viewportPosition.x, viewportPosition.y, (GLsizei)viewportSize.x, (GLsizei)viewportSize.y);
        viewMatrix = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));


            std::map<std::string, UIObject*>::iterator iter;
        for (iter = uiObjects.begin(); iter != uiObjects.end(); ++iter)
        {
            UIObject *uiObject= reinterpret_cast<UIObject*>(iter->second);
            Drawable *drawable = uiObject->getDrawable();
            if (drawable->isVisible()) {


                glm::mat4 modelMatrix = glm::translate(glm::mat4(1), glm::vec3((float) uiObject->getPosition()->x * viewportSize.x / SCREEN_X,
                                                                               (float) uiObject->getPosition()->y * viewportSize.y / SCREEN_Y,
                                                                               0))
                * glm::rotate(glm::mat4(1), (float)drawable->getRotation(), glm::vec3(0,0,1))
                * glm::scale(glm::mat4(1.0), glm::vec3((float) drawable->getScaleX() * drawable->getAtlasRegion()->width * viewportSize.x / SCREEN_X,
                                                       (float) drawable->getScaleY() * drawable->getAtlasRegion()->height * viewportSize.y / SCREEN_Y,
                                                       0));
                glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

                glUseProgram(drawable->getShader()->getShaderID());
                GLuint mvpID = glGetUniformLocation(drawable->getShader()->getShaderID(), "MVP");
                glUniformMatrix4fv(mvpID, 1, GL_FALSE, &mvp[0][0]);

                if (drawable->getTextureAtlas()->textureID)
                {

                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, drawable->getTextureAtlas()->textureID);
                    unsigned int uniformID = glGetUniformLocation(drawable->getShader()->getShaderID(), "texture0");
                    glUniform1i(uniformID, 0);

                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                }

                glBindVertexArray(vao);
                glEnableVertexAttribArray(0);
                glBindBuffer(GL_ARRAY_BUFFER, vbo);
                glVertexAttribPointer(
                   0,
                   3,
                   GL_FLOAT,
                   GL_FALSE,
                   0,
                   (void*)0
                );

                glEnableVertexAttribArray(1);
                glBindBuffer(GL_ARRAY_BUFFER, drawable->getUVBuffer());
                glVertexAttribPointer(
                    1,
                    2,
                    GL_FLOAT,
                    GL_FALSE,
                    0,
                    (void*)0
                );


                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);


                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

                glDisableVertexAttribArray(0);
                glDisableVertexAttribArray(1);

                glUseProgram(0);

            }
        }

        if ((err = glGetError()) != GL_NO_ERROR) {
            OS::getInstance()->alert("OpenGL ERR", to_string(err).c_str());
        }

    }

    void Graphics::addSprite(std::string objectName, Sprite *_object)
    {
         sprites.insert(std::make_pair(objectName, _object));
    }

    void Graphics::removeSprite(std::string objectName)
    {
        if (sprites.find(objectName) != sprites.end())
            sprites.erase(objectName);
    }

    void Graphics::addText(std::string objectName, Text *_text)
    {
        texts.insert(std::make_pair(objectName, _text));
    }

    void Graphics::removeText(std::string objectName)
    {
        if (texts.find(objectName) != texts.end())
            texts.erase(objectName);
    }

    void Graphics::addUIObject(std::string objectName, UIObject *_uiObject)
    {
        uiObjects.insert(std::make_pair(objectName, _uiObject));
    }

    void Graphics::removeUIObject(std::string objectName)
    {
        if (uiObjects.find(objectName) != uiObjects.end())
            uiObjects.erase(objectName);
    }

    void Graphics::removeGraphicsObject(std::string objectName)
    {

        if (graphicsObjects.find(objectName) != graphicsObjects.end()) {
            graphicsObjects.erase(objectName);
        }
    }

    void Graphics::addGraphicsObject(std::string objectName, GraphicsObjectGL *_object)
    {
        graphicsObjects.insert(std::make_pair(objectName, _object));
    }
}
