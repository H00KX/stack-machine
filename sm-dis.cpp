#include "sm-core.hpp"
#include "sm-util.hpp"

static bool isprintable(char c)
{
  return (c>=32 && c<=127)
    || c=='\n'
    || c=='\r'
    || c=='\t';
}

static const char* printable(char c)
{
  static char buf[2];
  buf[0] = c;
  buf[1] = '\0';

  switch ( c ) {
  default: return buf;
  case '\t': return "\\t";
  case '\n': return "\\n";
  case '\r': return "\\r";
  }
}

static void disassemble(machine_t &m)
{
  int32_t end = m.size();

  while ( m.pos() <= end ) {
    Op op = static_cast<Op>(m.cur());
    printf("0x%x %s", m.pos(), to_s(op));

    if ( op==PUSH && m.pos()<=end ) {
        m.next();
        printf(" 0x%x", m.cur());

        if ( isprintable(m.cur()) )
          printf(" ('%s')", printable(m.cur()));
    }

    printf("\n");
    m.next();
  }
}

int main(int argc, char** argv)
{
  for ( int n=1; n<argc; ++n ) {
    if ( argv[n][0] != '-' ) {
      machine_t m;
      m.load_image(fileptr(fopen(argv[n], "rb")));
      printf("; File %s --- %u bytes\n", argv[n], m.size());

      disassemble(m);
    }
  }

  return 0;
}
