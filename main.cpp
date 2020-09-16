// VISA-Local-Daemon.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <thread>
//#include <httplib.h>
#include <ixwebsocket/IXNetSystem.h>
#include <ixwebsocket/IXWebSocketServer.h>

int main()
{
	ix::initNetSystem();

	std::thread* thread1 = new std::thread([]() {
		//httplib::Server svr;

		//svr.Get("/hi", [](const httplib::Request& /*req*/, httplib::Response& res) {
		//	res.set_content("Hello World!", "text/plain");
		//	});

		//svr.listen("127.0.0.1", 8080);

		return 0;
		});
	std::thread* thread2 = new std::thread([]() {

		ix::WebSocketServer server(8080);

		server.setOnConnectionCallback(
			[&server](std::shared_ptr<ix::WebSocket> webSocket,
				std::shared_ptr<ix::ConnectionState> connectionState)
			{
				webSocket->setOnMessageCallback(
					[webSocket, connectionState, &server](const ix::WebSocketMessagePtr& msg)
					{
						if (msg->type == ix::WebSocketMessageType::Open)
						{
							std::cerr << "New connection" << std::endl;

							// A connection state object is available, and has a default id
							// You can subclass ConnectionState and pass an alternate factory
							// to override it. It is useful if you want to store custom
							// attributes per connection (authenticated bool flag, attributes, etc...)
							std::cerr << "id: " << connectionState->getId() << std::endl;

							// The uri the client did connect to.
							std::cerr << "Uri: " << msg->openInfo.uri << std::endl;

							std::cerr << "Headers:" << std::endl;
							for (auto it : msg->openInfo.headers)
							{
								std::cerr << it.first << ": " << it.second << std::endl;
							}
						}
						else if (msg->type == ix::WebSocketMessageType::Message)
						{
							// For an echo server, we just send back to the client whatever was received by the server
							// All connected clients are available in an std::set. See the broadcast cpp example.
							// Second parameter tells whether we are sending the message in binary or text mode.
							// Here we send it in the same mode as it was received.
							webSocket->send(msg->str, msg->binary);
						}
					}
				);
			}
		);

		auto res = server.listen();
		if (!res.first)
		{
			std::cout << res.second;
			// Error handling
			return 1;
		}

		// Run the server in the background. Server can be stoped by calling server.stop()
		server.start();

		// Block until server.stop() is called.
		server.wait();

		//return 0;
		});

	thread1->join();
	thread2->join();
	std::cout << "Hello World!\n";


	// getchar();
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
