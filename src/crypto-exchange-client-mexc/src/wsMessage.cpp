/*
MIT License
Copyright (c) 2023 Denis Rozhkov <denis@rozhkoff.com>
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

/// wsMessage.cpp
///
/// 0.0 - created (Denis Rozhkov <denis@rozhkoff.com>)
///

#include "crypto-exchange-client-mexc/wsMessage.hpp"


namespace as::cryptox::mexc {

	std::shared_ptr<::as::cryptox::ApiMessageBase> WsMessage::deserialize(
		const char * data, size_t size )
	{

		auto v = boost::json::parse( { data, size } );
		auto & v1 = v;

		WsMessage * r = nullptr;

		auto & o = v.get_object();

		if ( o.contains( "c" ) ) {
			auto & ch = o["c"].get_string();

			if ( ch.starts_with( "spot@public.bookTicker.v3.api@" ) ) {
				r = new WsMessagePriceBookTicker;
			}
		}

		if ( nullptr == r ) {
			return s_unknown;
		}

		r->deserialize( v1 );

		return std::shared_ptr<::as::cryptox::WsMessage>( r );
	}

	////

	void WsMessagePong::deserialize( boost::json::value & o )
	{
	}

	////

	void WsMessagePriceBookTicker::deserialize( boost::json::value & v )
	{
		auto & o = v.at("d").get_object();
		m_askPrice.Value( o["a"].get_string() );
		m_askSize.Value( o["A"].get_string() );
		m_bidPrice.Value( o["b"].get_string() );
		m_bidSize.Value( o["B"].get_string() );
		m_symbolName.assign( v.at("s").get_string() );
	}

} // namespace as::cryptox::mexc
