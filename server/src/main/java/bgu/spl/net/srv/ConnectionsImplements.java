package bgu.spl.net.srv;
import java.util.concurrent.ConcurrentHashMap;
//atomic Intger
import java.util.concurrent.atomic.AtomicInteger;
public class ConnectionsImplements<T> implements Connections<T> {
    private ConcurrentHashMap<Integer, ConnectionHandler<T>> ClientTohandler = new ConcurrentHashMap<>();
    AtomicInteger counterId=new AtomicInteger(0);
    public Connections<T> connections;
    @Override
    public boolean send(int connectionId, T msg) {
    if(this.ClientTohandler.get(connectionId) != null){
        //To prevent user's in an client to right at the same time.
        synchronized(this.ClientTohandler.get(connectionId)){
            this.ClientTohandler.get(connectionId).send(msg);
        }
        return true;
    }

    return false;
}
public int addConnection(ConnectionHandler<T> connections){
    synchronized (this){
        this.ClientTohandler.put(counterId.get(), connections);
        int id=counterId.get();
        counterId.incrementAndGet();
        return id;
    }

}
@Override
public void send(String channel, T msg) {
    
    
}

@Override
public void disconnect(int connectionId) {
    this.ClientTohandler.remove(connectionId);
    
}
    
}
