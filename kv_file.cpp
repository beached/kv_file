// The MIT License (MIT)
//
// Copyright (c) 2016 Darrell Wright
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files( the "Software" ), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <boost/utility/string_ref.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <fstream>
#include <sstream>
#include <string>

#include "kv_file.h"

namespace daw {
	namespace {
		kv_pair parse_line( boost::string_ref line ) {
			kv_pair result;
			std::vector<char> ch_stack;
			bool in_quote = false;
			for( size_t pos=0; pos<line.size( ); ++pos ) {
				switch( line[pos] ) {
					case '\\':
						ch_stack.push_back( '\\' );
					break;
					case '\"': {
						ch_stack.push_back( '\"' );
						break;
					}
					case '=': 
						if( !in_quote ) {
							auto sep = std::next( line.begin( ), pos );
							result.key = boost::trim_copy( std::string{ line.begin( ), sep } );
							result.value = boost::trim_copy( std::string{ std::next( sep ), line.end( ) } );
							return result;
						}
						break;
				}
				if( !ch_stack.empty( ) ) {
					if( ch_stack.front( ) == '\\' ) {
						if( ch_stack.size( ) == 2 ) {
							ch_stack.clear( );
						}
					} else if( ch_stack.front( ) == '\"' ) {
						in_quote = !in_quote;
						ch_stack.clear( );
					}
				}
			}
			std::stringstream ss;
			ss << "Could not find unquoted = symbol to split line on: '" << line.data( ) << "'";
			throw std::runtime_error( ss.str( ) ); 
		}

	}

	kv_file::kv_file( boost::string_ref file_name ): m_values{ } {
		std::ifstream in_file;
		in_file.open( file_name.data( ), std::ios::binary );

		if( !in_file ) {
			std::stringstream ss;
			ss << "Unable to open file '" << file_name.data( ) << "'";
			throw std::runtime_error( ss.str( ) ); 
		}

		in_file >> std::noskipws;
		std::string line;

		while( std::getline( in_file, line ).good( ) ) {
			line = boost::trim_copy( line );
			if( line.empty( ) || line.front( ) == '#' ) {
				continue;
			}
			m_values.push_back( parse_line( line ) );	
		}
	}

	kv_file::values_type const & kv_file::data( ) const {
		return m_values;
	}
	
	kv_file::values_type & kv_file::data( ) {
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
		for( auto const & kv: m_values ) {
			ss << kv.key << "=" << kv.value << '\n';
		}
		return ss.str( );
	}

	void kv_file::to_file( boost::string_ref file_name ) const {
		std::ofstream out_file;
		out_file.open( file_name.data( ), std::ios::binary | std::ios::trunc );
		if( !out_file ) {
			std::stringstream ss;
			ss << "Could not open file '" << file_name.data( ) << "'";
			throw std::runtime_error( ss.str( ) );
		}
		for( auto const & kv: m_values ) {
			out_file << kv.key << "=" << kv.value << '\n';
		}
	}
}    // namespace daw

