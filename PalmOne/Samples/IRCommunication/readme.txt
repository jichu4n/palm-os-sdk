---------------------------------------------------------------------------
Name

  IRCommunication: 

  IRFramer
  IRFramerZire71
  IRUnframer
  IRReceiverRaw
  IRTransmitterRaw
  IRCOMMReceiver
  IRCOMMTransmitter

---------------------------------------------------------------------------
Description

  This sample code shows how to use the Serial Manager to transmit and receive 
  Raw IR Data and IR COMM data. This sample code also shows how to wrap the 
  Raw Data in an IrDA packet and how to unwrap the Raw Data from an IrDA packet.

  IRFramer          : Wraps data in an IrDA packet and transmits it using 
                      the Raw IR Port.
  IRFramerZire71    : Wraps data in an IrDA packet and then splits the IrDA 
                      packet into four parts - Header, Payload, CRC and 
                      Trailer and then transmits the four parts, separately, 
                      using the Raw IR Port.
  IRUnframer        : Unwraps data from an IrDA packet after receiving the 
                      IrDA packet from the Raw IR Port.
  IRReceiverRaw     : Receives data from the Raw IR Port.
  IRTransmitterRaw  : Transmits data using the Raw IR Port.
  IRCOMMReceiver    : Receives data from the IR COMM Port.
  IRCOMMTransmitter : Transmits data using the IR COMM Port.

  Features:

  - Transmit data using the Raw IR Port.
  - Receive data from the Raw IR Port.
  - Wrap data in an IrDA packet and transmit it using the Raw IR Port.
  - Unwrap data from an IrDA packet after receiving the IrDa packet from the Raw IR Port.
  - Support for transmitting IrDA packets on a Zire 71
  - Transmit data using the IR COMM Port.
  - Receive data from the IR COMM Port.

---------------------------------------------------------------------------
Builds With

  CodeWarrior
  Palm OS Developer Suite
  gcc

---------------------------------------------------------------------------
Devices

  All Palm devices

---------------------------------------------------------------------------
Requirements


---------------------------------------------------------------------------
Libraries Used

  Serial Manager

---------------------------------------------------------------------------
How to Run

  Example: IR Raw Receiver/Transmitter: 

  1. Launch RawIR Receiver on a device
  2. Launch RawIR Transmitter on another device
  3. Adjust the baud rate (2400, 9600 etc) to be 115200 on both devices.
  4. Tap on 'Receive Raw IR' button in the RawIR Receiver application
  5. In the RawIR Transmitter application, input some data in the first field
  6. Tap on the 'send Raw IR' button in the RawIR Transmitter application. 
     In the Debug field of the receiving device, it should say 'Received
     x bytes...' where x is the number of characters entered in the first
     field

---------------------------------------------------------------------------
Note

  1. The code no longer uses StrLen to determine packet length and thus avoids 
     truncating the packet if there is a NULL character in the payload.

  2. For the implementation used by IRReceiverRaw and IRTransmitterRaw,  
     transmitted data may be corrupt the first time the applications are
     run. This behavior is expected. A workaround would be to packetize the
     data, f.ex. by using the IRComm protocol.

---------------------------------------------------------------------------