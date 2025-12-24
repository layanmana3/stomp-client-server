package bgu.spl.net.srv;

import java.io.IOException;

public interface Connections<T> {

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);
    int addConnection(ConnectionHandler<T> connections);
    void disconnect(int connectionId);
}
