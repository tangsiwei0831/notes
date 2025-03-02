<h1>TCP</h1>
<details>
  <h2>Layer</h2>
  TCP: Byte Stream<br>
  UDP: Packet with port number<br>
  IP: Packet(sender, receiver and data)<br>
  DNS: Protocol(domain name to IP address lookup), runs on UDP normally, a 2-byte length field is prepended to each DNS message so that the server or client can tell which part of the byte stream is which message. <br>

  <h2>Build Connection</h2>
    To establish a TCP connection, there should be a client and a server (ignoring the simultaneous case). The server waits for the client at a specific address (IP + port), this step is called bind & listen. Then the client can connect to that address. The “connect” operation involves a 3-step handshake (SYN, SYN-ACK, ACK).

  <h2>Socket</h2>
  Listening socket
    <li> bind & listen </li>
    <li> accept </li>
    <li> close </li> 
  Connection socket
    <li> read </li>
    <li> write </li>
    <li> close </li>
  <h2>Sync vs Async</h2>
    Most Node.js library functions related to IO are callback-based or promise-based.
    Promises can be viewed as another way to manage callbacks. These are also described as
    asynchronous, meaning that the result is delivered via a callback. These APIs do not block
    the event loop because the JS code doesn't wait for the result; instead, the JS code returns
    to the runtime, and when the result is ready, the runtime invokes the callback to continue
    your program.
    The opposite is the synchronous API, which blocks the calling OS thread to wait for the
    result. 
</details>
<br>
<h1>Promises and Events</h1>
<details>
  The advantage of promise-based APIs is that you can
  await on them and get the result, thus avoiding breaking your program into tiny callbacks
  that scattered all over the place.

  <h2>Backpressure in TCP: Flow Control</h2>
      <li>
        The consumer's TCP stack stores incoming data in a receive buffer for the application
  to consume. 
      </li>
      <li>
        The amount of data the producer's TCP stack can send is bounded by a window known
  to the producer's TCP stack, and it will pause sending data when the window is full. 
      </li>
      <li>
        The consumer's TCP stack manages the window; when the app drains from the
  receive buffer, it moves the window forward and notifies the producer's TCP stack to
  resume sending. 
      </li> 

  <h2>Promise vs. Callback</h2>
      <li>
        If you stick to promises and async/await, it's harder to create the kind of race conditions
  described above because things happen in order. 
      </li>
      <li>
        With callback-based code, it's not only harder to figure out the order of code execution, it's also harder to control the order. In short, callbacks are harder to read and
  more error-prone to write. 
      </li>
      <li>
        Backpressure is naturally present when using the promse-based style. This is similar
  to coding with blocking IO (which you can't do in Node.js). 
      </li> 
</details>
<br>
<h1>Network Protocol</h1>
<details>
  <h2>Step 1: The Server Loop</h2>
  At a high level, the server should be a loop.
    <li>
      Parse and remove a complete message from the incoming byte stream.(append some data to the buffer, continue the loop if the message is incomplete)
    </li>
    <li>
      Handle the message
    </li>
    <li>
      Send the response
    </li>

  <h2>Step 2: Split Messages</h2>
    <li>
      test if the message is complete using the delimiter '\n'
    </li>
    <li>
      make a copy of the message data, because it will be removed from the buffer
    </li>

  <h2>Step 3: Handle Requests</h2>
    Requests are handled and responses are sent in the server loop
  
  <h2>Optimization</h2>
    <h3>Pipelined Request</h3>
      Consider a typical modern web page that involves many scripts and style sheets. It takes
      many HTTP requests to load the page, and each request increases the load time by at least
      one roundtrip time (RTT). If we can send multiple requests at once, without waiting for
      the responses one by one, the load time could be greatly reduced. On the server side, the
      server shouldn't tell the difference because a TCP connection is just a byte stream.
      <br><br>
      A caveat about pipelining requests: pipelining too many requests can lead to deadlocks;
      because both the server and the client can be sending at the same time, and if both their
      send buffers are full, it's deadlocked as they are both stuck at sending and cannot drain the
      buffer.
    <h3>Removing Data from the Front</h3>
      There is still O(n<sup>2</sup>) behavior in our buffer code; whenever we removed a message from the
      buffer, we moved the remaining data to the front. This can be triggered by pipelining many
      small messages.
      To fix this, the data movement has to be amortized. This can be done by deferring the data
      movement. We can keep the remaining data temporarily in place until the wasted space in
      the front reaches a threshold (such as 1/2 capacity).

</details>
<br>
<h1>HTTP Semantics and Syntax</h1>
<details>
  <h2> High-Level Structures</h2>
    <h3>Request</h3>
      <li>
        The method, which is a verb such as GET, POST
      </li>
      <li>
        The URI
      </li>
      <li>
        A list of header fields, which is a list of key-value pairs
      </li>
      <li>
        A payload body, which follows the request header. Special case: GET and HEAD
        have no payload.
      </li>
    <h3>Response</h3>
      <li>
        A status code, mostly to indicate whether the request was successful
      </li>
      <li>
        A list of header fields
      </li>
      <li>
        An optional payload body
      </li>

  <h2>Header</h2>
    The most important header fields are Content-Length and Transfer-Encoding.
    <br><br>
    Both a request and a response consist of 2 parts: header + body. They are separated by
    an empty line. A line ends with '\r\n'. So the header ends with '\r\n\r\n' including the
    empty line. That's how we determine the length of the header.
    <br>
    The length of the body is complicated because there are 3 ways to determine it. The first
    way is to use Content-Length, which contains the length of the body.
    <br>
    Some ancient HTTP/1.0 software doesn't use Content-Length, so the body is just the rest of
    the connection data, the parser reads the socket to EOF and that's the body. This is the
    second way to determine the body length. This way is problematic because you cannot tell
    if the connection is ended prematurely.
    <br>
    The third way is to use Transfer-Encoding: chunked instead of Content-Length. This is called
    chunked transfer encoding. It can mark the end of the payload without knowing its size in
    advance.
    This allows the server to send the response while generating it on the fly. This use case is
    called streaming. An example is displaying real-time logs to the client without waiting for
    the process to finish.

  <h2>HTTP Message format</h2>
    The HTTP message format is described in a language called BNF.
    An HTTP message is either a request message or a response message. A message
    starts with either a request line or a status line, followed by multiple header fields, then an
    empty line, then the optional payload body. Lines are separated by CRLF, which is the
    ASCII string '\r\n'. The BNF language is much more concise and less ambiguous than
    English.

  <h2>Idempotence</h2>
  An idempotent operation is one that can be repeated with the same effect. This means that
  you can safely retry the operation until it succeeds. For example, if you rm a file over SSH
  and the connection breaks before you see the result, so the state of the file is unknown to
  you, but you can always blindly rm it again (if it's really the same file).

</details>