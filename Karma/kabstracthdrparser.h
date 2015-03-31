#ifndef KABSTRACTHDRPARSER_H
#define KABSTRACTHDRPARSER_H KAbstractHdrParser

class KAbstractReader;

class KAbstractHdrParserPrivate;
class KAbstractHdrParser
{
public:

  typedef unsigned char Byte;
  enum PixelOrder
  {
    Negative,
    Positive
  };

  KAbstractHdrParser(KAbstractReader *reader);
  ~KAbstractHdrParser();
  void initialize();
  bool parse();
protected:
  virtual void onKeyValue(char const *key, char const *value) = 0;
  virtual void onResolution(PixelOrder xOrder, PixelOrder yOrder, int width, int height) = 0;
  virtual float *beginData() = 0;
  virtual void endData() = 0;
private:
  KAbstractHdrParserPrivate *m_private;
  friend class KAbstractHdrParserPrivate;
};

#endif // KABSTRACTHDRPARSER_H
