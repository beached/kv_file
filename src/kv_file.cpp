// The MIT License (MIT)
//
// Copyright (c) 2016-2017 Darrell Wright
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and / or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <fstream>
#include <sstream>
#include <string>

#include <daw/daw_bounded_stack.h>
#include <daw/daw_parser_helper_sv.h>
#include <daw/daw_string_view.h>

#include "kv_file.h"

namespace daw {
	kv_pair::kv_pair( std::string k, std::string v ) noexcept : key{std::move( k )}, value{std::move( v )} {}

	kv_pair::~kv_pair( ) = default; 

	kv_pair parse_kv_string( daw::string_view line ) {
		daw::bounded_stack_t<char, 2> ch_stack;
		bool in_quote = false;
		auto first = line.begin( );
		while( !line.empty( ) ) {
			switch( line.front( ) ) {
			case '\\':
			case '\"':
				ch_stack.push_back( line.pop_front( ) );
				break;
			case '=':
				if( !in_quote ) {
					return {
					  daw::parser::trim( daw::make_string_view_it( first, line.begin( ) ) ).to_string( ),
					  daw::parser::trim( daw::make_string_view_it( std::next( line.begin( ) ), line.end( ) ) ).to_string( )};
				}
			default:
				line.remove_prefix( );
				break;
			}
			if( !ch_stack.empty( ) ) {
				if( ch_stack.front( ) == '\\' ) {
					if( ch_stack.used( ) == 2 ) {
						ch_stack.clear( );
					}
				} else if( ch_stack.front( ) == '\"' ) {
					in_quote = !in_quote;
					ch_stack.clear( );
				}
			}
		}
		daw::exception::daw_throw( std::string{"Could not find unquoted = symbol to split line on: '"} + line + "'" );
	}

	kv_file::kv_file( daw::string_view file_name ) : m_values{} {
		std::ifstream in_file;
		in_file.open( file_name.data( ), std::ios::binary );

		daw::exception::daw_throw_on_false( in_file, "Unable to open file '{0}'", file_name.to_string( ) );

		in_file >> std::noskipws;
		std::string line;

		while( std::getline( in_file, line ).good( ) ) {
			daw::string_view sv{line};
			sv = daw::parser::trim( sv );
			if( line.empty( ) || line.front( ) == '#' ) {
				continue;
			}
			m_values.push_back( parse_kv_string( line ) );
		}
	}

	kv_file::values_type const &kv_file::data( ) const {
		return m_values;
	}

	kv_file::values_type &kv_file::data( ) {
		return m_values;
	}

	kv_file::iterator kv_file::begin( ) {
		return m_values.begin( );
	}

	kv_file::const_iterator kv_file::begin( ) const {
		return m_values.begin( );
	}

	kv_file::const_iterator kv_file::cbegin( ) const {
		return m_values.cbegin( );
	}

	kv_file::iterator kv_file::end( ) {
		return m_values.end( );
	}

	kv_file::const_iterator kv_file::end( ) const {
		return m_values.end( );
	}

	kv_file::const_iterator kv_file::cend( ) const {
		return m_values.end( );
	}

	std::string kv_file::to_string( ) const {
		std::stringstream ss;
		for( auto const &kv : m_values ) {
			ss << kv.key << "=" << kv.value << '\n';
		}
		return ss.str( );
	}

	void kv_file::to_file( daw::string_view file_name ) const {
		std::ofstream out_file;
		out_file.open( file_name.data( ), std::ios::binary | std::ios::trunc );

		daw::exception::daw_throw_on_false( out_file, "Could not open file '{0}'", file_name.to_string( ) );

		for( auto const &kv : m_values ) {
			out_file << kv.key << "=" << kv.value << '\n';
		}
	}
	kv_file::kv_file( ) : m_values{} {}

	kv_file &kv_file::add( std::string key, std::string value ) {
		m_values.emplace_back( std::move( key ), std::move( value ) );
		return *this;
	}
} // namespace daw

