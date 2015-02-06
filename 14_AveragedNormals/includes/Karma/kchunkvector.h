#ifndef KCHUNKVECTOR_H
#define KCHUNKVECTOR_H

template <typename T, size_t S>
class KChunkVector
{
public:

  iterator begin();
  iterator end();
  const_iterator begin() const;
  const_iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;

private:
  class Chunk
  {

  };
  Chunk *_begin;
  Chunk *_end;
};

#endif // KCHUNKVECTOR_H
