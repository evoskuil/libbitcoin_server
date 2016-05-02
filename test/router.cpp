#include <czmq.h>
#include <czmq++/czmqpp.hpp>
#include <bitcoin/bitcoin.hpp>

int main()
{
    czmqpp::context context;

    czmqpp::socket socket(context, ZMQ_ROUTER);
    assert(socket.self() != nullptr);

    czmqpp::authenticator authenticate(context);
    authenticate.set_verbose(true);
    std::string client_certs(CURVE_ALLOW_ANY);
    authenticate.configure_curve("*", client_certs);

    czmqpp::certificate certificate("C:\\ProgramData\\libbitcoin\\cert.txt");
    assert(certificate.valid());
    certificate.apply(socket);
    ////static const auto public_key = "rq:rM>}U?@Lns47E1%kR.o@n%FcmmsL/@{H8]yf7";
    ////static const auto secret_key = "JTKVSB%%)wK0E.X)V>+}o?pNmC{O&4W4b!Ni{Lh6";
    ////zmq_setsockopt(socket.self(), ZMQ_CURVE_PUBLICKEY, public_key, strlen(public_key));
    ////zmq_setsockopt(socket.self(), ZMQ_CURVE_SECRETKEY, secret_key, strlen(secret_key));

    int as_server = 1;
    socket.set_curve_server(as_server);
    ////zmq_setsockopt(socket.self(), ZMQ_CURVE_SERVER, &as_server, sizeof(int));

    auto connected = socket.bind("tcp://*:9091");
    assert(connected == 9091);

    czmqpp::poller poller(socket);
    assert(poller.self() != nullptr);

    czmqpp::socket which = poller.wait(100000);
    assert(which == socket);

    czmqpp::message message;
    message.receive(socket);
}
