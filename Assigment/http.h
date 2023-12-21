#ifndef DUMMY_HTTP_H_INCLUDED
#define DUMMY_HTTP_H_INCLUDED

#define BUFFER_SIZE		1024
#define HTTP_PORT 12345
#define MAX_CONNECTIONS 5

#define DUMMY_REQUEST 	"GET / HTTP/1.0r\n\
Host: www.mcast.edu.mt\r\n\r\n"

#define CMDLINE_REQUEST "GET / HTTP/1.0\r\n\
Host: %s\r\n\r\n"

#define DUMMY_RESPONSE 	"HTTP/1.0 200 OK\r\n\
Server: LLP2 DummyServer (1.0)\r\n\
Last-Modified: Wed, 27 Nov 2016 07:15:56 GMT\r\n\
Content-Type: text/html\r\n\
Connection: Closed\r\n\
\r\n\
<html>\r\n\
  <title>Simple Response</title>\r\n\
  <body>\r\n\
    <img src=\"https://cdn.tutsplus.com/net/uploads/legacy/511_http/http_diagram.png\" />\r\n\
    <br />\r\n\
    <a href=\"https://code.tutsplus.com/tutorials/http-headers-for-dummies--net-8039\">HTTP Headers for Dummies</a>\r\n\
  </body>\r\n\
</html>"

#define REDIR_RESPONSE "HTTP/1.1 301 Moved Permanently\r\n\
Location: http://www.mcast.edu.mt/\r\n\
\r\n"

#endif //DUMMY_HTTP_H_INCLUDED