# STOMP Client-Server System (Java & C++)

A client-server messaging system based on the **STOMP 1.2 protocol**, implemented as part of the SPL231 course.
The system allows users to subscribe to channels and receive real-time event updates.

---

## 🚀 Features
- Java server supporting **Thread-Per-Client (TPC)** and **Reactor** architectures
- Full implementation of STOMP frames:
  - CONNECT, SEND, SUBSCRIBE, UNSUBSCRIBE, DISCONNECT
  - MESSAGE, RECEIPT, ERROR
- C++ client with **multithreading**:
  - One thread for keyboard input
  - One thread for socket communication
- Real-time message broadcasting to subscribed channels
- Game event reporting using **JSON parsing**

---

## 🧱 Project Structure
stomp-client-server/

├── server/ # Java (Maven) – STOMP server

└── client/ # C++ – STOMP client

---

## 🛠️ Technologies
- Java (Sockets, Multithreading, Maven)
- C++ (Sockets, Threads)
- STOMP 1.2 Protocol
- Client-Server Architecture
- Concurrency & Networking



# Reactor server
mvn exec:java -Dexec.mainClass="bgu.spl.net.impl.stomp.StompServer" -Dexec.args="<port> reactor"
