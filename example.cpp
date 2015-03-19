#include <cstdio>

#include "codec.h"  // lameplus::Codec

int main()
{
    printf( "Hello, world\n" );

    lameplus::Codec::encode( "test.wav", "test.mp3", 16000, 44100, 1 );

    return 0;
}
