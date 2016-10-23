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

#pragma once

#include <boost/utility/string_view.hpp>
#include <boost/optional.hpp>
#include <string>
#include <vector>

namespace daw {
	struct kv_pair {
		std::string key;
		std::string value;
		kv_pair( );
		kv_pair( std::string k, std::string p );
		~kv_pair( );
		kv_pair( kv_pair const & ) = default;
		kv_pair( kv_pair && ) = default;
		kv_pair & operator=( kv_pair const & ) = default;
		kv_pair & operator=( kv_pair && ) = default;
	};	// kv

	kv_pair parse_kv_string( boost::string_view line );

	struct kv_file {
		using values_type = std::vector<kv_pair>;
		using iterator = values_type::iterator;
		using const_iterator = values_type::const_iterator;
	private:
		std::vector<kv_pair> m_values;
	public:
		kv_file( );
		kv_file( boost::string_view file_name );

		values_type const & data( ) const;
		values_type & data( );
		
		iterator begin( );
		const_iterator begin( ) const;
		const_iterator cbegin( ) const;

		iterator end( );
		const_iterator end( ) const;
		const_iterator cend( ) const;
		std::string to_string( ) const;
		void to_file( boost::string_view file_name ) const;
		kv_file & add( std::string key, std::string value );
	};	// kv_file
}    // namespace daw

