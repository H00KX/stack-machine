#include "sm-core.hpp"

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

static void disassemble(FILE* f)
{
  machine_t m;
  m.load_image(f);

  int32_t *p = &m.memory[0];
  int32_t *end = m.find_end();

  printf("--- %u bytes\n", end - p);

  while ( p <= end ) {
    Op op = static_cast<Op>(*p);
    printf("0x%x %s", p - &m.memory[0], to_s(op));

    switch(op) {
    default: break;
    case PUSH:
      if ( p <= end ) {
        p += sizeof(int32_t);
        printf(" 0x%x", *p);
        if ( isprintable(*p) )
          printf(" ('%s')", printable(*p));
      }
      break;
    }

    printf("\n");
    p += sizeof(int32_t);
  }
}

int main(int argc, char** argv)
{
  for ( int n=1; n<argc; ++n ) {
    if ( argv[n][0] != '-' ) {
      printf("; File %s ", argv[n]);
      disassemble(fileptr(fopen(argv[n], "rb")));
    }
  }

  return 0;
}