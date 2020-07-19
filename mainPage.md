@mainpage Protocol Stack
@tableofcontents

# Sumary
This is a simple and dynamic protocol specificaton.
There are two ways of implementing this protocol.
 - Use the API.
 - Write the code to deal with the packets.


# Revision history
|Date             |Author             |Details                                                                             |
|:--              |:--                |:--                                                                                 |
|2020-Jul-18      |Douglas Reis       |First release                                                                       |

# Using the API
 The fastest and easiest solution it's to use the API. It implents all the rules and assure the packet integrity.
 ## API example
 @copydoc API
 ## Full API documentation
 - @subpage API "Library API"

# Implementing the protocol
    In case you are crazy or have strong reasons to implement the protocol by yourself, this section provides details.
    There are two layers composing the stack, Transport Layer and Enlace Layer.
    Transport Layer is lowest layer and is responsable for sending and receiving the packets with integrity assured.
    Enlace Layer is over Tranporte Layer whitch is responsable for elaborate the purpose and sequence of each packet sent and received.

## Transport Layer
   @copydoc transportFrame
   @image html Protocols-TransportProtocol.jpg "Transport Flow"

## Enlace Layer
   @copydoc PacketFrame
   @copydoc PacketType
   @image html Protocols-EnlaceProtocol.jpg "Enlace Flow"
   @copydoc StatusCode

## Command/Response list   
   @copydoc Commands

## Event list
   @copydoc Events
