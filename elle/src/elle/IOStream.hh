#ifndef ELLE_IO_STREAM_HH
# define ELLE_IO_STREAM_HH

# include <iostream>

# include <elle/fwd.hh>

namespace elle
{
  class StreamBuffer;

  /// RAII helper to call clear on std streams.
  class IOStreamClear
  {
  public:
    IOStreamClear (std::ios& s);
    ~IOStreamClear();

  private:
    std::ios& _stream;
  };

  class IOStream: public std::iostream
  {
  public:
    IOStream(std::streambuf* buffer);
    IOStream(IOStream&& buffer);
    ~IOStream();

  private:
    std::streambuf* _buffer;
  };

  class StreamBuffer: public std::streambuf
  {
  public:
    typedef uint64_t Size;
    StreamBuffer();
    virtual
    ~StreamBuffer();

  /// API to override.
  protected:
    friend class IOStream;

    /// The buffer where to write.
    virtual
    WeakBuffer
    write_buffer() = 0;

    /// The buffer with the next available data.
    virtual
    WeakBuffer
    read_buffer()  = 0;

    /// Synchronize the write buffer to the underlying implementation.
    virtual
    void
    flush(Size size);

  /// std::streambuf interface.
  protected:
    virtual
    int
    underflow();

    virtual
    int
    overflow(int c);

    virtual
    int
    sync();
  };

  /// Simple implementation of a streambuf with local buffers.
  class PlainStreamBuffer: public StreamBuffer
  {
  public:
    typedef StreamBuffer::Size Size;
    PlainStreamBuffer();
    ~PlainStreamBuffer();

  protected:
    friend class IOStream;
    virtual Size read (char* buffer, Size size) = 0;
    virtual void write(char* buffer, Size size) = 0;

    virtual
    WeakBuffer
    write_buffer();

    virtual
    WeakBuffer
    read_buffer();

    virtual
    void
    flush(Size size);

  private:
    static const int _bufsize = (1 << 12); // == 4096 bytes.
    char _ibuf[_bufsize];
    char _obuf[_bufsize];
  };

  class DynamicStreamBuffer : public StreamBuffer
  {
  public:
    typedef StreamBuffer::Size Size;
    typedef unsigned char Byte;

    DynamicStreamBuffer(Size size);
    ~DynamicStreamBuffer();

  protected:
    friend class IOStream;
    virtual Size read(char *buffer, Size size) = 0;
    virtual void write(char *buffer, Size size) = 0;

    virtual
    WeakBuffer
    read_buffer();

    virtual
    WeakBuffer
    write_buffer();

    virtual
    void
    flush(Size size);

  private:
    Size const  _bufsize;
    Byte        *_ibuf;
    Byte        *_obuf;
  };

}

#endif
