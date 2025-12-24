package bgu.spl.net.api;

import java.util.concurrent.ConcurrentLinkedQueue;

import bgu.spl.net.srv.Connections;

public interface StompMessagingProtocol<T>  {
    /**
     * Used to initiate the current client protocol with it's personal connection ID and the connections implementation
    **/
    void start(int connectionId, Connections<T> connections);
    
    void process(T message);
    public void sendmsgtotopic( ConcurrentLinkedQueue<Pair> queue,T msg);
    /**
     * @return true if the connection should be terminated
     */
    boolean shouldTerminate();
}
