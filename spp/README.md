# Simple Property Protocol

This protocl defines how properties are read, written, streamed.
SPP is a transport and physical layer agnostic multi-host multi-client protocol.

The motivation for multi-host is allowing a debug host to connect to the network.

Protocol word is 16 bits.

## Property Definitions

Field       type
-------     --------------
`id`        `u16`           
`type`      `u8`           
`name`      `char[32]`
`size`      `u8`
`value`     `u8[size]`
`flags`     `u8`

### Property IDs

Properties `0xF000` to `0xFFFF` reserved. Proerty IDs are unique for all properties

### Property Types

Type            size (bytes)
--------------  ------------
`u16`           2
`i16`           2
`float`         4
`bool`          1
`array`         n/a

### Property Names

Each property has a name.

### Property Sizes

Property size is the size of the property value in bytes.

### Property Values

Property data to be read, written, and received. Property values are located in the first N bytes of the packet and padded to a u16 boundary.

### Property Flags

Flag
-----------         --------------------
Readable            can be read
Writable            can be written
Streamable (write)  streamed to client from host
Streamable (read)   streamed to host from client

## Connection Process

1. Client makes connection broadcast
2. Host accepts, registers client connection, sends response. Client registers host address
3. If client type hasn't been deteced yet, requests property list
4. Client provides property list to host
5. Connection is complete

## Messages

Message                             ID          Direction
--------------------------------    ----------- --------------
Connection Request                  `0xA0`      client to all
Connection Response                 `0xA1`      host to client
Host alive                          `0xA2`      host to all
Property definition request         `0xA3`      host to client
Property definition response        `0xA4`      client to host
Property definition                 `0xA5`      client to host
Property read value request         `0xB0`      host to client
Property read value response        `0xB1`      host to client
Property write value request        `0xB2`      host to client
Property write value response       `0xB3`      host to client
Property stream request             `0xB4`      bi-directional
Property stream response            `0xB5`      bi-directional
Property stream                     `0xB6`      bi-directional

### Common header

For messages with a status, it is expected the 8 bit status is located in the least significant bits for the word
following the header. If the message has a token, it will be located in the 8 most significant bits of the word
followng the header.

Word    Bits    Contents
------- ------- ----------------------------------------
0       8-15    Sync `0xCD`
0       0-7     Message id
1       8-15    source address
1       0-7     destination address (0 for broadcast)

### Connection Request broadcast

Word    Bits    Contents
------- ------- ----------------------------------------
2       0-15    device type

### Connection Response

Word    Bits    Contents
------- ------- ----------------------------------------
2       8-15    device type
2       0-7     status

### Host alive broadcast

Header only

### Property definition request

Header only

### Property definition response

Word    Bits    Contents
------- ------- ----------------------------------------
2       8-15    number of properties
2       0-7     status
3       0-15    total bytes for all properties

### Property definition

Word    Bits    Contents
------- ------- ----------------------------------------
2       8-15    property size
2       0-7     properties remaining (when less than `0xFF`)
3       0-15    property id
4       0-15    property type
5..36   0-15    property name
37      0-7     property flags

### Property read value request

Word    Bits    Contents
------- ------- ----------------------------------------
2       8-15    token
2       0-7     reserved
3       0-15    property id

### Property read value response

Word    Bits    Contents
------- ------- ----------------------------------------
2       8-15    token
2       0-7     reserved
3       8-15    value size
3...N+3 0-15    value size / sizeof(u16) padded to boundary

### Property write value request

Word    Bits    Contents
------- ------- ----------------------------------------
2       8-15    token
2       0-7     reserved
3       8-15    value size
3...N+3 0-15    value size / sizeof(u16) padded to boundary

### Property write value response

Word    Bits    Contents
------- ------- ----------------------------------------
2       8-15    token
2       0-7     status

### Property stream request

Word    Bits    Contents
------- ------- ----------------------------------------
2       8-15    token
2       0-7     stream type
3       0-15    property id
4       0-15    interval

### Property stream response

Word    Bits    Contents
------- ------- ----------------------------------------
2       8-15    token
2       0-7     status
3       8-15    stream size

### Property stream 

Word    Bits    Contents
------- ------- ----------------------------------------
2       8-15    token
3...N+3 0-15    value size / sizeof(u16) padded to boundary

## Handshake tokens

Each request is associated with a token to allow for processing and awaiting concurrent messages.
Tokens are generated by the message source. If a token is received before the message timeout, the operation is complete.
Concurrent requests are only allowed after the connection process is complete

## Property Streams

Host or client can initiate a stream. The host can stream a property from a client (successive reads) or a client can stream property updates from a host (successive and predictable property writes).
