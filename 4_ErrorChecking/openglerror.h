#ifndef OPENGLERROR_H
#define OPENGLERROR_H

#include <QEvent>

class OpenGLError : public QEvent
{
public:

  /*****************************************************************************
   * Different Possible Error Types
   ****************************************************************************/
  enum ErrorType
  {
    ErrorOn_bind,
    ErrorOn_create,
    ErrorOn_unmap,
    ErrorOn_link,
    ErrorOn_addShader,
    ErrorOn_addShaderFromSourceCode,
    ErrorOn_addShaderFromSourceFile
  };

  /*****************************************************************************
   * Event Methods
   ****************************************************************************/
  // Constructors / Destructors
  OpenGLError(char const *fnName, char const *callerName, ErrorType errtype);
  virtual ~OpenGLError();

  // Accessing Types
  char const *functionName() const;
  char const *callerName() const;
  ErrorType errorType() const;

  // Static Access
  static QEvent::Type type();
  static bool sendEvent(OpenGLError *event);
  static void setErrorHandler(QObject *obj);

private:
  char const *m_functionName;
  char const *m_callerName;
  ErrorType m_errorType;
  static QObject *m_errorHandler;
};

// Inline Functions
inline OpenGLError::OpenGLError(char const *fnName, char const *callerName, ErrorType errtype) : QEvent(type()), m_functionName(fnName), m_callerName(callerName), m_errorType(errtype) {}
inline OpenGLError::~OpenGLError() {}
inline char const* OpenGLError::functionName() const { return m_functionName; }
inline char const* OpenGLError::callerName() const { return m_callerName; }
inline OpenGLError::ErrorType OpenGLError::errorType() const { return m_errorType; }

#endif // OPENGLERROR_H

