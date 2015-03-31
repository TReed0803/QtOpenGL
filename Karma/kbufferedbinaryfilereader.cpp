#include "kbufferedbinaryfilereader.h"
#include <QFile>
#include <QString>
#include <QTextStream>

#include <KMacros>

/*******************************************************************************
 * KBufferedFileReaderPrivate
 ******************************************************************************/
class KBufferedBinaryFileReaderPrivate
{
public:
  inline KBufferedBinaryFileReaderPrivate();
  inline KBufferedBinaryFileReaderPrivate(const QString &fileName, size_t buffsize);
  inline ~KBufferedBinaryFileReaderPrivate();
  inline int next();
  QFile m_file;
  char *m_buffer; // Note: (m_buffer == Null) ? !isVaid : isValid;
  char *m_currBuffer, *m_nextBuffer;
  char const *m_bufferPos;
  size_t m_bufferSize;
};

inline KBufferedBinaryFileReaderPrivate::KBufferedBinaryFileReaderPrivate() :
  m_file(), m_buffer(Q_NULLPTR), m_bufferSize(0)
{
  // Intentionally Empty
}

inline KBufferedBinaryFileReaderPrivate::KBufferedBinaryFileReaderPrivate(const QString &fileName, size_t buffsize) :
  m_file(fileName), m_buffer(Q_NULLPTR), m_bufferSize(buffsize)
{
  if (m_file.open(QFile::ReadOnly))
  {
    // m_buffer is also our "isValid"
    m_buffer = new char[buffsize * 2 + 2];

    // Start current position so we can increment on next()
    m_bufferPos = m_buffer - 1;

    // Setup the EOF markers (detect end of buffers)
    m_buffer[m_bufferSize] = EOF;
    m_buffer[2 * m_bufferSize + 1] = EOF;

    // Setup the buffer swaps (only use with _our_ EOF markers)
    m_currBuffer = m_buffer;
    m_nextBuffer = &m_buffer[buffsize + 1];

    // Read the first two chunks (Alternatively: async next buffer)
    m_currBuffer[m_file.read(m_currBuffer, m_bufferSize)] = EOF;
    m_nextBuffer[m_file.read(m_nextBuffer, m_bufferSize)] = EOF;
  }
}

inline KBufferedBinaryFileReaderPrivate::~KBufferedBinaryFileReaderPrivate()
{
  delete [] m_buffer;
}

inline int KBufferedBinaryFileReaderPrivate::next()
{
  ++m_bufferPos;

  // Handle EOF Markers
  if (m_bufferPos == &m_buffer[m_bufferSize] ||
      m_bufferPos == &m_buffer[2 * m_bufferSize + 1])
  {
    // Swap buffers, and start reading from the other buffer.
    m_bufferPos = m_nextBuffer;
    std::swap(m_currBuffer, m_nextBuffer);

    // Start Reading in the next chunk
    m_nextBuffer[m_file.read(m_nextBuffer, m_bufferSize)] = EOF;
  }

  return *m_bufferPos;
}

/*******************************************************************************
 * KBufferedFileReader
 ******************************************************************************/


KBufferedBinaryFileReader::KBufferedBinaryFileReader() :
  m_private(new KBufferedBinaryFileReaderPrivate())
{
  // Intentionally Empty
}

KBufferedBinaryFileReader::KBufferedBinaryFileReader(const QString &fileName, size_t buffsize) :
  m_private(new KBufferedBinaryFileReaderPrivate(fileName, buffsize))
{
  // Intentionally Empty
}

KBufferedBinaryFileReader::~KBufferedBinaryFileReader()
{
  // Intentionally Empty
}

int KBufferedBinaryFileReader::next()
{
  P(KBufferedBinaryFileReaderPrivate);
  return p.next();
}

bool KBufferedBinaryFileReader::valid()
{
  P(KBufferedBinaryFileReaderPrivate);
  return (p.m_buffer != Q_NULLPTR);
}
