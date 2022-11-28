# Simple Property Protocol

This protocl defines how properties are read, written, streamed.
SPP is a transport and physical layer agnostic (FIXME) multi-host multi-client protocol.

The motivation for multi-host is allowing a debug host to connect to the network.

The SPP client manages properties and the SPP host is used to request (get, set, and stream) the values.

In a typical application, the SPP client is running at all times and the SPP host connects to the client
whenever it needs to make a request.

