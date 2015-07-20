#include "Callbacks.h"
#include "log/Log.h"

using namespace MOONLIGHT;

void connection_connection_terminated(long errorCode)
{
  isyslog("Error code: %i", errorCode);
}

void connection_display_message(char *msg)
{
  isyslog("%s", msg);
}

void connection_display_transient_message(char *msg)
{
  isyslog("%s", msg);
}

CONNECTION_LISTENER_CALLBACKS MOONLIGHT::getConnectionCallbacks()
{
  CONNECTION_LISTENER_CALLBACKS callbacks = {
      NULL,
      NULL,
      NULL,
      NULL,
      connection_connection_terminated,
      connection_display_message,
      connection_display_transient_message
  };
  return callbacks;
}
