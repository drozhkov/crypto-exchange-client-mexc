#include <iostream>
#include <exception>

#include "crypto-exchange-client-mexc/client.hpp"

#include "api-secret.hpp"


int main()
{
	try {
		as::cryptox::mexc::Client client(
			as::cryptox::mexc::ApiKey(), as::cryptox::mexc::ApiSecret() );

		client
			.ErrorHandler( []( as::cryptox::Client &, size_t wsClientIndex ) {
				std::cerr << "ErrorHandler" << std::endl;
			} )
			.run( []( as::cryptox::Client & c, size_t wsClientIndex ) {
				std::cout << "ready: " << wsClientIndex << std::endl;

				// c.subscribeOrderUpdate( wsClientIndex,
				//	[]( as::cryptox::Client & c,
				//		size_t,
				//		as::cryptox::t_order_update & u ) {
				//		std::cout << "order update: " << u.orderId << std::endl;
				//	} );

				auto symbol = c.toSymbol(
					as::cryptox::Coin::BTC, as::cryptox::Coin::USDT );

				c.subscribePriceBookTicker( wsClientIndex,
					symbol,
					[]( as::cryptox::Client & c,
						size_t,
						as::cryptox::t_price_book_ticker & t ) {
						const auto & pair = c.toPair( t.symbol );

						std::cout << "t_price_book_ticker: " << pair.Name()
								  << ", " << t.askPrice.toString() << " / "
								  << t.askQuantity.toString() << " - "
								  << t.bidPrice.toString() << " / "
								  << t.bidQuantity.toString() << std::endl;
					} );
			} );
	}
	catch ( const std::exception & x ) {
		std::cerr << x.what() << std::endl;
	}
	catch ( ... ) {
		std::cerr << "error" << std::endl;
	}

	return 0;
}
