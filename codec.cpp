/*

 Very thin C++ wrapper for LAME library.
 Enables easy PCM to MP3 conversion and vice versa.

 Copyright (C) 2015 Sergey Kolevatov

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.

 */

// $Revision: 1594 $ $Date:: 2015-03-18 #$ $Author: serge $

#include "codec.h"          // self

#include <fstream>          // std::ofstream
#include "lameplus.h"       // LamePlus

NAMESPACE_LAMEPLUS_START

bool Codec::encode( const char *in_file, const char *out_file, unsigned sample_rate, unsigned byte_rate, unsigned channels )
{
    std::ifstream pcm( in_file, std::ios_base::binary | std::ios_base::in );
    std::ofstream mp3( out_file, std::ios_base::binary | std::ios_base::out  );

    if( pcm.is_open() == false )
    {
        return false;
    }

    const int PCM_SIZE = 8192;
    const int MP3_SIZE = 8192;

    short int pcm_buffer[PCM_SIZE * 2];
    unsigned char mp3_buffer[MP3_SIZE];

    LamePlus l;

    l.set_in_samplerate( sample_rate );
    l.set_brate( byte_rate );

    if( channels == 1 )
    {
        l.set_num_channels( 1 );
        l.set_mode( MONO );
    }
    else
    {
        l.set_num_channels( channels );
    }

    l.set_VBR( vbr_default );
    l.init_params();

    while( true )
    {
        int k = ( channels == 1 ) ? 1 : 2;

        pcm.read( reinterpret_cast<char*>( pcm_buffer ), k * sizeof(short int) * PCM_SIZE );

        int read = pcm.gcount();


        if( read > 0 )
        {
            read /= 2;  // need number of 'short int' read

            int write = 0;

            if( channels == 1 )
            {
                write = l.encode_buffer( pcm_buffer, NULL, read, mp3_buffer, MP3_SIZE );
            }
            else
            {
                write = l.encode_buffer_interleaved( pcm_buffer, read, mp3_buffer, MP3_SIZE );
            }

            mp3.write( reinterpret_cast<char*>( mp3_buffer ) , write );
        }

        if( pcm.fail() )
        {
            int write = l.encode_flush( mp3_buffer, MP3_SIZE );

            mp3.write( reinterpret_cast<char*>( mp3_buffer ), write );

            break;
        }
    };

    return true;
}

NAMESPACE_LAMEPLUS_END
