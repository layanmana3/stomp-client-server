package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Server;


public class StompServer {

    public static void main(String[] args) {
        // TODO: implement this
        int port = Integer.parseInt(args[0]);
        String srv = args[1];
        if(srv.equals("tpc"))
            Server.threadPerClient(
                port, //port
                () -> new StompMessagingProtocolImplemnt<String>(), //protocol factory
                () -> new LineMessageEncoderDecoder() //message encoder decoder factory
            ).serve();
        else
            Server.reactor(
                    4,
                    port, //port
                    () -> new StompMessagingProtocolImplemnt<String>(), //protocol factory
                    () -> new LineMessageEncoderDecoder() //message encoder decoder factory
            ).serve();
    }
}
