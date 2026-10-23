#include <cstddef>
#include <cstdint>
#include <string>

#include "src/statesync/completeterminal.h"
#include "src/terminal/parser.h"

extern "C" int LLVMFuzzerTestOneInput( const uint8_t* data, size_t size )
{
  Terminal::Display display( false );
  Terminal::Complete complete( 80, 24 );
  Terminal::Framebuffer state( 80, 24 );
  /* Feed the bytes through the VT/UTF-8 parser (the path remote bytes
   * take via NetworkTransport -> Complete::act(std::string)).  The old
   * per-byte act(Parser::UserByte) overload bypassed the parser and
   * drove the local keyboard-input path instead, leaving the escape
   * sequence dispatcher unfuzzed. */
  complete.act( std::string( reinterpret_cast<const char*>( data ), size ) );
  display.new_frame( true, state, complete.get_fb() );

  return 0;
}
