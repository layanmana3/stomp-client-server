package bgu.spl.net.api;
import java.text.MessageFormat;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentLinkedQueue;
import java.util.concurrent.atomic.AtomicInteger;

import bgu.spl.net.api.Commands;

public class CommandsImplements implements Commands {
    Data data = Data.getInstance();
    String Login="";
    AtomicInteger messageid= new AtomicInteger(0);
    //command's come from client!
    @Override
    public String connect(String[] lines, String connectionId) {
        String login = (String)lines[3].substring(lines[3].indexOf(':')+1);
        String passcode = (String)lines[4].substring(lines[4].indexOf(':')+1);
        String reply = "" ;
        Login=login;

        if(!data.IsExistUser(login)){
            data.addUserPass(login, passcode);
            data.addLogin(login,true);
            System.out.println("here1");
            reply =  makeConnected();
        }

        else if(data.IsLogin(login)){
            reply =  makeError("User already logged in.");
        }
        else if(data.IsTruepassword(login, passcode)){
            data.change(login,true);
            reply = makeConnected();
            System.out.println("here2");
        }
        else{
            reply =makeError("Wrong password");
        }
        return reply;

    }

    @Override
    public String send(String[] lines,String connectionId) {
        String destination = (String)lines[1].substring(lines[1].indexOf(':')+1);
        String framebody="";
//        if(!IsLegalMsg(destination,connectionId))
//            return makeError("The msg is Unlegil msg");
        for(int i=2;i<lines.length;i++){
            framebody=framebody+lines[i]+"\n";
        }
        String msg=Message(connectionId,messageid.get()+"",destination,framebody);
        messageid.incrementAndGet();
        return msg;
    }

    public ConcurrentLinkedQueue<Pair> returnQ(String destination){
        return data.gettopic(destination);
    }
    public String Message(String subscripthion,String messageId,String destination,String frameBody){
        String Command="MESSAGE";
        Map<String, String> headers = new HashMap<>();
        String headers1="subscription:".concat(subscripthion);
        String headers2="message-id:".concat(messageId);
        String headers3="destination:".concat(destination);
        String FrameBody=frameBody;
        String msg=Command+"\n"+headers1+"\n"+headers2+"\n"+headers3+"\n"+frameBody+"\n";
        return msg;
    } 

    ///function that ckeck if the msg is legal 
    /// if there is this topic ?? if he is subscriber
    public boolean IsLegalMsg(String destination,String connectionId){
        boolean ans =false;
        if(data.isWeHaveTopic(destination)){
            String id=data.getid(connectionId);
            if(id!=null)
                if(data.isSubscribed(destination, id, connectionId))
                    ans=true;
            }
            return ans;
    }
    @Override
    public String subscribe(String[] lines,String connectionId) {
        String destination = lines[1].substring(lines[1].indexOf(':')+1);
        String id  = lines[2].substring(lines[2].indexOf(':')+1);
        String Received=makeReceipt(lines[3].substring(lines[3].indexOf(':')+1));
        if(!data.isWeHaveTopic(destination)){
            data.addnewtopic(destination);
            data.addnewClient(destination, id,connectionId);
        }
        else if(!data.isSubscribed(destination, id,connectionId)){
            data.addnewClient(destination, id,connectionId);
        }
        else{
            Received=makeError("SUBSCRIBE the client is subscribed!");
        }
        return Received;
    }
  
    @Override
    public String unsubscribe(String[] lines,String connectionId ) {
        String id  = lines[1].substring(lines[1].indexOf(':')+1);
        String response="";
        if(data.removeClient(id, connectionId))
            response= "RECEIPT\n"+"receipt-id:"+lines[2].substring(lines[2].indexOf(':')+1)+"\n";
        else{
            response= makeError("unsubscribe ,the client is UNsubscribed!");
        }
        return response;
    }

    @Override
    public String disconnect(String[] lines) {
        String receipt = lines[1].substring(lines[1].indexOf(':')+1);
        data.reemoveallsebcribtions(receipt);
        data.change(Login, false); 
        return makeReceipt(receipt);
    }
    private String makeError(String problem){
        String frameToSend="ERROR\nmessage:"+problem+"\n\n";
        return frameToSend;
    }
    private String makeConnected(){
        String frameToSend="CONNECTED\n"+"version:1.2\n\n";
        return  frameToSend;
    }
    private String makeReceipt(String recipt){
        String frameToSend="RECEIPT\n"+"receipt-id:"+recipt+"\n";
        return frameToSend;
    }
    
}
