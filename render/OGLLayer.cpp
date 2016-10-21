#include"OGLLayer.h"

OGLLayer::OGLLayer(unsigned int id, std::shared_ptr<OGLSender> oglSender)
{
    /*shared*/
    mOglSender = oglSender;
    /*save id layer*/
    mId = id;
    /*init shared data*/
    mConVarSync = std::shared_ptr<std::condition_variable> (new std::condition_variable);
    mMutexSync = std::shared_ptr<std::mutex> (new std::mutex);
    mWaitSync = true;
}

OGLLayer::~OGLLayer()
{
    /*nothing*/
}

void OGLLayer::beginCommands()
{
    GLmsg message;
    message.id = GLMSG_INITCOMMANDS;
    message.data.initCommnads.idLayer = mId;
    mOglSender->begin();
    mOglSender->pushMessage(message);
}

void OGLLayer::endCommands()
{
    GLmsg message;
    message.id = GLMSG_FINISHCOMMANDS;
    mOglSender->pushMessage(message);
    mOglSender->end();
}

void OGLLayer::glBegin(GLenum mode)
{
    GLmsg message;
    message.id = GLMSG_BEGIN;
    message.data.begin.mode = mode;
    mOglSender->pushMessage(message);
}

void OGLLayer::glEnd()
{
    GLmsg message;
    message.id = GLMSG_END;
    mOglSender->pushMessage(message);
}

void OGLLayer::glBindTexture(GLenum target, GLuint texture)
{
    GLmsg message;
    message.id = GLMSG_BINDTEXTURE;
    message.data.bindTexture.target = target;
    message.data.bindTexture.texture = texture;
    mOglSender->pushMessage(message);
}

void OGLLayer::glTexParameteri(GLenum target, GLenum pname, GLint param)
{
    GLmsg message;
    message.id = GLMSG_TEXPARAMETERI;
    message.data.texParameteri.target = target;
    message.data.texParameteri.pname = pname;
    message.data.texParameteri.param = param;
    mOglSender->pushMessage(message);
}

void OGLLayer::glTexImage2D(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels)
{
    GLmsg message;
    message.id = GLMSG_TEXIMAGE2D;
    message.data.texImage2D.target = target;
    message.data.texImage2D.level = level;
    message.data.texImage2D.internalformat = internalformat;
    message.data.texImage2D.width = width;
    message.data.texImage2D.height = height;
    message.data.texImage2D.border = border;
    message.data.texImage2D.format = format;
    message.data.texImage2D.type = type;
    message.data.texImage2D.pixels = pixels;
    mOglSender->pushMessage(message);
}

void OGLLayer::glEnable(GLenum cap)
{
    GLmsg message;
    message.id = GLMSG_ENABLE;
    message.data.enable.cap = cap;
    mOglSender->pushMessage(message);
}

void OGLLayer::glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels)
{
    GLmsg message;
    message.id = GLMSG_TEXSUBIMAGE2D;
    message.data.texSubImage2D.target = target;
    message.data.texSubImage2D.level = level;
    message.data.texSubImage2D.xoffset = xoffset;
    message.data.texSubImage2D.yoffset = yoffset;
    message.data.texSubImage2D.width = width;
    message.data.texSubImage2D.height = height;
    message.data.texSubImage2D.format = format;
    message.data.texSubImage2D.type = type;
    message.data.texSubImage2D.pixels = pixels;
    mOglSender->pushMessage(message);
}

void OGLLayer::glTexCoord2f(GLfloat s, GLfloat t)
{
    GLmsg message;
    message.id = GLMSG_TEXCOORD2F;
    message.data.texCoord2f.s = s;
    message.data.texCoord2f.t = t;
    mOglSender->pushMessage(message);
}

void OGLLayer::glVertex2f(GLfloat x, GLfloat y)
{
    GLmsg message;
    message.id = GLMSG_VERTEX2F;
    message.data.vertex2f.x = x;
    message.data.vertex2f.y = y;
    mOglSender->pushMessage(message);
}

void OGLLayer::glDeleteTextures(GLsizei n, const GLuint *textures)
{
    GLmsg message;
    message.id = GLMSG_DELETETEXTURES;
    message.data.deleteTextures.n = n;
    message.data.deleteTextures.textures = textures;
    mOglSender->pushMessage(message);
}

void OGLLayer::glColor3f(GLfloat red, GLfloat green, GLfloat blue)
{
    GLmsg message;
    message.id = GLMSG_COLOR3F;
    message.data.color3f.red = red;
    message.data.color3f.green = green;
    message.data.color3f.blue = blue;
    mOglSender->pushMessage(message);
}

void OGLLayer::glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
    GLmsg message;
    message.id = GLMSG_COLOR4F;
    message.data.color4f.red = red;
    message.data.color4f.green = green;
    message.data.color4f.blue = blue;
    message.data.color4f.alpha = alpha;
    mOglSender->pushMessage(message);
}

void OGLLayer::glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha)
{
    GLmsg message;
    message.id = GLMSG_CLEARCOLOR;
    message.data.clearColor.red = red;
    message.data.clearColor.green = green;
    message.data.clearColor.blue = blue;
    message.data.clearColor.alpha = alpha;
    mOglSender->pushMessage(message);
}

void OGLLayer::glClear(GLbitfield mask)
{
    GLmsg message;
    message.id = GLMSG_CLEAR;
    message.data.clear.mask = mask;
    mOglSender->pushMessage(message);
}

void OGLLayer::glMatrixMode(GLenum mode)
{
    GLmsg message;
    message.id = GLMSG_MATRIXMODE;
    message.data.matrixMode.mode = mode;
    mOglSender->pushMessage(message);
}

void OGLLayer::glLoadIdentity()
{
    GLmsg message;
    message.id = GLMSG_LOADIDENTITY;
    mOglSender->pushMessage(message);
}

void OGLLayer::glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val)
{
    GLmsg message;
    message.id = GLMSG_ORTHO;
    message.data.ortho.left = left;
    message.data.ortho.right = right;
    message.data.ortho.bottom = bottom;
    message.data.ortho.top = top;
    message.data.ortho.near_val = near_val;
    message.data.ortho.far_val = far_val;
    mOglSender->pushMessage(message);
}

void OGLLayer::glGenTextures(GLsizei n, GLuint *textures)
{
    GLmsg message;
    message.id = GLMSG_GENTEXTURES;
    message.data.genTextures.n = n;
    message.data.genTextures.textures = &textures;
    message.sync.conVarSync = mConVarSync;
    message.sync.mutexSync = mMutexSync;
    message.sync.waitSync = &mWaitSync;
    /*send*/
    mOglSender->postMessage(message);
    /*wait*/
    std::unique_lock<std::mutex> lockWaitSync(*mMutexSync.get());
    while (mWaitSync) {
        mConVarSync->wait(lockWaitSync);
    }
    mWaitSync = true;
}

void OGLLayer::glGetError(GLenum &error)
{
    GLmsg message;
    message.id = GLMSG_GETERROR;
    message.data.getError.error = &error;
    message.sync.conVarSync = mConVarSync;
    message.sync.mutexSync = mMutexSync;
    message.sync.waitSync = &mWaitSync;
    /*send*/
    mOglSender->postMessage(message);
    /*wait*/
    std::unique_lock<std::mutex> lockWaitSync(*mMutexSync.get());
    while (mWaitSync) {
        mConVarSync->wait(lockWaitSync);
    }
    mWaitSync = true;
}
