#ifndef OPENGLERROREVENT
#define OPENGLERROREVENT

#include <QEvent>

class OpenGLError : public QEvent
{
public:

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

  OpenGLError(char const *fnName, char const *typeName, ErrorType errtype);
  virtual ~OpenGLError();
  char const *functionName() const;
  static QEvent::Type type();
  static bool sendEvent(OpenGLError *event);
  static void setErrorHandler(QObject *obj);

private:
  char const *m_functionName;
  char const *m_typeName;
  ErrorType m_errorType;
  static QObject *m_errorHandler;
};

inline OpenGLError::OpenGLError(char const *fnName, char const *typeName, ErrorType errtype) : QEvent(type()), m_functionName(fnName), m_typeName(typeName), m_errorType(errtype) {}
inline OpenGLError::~OpenGLError() {}
inline char const* OpenGLError::functionName() const { return m_functionName; }


#endif // OPENGLERROREVENT

