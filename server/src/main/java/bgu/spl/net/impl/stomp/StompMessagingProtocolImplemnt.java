package bgu.spl.net.impl.stomp;
import java.sql.SQLOutput;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentLinkedQueue;

import bgu.spl.net.api.Commands;
import bgu.spl.net.api.CommandsImplements;
import bgu.spl.net.api.Pair;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;

public class StompMessagingProtocolImplemnt<T> implements StompMessagingProtocol<T>{
    int connectionId;
    Connections<T> connections;
    private boolean terminate = false;
    Commands commands;
    /**
     * 
     */
     public  StompMessagingProtocolImplemnt(){
        this.commands = new CommandsImplements();
    }

    @Override
    public void start(int connectionId, Connections<T> connections) {
        this.connectionId = connectionId;
        this.connections = connections;
    }

    @Override
    public void process(T message) {
        T response = (T)"";
        String frame  = (String) message;
        String[] lines = frame.split("\n");
        T command = (T)lines[0];
        switch ((String)command) {
            case "CONNECT":
              response = (T) commands.connect(lines,connectionId+"");
              connections.send(connectionId, response);
              break;
            case "SEND":
                response=(T)commands.send(lines,connectionId+"");
                if(!IsErrorFrame((String) response)){
                    String destination = (String)lines[1].substring(lines[1].indexOf(':')+1);
                    ConcurrentLinkedQueue<Pair> queue = commands.returnQ(destination);
                    sendmsgtotopic(queue,response);
                }
              connections.send(connectionId, response);
              break;
            case "SUBSCRIBE":
              response = (T)commands.subscribe(lines,connectionId+"");
              connections.send(connectionId, response);
              break;
            case "UNSUBSCRIBE":
            response = (T)commands.unsubscribe(lines,connectionId+"");
            connections.send(connectionId, response);
              break;
            case "DISCONNECT":
              response=(T)commands.disconnect(lines);
              connections.send(connectionId, response);
              break;
            default:
        }
    }
    public void sendmsgtotopic(ConcurrentLinkedQueue<Pair> queue,T response){
         if(queue!=null){
            for(Pair p:queue){
                connections.send( Integer.parseInt((String) p.getSecond()),(T)response);

        }
    }}

    public boolean IsErrorFrame(String Response){
        String frame  = (String) Response;
        String[] lines = frame.split("\n");
        String command = (String)lines[0];
        return command.equals("ERROR");
    }
    @Override
    public boolean shouldTerminate() {
       return this.terminate;
    }

    

}