#include "kbufferedfilereader.h"
#include <QFile>
#include <QString>
#include <QTextStream>

#include <KMacros>

/*******************************************************************************
 * KBufferedFileReaderPrivate
 ******************************************************************************/
class KBufferedFileReaderPrivate
{
public:
  inline KBufferedFileReaderPrivate();
  inline KBufferedFileReaderPrivate(const QString &fileName, size_t buffsize);
  inline ~KBufferedFileReaderPrivate();
  inline int next();
  QFile m_file;
  char *m_buffer; // Note: (m_buffer == Null) ? !isVaid : isValid;
  char *m_currBuffer, *m_nextBuffer;
  char const *m_bufferPos;
  size_t m_bufferSize;
};

inline KBufferedFileReaderPrivate::KBufferedFileReaderPrivate() :
  m_file(), m_buffer(Q_NULLPTR), m_bufferSize(0)
{
  // Intentionally Empty
}

inline KBufferedFileReaderPrivate::KBufferedFileReaderPrivate(const QString &fileName, size_t buffsize) :
  m_file(fileName), m_buffer(Q_NULLPTR), m_bufferSize(buffsize)
{
  if (m_file.open(QFile::ReadOnly | QFile::Text))
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

inline KBufferedFileReaderPrivate::~KBufferedFileReaderPrivate()
{
  delete [] m_buffer;
}

inline int KBufferedFileReaderPrivate::next()
{
  ++m_bufferPos;

  // Handle EOF Markers
  if ((*m_bufferPos) == static_cast<char>(EOF))
  {

    // If they're not our markers, this is the end of the file
    if (m_bufferPos != &m_buffer[m_bufferSize] &&
        m_bufferPos != &m_buffer[2 * m_bufferSize + 1])
    {
      --m_bufferPos;
      return KBufferedFileReader::EndOfFile;
    }

    // Otherwise, we've simply read the current buffer.
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


KBufferedFileReader::KBufferedFileReader() :
  m_private(new KBufferedFileReaderPrivate())
{
  // Intentionally Empty
}

KBufferedFileReader::KBufferedFileReader(const QString &fileName, size_t buffsize) :
  m_private(new KBufferedFileReaderPrivate(fileName, buffsize))
{
  // Intentionally Empty
}

KBufferedFileReader::~KBufferedFileReader()
{
  // Intentionally Empty
}

int KBufferedFileReader::next()
{
  P(KBufferedFileReaderPrivate);
  return p.next();
}

bool KBufferedFileReader::valid()
{
  P(KBufferedFileReaderPrivate);
  return (p.m_buffer != Q_NULLPTR);
}
