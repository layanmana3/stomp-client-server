package bgu.spl.net.api;

import java.util.Map;
import java.util.concurrent.ConcurrentLinkedQueue;

public interface Commands {
    String connect(String[] lines,String connectionId);
    String send(String[] lines,String connectionId);
    String subscribe(String[] lines,String connectionId);
    String unsubscribe(String[] lines,String connectionId);
    String disconnect(String[] lines);
    ConcurrentLinkedQueue<Pair> returnQ(String connectionId);
}