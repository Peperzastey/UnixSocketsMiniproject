#ifndef CLIENT_MANAGER_H
#define CLIENT_MANAGER_H

#include "SocketEndpoint.h"
#include "BlockingMessageQueue.h"

extern "C" {
  #include <pthread.h>  // POSIX threads
  #include <sys/types.h>
}

#include <memory>

//class comm_layer::CommSocketEndpoint;

namespace clients
{
    namespace internals
    {
        extern "C" void * receiver_thread_routine_wrapper(void *comm_manager);

        extern "C" void * sender_thread_routine_wrapper(void *comm_endpoint);
    }

    // CommManager ?
    class ClientManager
    {
        private:
          // TODO
          //int client_id_;
          /*comm_layer::CommSocketEndpoint comm_endpoint_;*/
          std::unique_ptr< comm_layer::CommSocketEndpoint > comm_endpoint_ptr_;
          // TODO? lazy creation (in startFullDuplexComm, not in ctor) ?
          threads_layer::BlockingMessageQueue message_queue_;

          // TODO?
          //SessionManager &session_manager_;
          //or: std::list<SessionManager &> sessions_;

          pthread_t sender_thread_;
          pthread_t receiver_thread_;

          const std::size_t MSG_BUF_SIZE = 100;

        private:
          void sender_thread_routine();
          void receiver_thread_routine() /*const*/;

          void cancel_sender_thread();

          friend /*extern "C"*/ void * internals::receiver_thread_routine_wrapper(void *comm_manager);
          friend /*extern "C"*/ void * internals::sender_thread_routine_wrapper(void *comm_manager);

        public:
          // TODO add base class CommEndpoint
          /*ClientManager(const comm_layer::CommSocketEndpoint &comm_endpoint);*/
          ClientManager(/*const*/ std::unique_ptr< comm_layer::CommSocketEndpoint > comm_endpoint_ptr);
          ~ClientManager()/* = default*/;

          // start full-duplex communication with the client (async)
          void startFullDuplexComm();
          // send message to the client (async)
          void send(const messages::Message &message);

          //void close();   // close connection with the client (initiated by the server)
    };
}

#endif /*CLIENT_MANAGER_H*/