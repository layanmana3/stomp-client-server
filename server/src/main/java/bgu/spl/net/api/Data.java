package bgu.spl.net.api;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentLinkedQueue;

import javax.swing.text.html.HTMLDocument.Iterator;

public class Data {
    private static Data instance;
    private ConcurrentHashMap<String, String> UsersToPassWord = new ConcurrentHashMap<>();
    private ConcurrentHashMap<String, Boolean> UsersToChickIfLogin = new ConcurrentHashMap<>();
    private ConcurrentHashMap<String, ConcurrentLinkedQueue<Pair>> topics = new ConcurrentHashMap<>();

    /**
     * Retrieves the single instance of this class.
     */
    public static synchronized Data getInstance() {
        if (instance == null) {
            instance = new Data();
        }
        return instance;
    }

    //Get Users data
    public ConcurrentHashMap<String, String> getClietUserPass() {
        return UsersToPassWord;
    }

    public ConcurrentHashMap<String, Boolean> getUsersToChickIfLogin() {
        return UsersToChickIfLogin;
    }

    //Get Topics and subscriber's data
    public ConcurrentHashMap<String, ConcurrentLinkedQueue<Pair>> getTopics() {
        return topics;
    }

    public void addUserPass(String username, String passcode){
        UsersToPassWord.put(username, passcode);
    }

    public void addLogin(String username, boolean loggedIn){
        UsersToChickIfLogin.put(username, loggedIn);
    }

    //Get Topics and subscriber's data
    public boolean isSubscribed(String topic, String id, String connectionId) {
        synchronized (getTopics()) {
            //synchronized(getTopics().get(topic)){
            if (getTopics().get(topic).contains(new Pair(id, connectionId)))
                return true;
            else
                return false;
            //}
        }
    }

    //add a new topic
    public void addnewtopic(String topic) {
        synchronized (getTopics()) {
            getTopics().put(topic, new ConcurrentLinkedQueue<Pair>());
        }
    }

    //check if the topic existing
    public boolean isWeHaveTopic(String topic) {
        boolean ans = false;
        synchronized (getTopics()) {
            ans = topics.contains(topic);
        }
        return ans;
    }

    //add the
    public void addnewClient(String topic, String Id, String connectionId) {
        synchronized (getTopics().get(topic)) {
            getTopics().get(topic).add(new Pair(Id, connectionId));
        }
    }

    public boolean removeClient(String id, String connectionId) {
        for (String key : topics.keySet()) {
            ConcurrentLinkedQueue<Pair> queue = topics.get(key);
            // do something with the key and queue
            for (Pair p : queue) {
                if (p.getSecond().equals(connectionId) & p.getFirst().equals(id)) {
                    topics.get(key).remove(p);
                    return true;
                }
            }
        }
        return false;
    }

    public boolean IsExistUser(String login) {
        return UsersToPassWord.containsKey(login);
    }

    public boolean IsLogin(String login) {
        return UsersToChickIfLogin.get(login);
    }

    public boolean IsTruepassword(String login, String Password) {
        return UsersToPassWord.get(login).contains(Password) ;
    }

    public void change(String login, boolean status) {
        UsersToChickIfLogin.replace(login, UsersToChickIfLogin.get(login), status);
    }

    public void reemoveallsebcribtions(String idCient) {
        for (String key : topics.keySet()) {
            ConcurrentLinkedQueue<Pair> queue = topics.get(key);
            // do something with the key and queue
            for (Pair p : queue) {
                if (p.getSecond().equals(idCient))
                    queue.remove(p);
            }
        }
    }

    public String getid(String currentId) {
        for (String key : topics.keySet()) {
            ConcurrentLinkedQueue<Pair> queue = topics.get(key);
            // do something with the key and queue
            for (Pair p : queue) {
                if (p.getSecond().equals(currentId))
                    return (String) p.getFirst();
            }
        }
        return null;
    }

    public ConcurrentLinkedQueue<Pair> gettopic(String destination) {
//        for (String key : topics.keySet()) {
//            ConcurrentLinkedQueue<Pair> queue = topics.get(key);
//            // do something with the key and queue
//            for(Pair p:queue){
//                if (p.getSecond().equals(destination))
//                   return queue;
//            }
//        }
//        return null;
        // }
        return topics.get(destination);
    }
}
