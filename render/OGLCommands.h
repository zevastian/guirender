#include<GL/glx.h>
#include<condition_variable>
#include<mutex>
#include<memory>

#ifndef _OGLCOMMANDS_H_
#define _OGLCOMMANDS_H_

/*opengl commands*/
enum GLMSG_ENUM {
    GLMSG_NONE,
    GLMSG_INITCOMMANDS,
    GLMSG_FINISHCOMMANDS,
    GLMSG_VIEWPORT,
    GLMSG_BEGIN,
    GLMSG_END,
    GLMSG_BINDTEXTURE,
    GLMSG_TEXPARAMETERI,
    GLMSG_TEXIMAGE2D,
    GLMSG_ENABLE,
    GLMSG_TEXSUBIMAGE2D,
    GLMSG_TEXCOORD2F,
    GLMSG_VERTEX2F,
    GLMSG_DELETETEXTURES,
    GLMSG_COLOR3F,
    GLMSG_COLOR4F,
    GLMSG_CLEARCOLOR,
    GLMSG_CLEAR,
    GLMSG_MATRIXMODE,
    GLMSG_LOADIDENTITY,
    GLMSG_ORTHO,
    GLMSG_GENTEXTURES,
    GLMSG_GETERROR,
    GLMSG_CLOSE,
    GLMSG_SWAPBUFFERS
};

/*commands struct*/
struct GLmsg {
    GLMSG_ENUM id;
    /*sync thread*/
    struct {
        std::shared_ptr<std::condition_variable> conVarSync;
        std::shared_ptr<std::mutex> mutexSync;
        bool *waitSync;
    } sync;
    /*commnads data*/
    union {
        struct {
            int idLayer;
        } initCommnads;
        struct {
            GLsizei width;
            GLsizei height;
        } viewport;
        struct {
            GLenum mode;
        } begin;
        struct {
            GLenum target;
            GLuint texture;
        } bindTexture;
        struct {
            GLenum target;
            GLenum pname;
            GLint param;
        } texParameteri;
        struct {
            GLenum target;
            GLint level;
            GLint internalformat;
            GLsizei width;
            GLsizei height;
            GLint border;
            GLenum format;
            GLenum type;
            const GLvoid *pixels;
        } texImage2D;
        struct {
            GLenum cap;
        } enable;
        struct {
            GLenum target;
            GLint level;
            GLint xoffset;
            GLint yoffset;
            GLsizei width;
            GLsizei height;
            GLenum format;
            GLenum type;
            const GLvoid *pixels;
        } texSubImage2D;
        struct {
            GLfloat s;
            GLfloat t;
        } texCoord2f;
        struct {
            GLfloat x;
            GLfloat y;
        } vertex2f;
        struct {
            GLsizei n;
            const GLuint *textures;
        } deleteTextures;
        struct {
            GLfloat red;
            GLfloat green;
            GLfloat blue;
        } color3f;
        struct {
            GLfloat red;
            GLfloat green;
            GLfloat blue;
            GLfloat alpha;
        } color4f;
        struct {
            GLclampf red;
            GLclampf green;
            GLclampf blue;
            GLclampf alpha;
        } clearColor;
        struct {
            GLbitfield mask;
        } clear;
        struct {
            GLenum mode;
        } matrixMode;
        struct {
            GLdouble left;
            GLdouble right;
            GLdouble bottom;
            GLdouble top;
            GLdouble near_val;
            GLdouble far_val;
        } ortho;
        struct {
            GLsizei n;
            GLuint **textures;
        } genTextures;
        struct {
            GLenum *error;
        } getError;
    } data;
};

#endif
