/** 
 * NOT PRETTY - But taken for productvity, rewrite ASAP.
 * Comes from :http://stackoverflow.com/questions/15053753/writing-a-manipulator-for-a-custom-stream-class 
 */
#ifndef INDENTING_OSTREAM_HPP
#define INDENTING_OSTREAM_HPP

#include <pre/detail/namespace_compatibility.hpp>

#include <ostream>
#include <streambuf>

namespace pre { namespace iostreams {

  class indenting_streambuf: public std::streambuf {

  public:
      indenting_streambuf(std::streambuf* sbuf): 
        m_sbuf(sbuf), 
        m_indent(4), 
        m_need(true) {}

      int indent() const { return m_indent; }
      void indent() { m_indent+=4; }
      void deindent() { if(m_indent >= 4) m_indent-= 4; }

  protected:

      virtual int_type overflow(int_type c) {

          if (traits_type::eq_int_type(c, traits_type::eof()))

              return m_sbuf->sputc(c);

          if (m_need)
          {
              fill_n(std::ostreambuf_iterator<char>(m_sbuf), m_indent, ' ');
              m_need = false;
          }

          if (traits_type::eq_int_type(m_sbuf->sputc(c), traits_type::eof()))

              return traits_type::eof();

          if (traits_type::eq_int_type(c, traits_type::to_char_type('\n')))

              m_need = true;

          return traits_type::not_eof(c);
      }

      std::streambuf* m_sbuf;
      int m_indent;
      bool m_need;
  };

  class indenting_stream : public std::ostream {
  public:
      indenting_stream(std::ostream &os) 
        : std::ostream(&ib), ib(os.rdbuf()) {};

      std::ostream& indent(std::ostream& stream) {
          ib.indent();
          return stream;
      }

      friend std::ostream& deindent(std::ostream& stream);
      friend std::ostream& indent(std::ostream& stream);

  private:
      indenting_streambuf ib;
  };

  inline std::ostream& deindent(std::ostream& stream)
  {
      indenting_stream* pIndentStream = dynamic_cast<indenting_stream*>(&stream);
      if (pIndentStream != nullptr)
      {
          pIndentStream->ib.deindent();
      }

      return stream;
  }

  inline std::ostream& indent(std::ostream& stream)
  {
      indenting_stream* pIndentStream = dynamic_cast<indenting_stream*>(&stream);
      if (pIndentStream != nullptr)
      {
          pIndentStream->ib.indent();
      }

      return stream;
  }

}}

#endif
