#ifndef GLPLATFORM_H_INCLUDED
#define GLPLATFORM_H_INCLUDED

#if defined(__ANDROID__)
  #include <GLES3/gl3.h>
  #include <GLES3/gl3ext.h>
#elif defined(__APPLE__)
  #include <TargetConditionals.h>
  #if TARGET_OS_IPHONE
    #include <OpenGLES/ES3/gl.h>
    #include <OpenGLES/ES3/glext.h>
  #else
    #include <glad/glad.h>
  #endif
#else
  #include <glad/glad.h>
#endif

#endif // GLPLATFORM_H_INCLUDED