#ifndef OPENGLERROR_H
#define OPENGLERROR_H

#include <QEvent>
#include <QStack>

class OpenGLError : public QEvent
{
public:

  /*****************************************************************************
   * Different Possible Error Types
   ****************************************************************************/
  enum FunctionType
  {
    bind,
    read,
    create,
    unmap,
    link,
    addShader,
    addShaderFromSourceCode,
    addShaderFromSourceFile
  };

  enum ObjectType
  {
    QOpenGLBuffer,
    QOpenGLShaderProgram,
    QOpenGLVertexArrayObject
  };

  /*****************************************************************************
   * Event Methods
   ****************************************************************************/
  // Constructors / Destructors
  OpenGLError(char const *callerName, char const *fnName, ObjectType objType, FunctionType fnType);
  virtual ~OpenGLError();

  // Accessing Types
  char const *functionName() const;
  char const *callerName() const;
  ObjectType objectType() const;
  FunctionType functionType() const;

  // Static Access
  static QEvent::Type type();
  static bool sendEvent(OpenGLError *event);
  static void pushErrorHandler(QObject *obj);
  static void popErrorHandler();

private:
  char const *m_functionName;
  char const *m_callerName;
  ObjectType m_objectType;
  FunctionType m_functionType;
  static QStack<QObject*> m_errorHandler;
};

// Inline Functions
inline OpenGLError::OpenGLError(char const *callerName, char const *fnName, ObjectType objType, FunctionType fnType) : QEvent(type()), m_functionName(fnName), m_callerName(callerName), m_objectType(objType), m_functionType(fnType) {}
inline OpenGLError::~OpenGLError() {}
inline char const* OpenGLError::functionName() const { return m_functionName; }
inline char const* OpenGLError::callerName() const { return m_callerName; }
inline OpenGLError::ObjectType OpenGLError::objectType() const { return m_objectType; }
inline OpenGLError::FunctionType OpenGLError::functionType() const { return m_functionType; }

#endif // OPENGLERROR_H
