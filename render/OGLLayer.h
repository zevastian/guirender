#include"OGLCommands.h"
#include"OGLSender.h"
#include<memory>
#include<condition_variable>
#include<mutex>

#ifndef _OGLLAYER_H_
#define _OGLLAYER_H_

class OGLLayer
{
private:
    /*opengl message sender*/
    std::shared_ptr<OGLSender> mOglSender;
    /*id layer*/
    unsigned int mId;
    /*sync thread*/
    std::shared_ptr<std::condition_variable> mConVarSync;
    std::shared_ptr<std::mutex> mMutexSync;
    bool mWaitSync;

public:
    /*init layer*/
    OGLLayer(unsigned int id, std::shared_ptr<OGLSender> oglSender);
    /*destroy layer*/
    ~OGLLayer();

    /*init commands packeg*/
    void beginCommands();
    /*push commands packege*/
    void endCommands();

    /*opengl async commands*/
    void glBegin(GLenum mode);
    void glEnd();
    void glBindTexture(GLenum target, GLuint texture);
    void glTexParameteri(GLenum target, GLenum pname, GLint param);
    void glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
    void glEnable(GLenum cap);
    void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
    void glTexCoord2f(GLfloat s, GLfloat t);
    void glVertex2f(GLfloat x, GLfloat y);
    void glDeleteTextures(GLsizei n, const GLuint *textures);
    void glColor3f(GLfloat red, GLfloat green, GLfloat blue);
    void glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
    void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
    void glClear(GLbitfield mask);
    void glMatrixMode(GLenum mode);
    void glLoadIdentity();
    void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val);

    /*opengl sync commands*/
    void glGenTextures(GLsizei n, GLuint *textures);
    void glGetError(GLenum &error);
};

#endif
