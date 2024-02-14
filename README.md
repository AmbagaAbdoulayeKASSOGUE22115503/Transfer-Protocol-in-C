# Transfer-Protocol-in-C

# DESCRIPTION:
  This project aims to implement different data transfer methods, including the Stop-and-Wait with negative acknowledgment/unconnected mode method, the Stop-and-Wait ARQ/unconnected mode method, the Go-Back-N       ARQ/unconnected mode method, and the Go-Back-N ARQ/connected mode method. Each method is designed to ensure reliable and efficient data transfer in various contexts.
  
# Installation:
  1• Clone this repository to your local machine.
  2• Ensure you have the required dependencies installed.
  3• Compile the code according to the specific instructions for each transfer method.

  # USAGE:
    1• make tdd(number of protocol); for example : make tdd1 for the protocol 1(Stop-and-Wait with negative acknowledgment/unconnected mode). This will generate the v1 protocol executables in the bin/ folder.
    2• In a terminal, launch the receiver bin/recepteur, then, in another terminal, start the transmitte bin/emetteur.
    3• Check if the transfer was successful (result in folder fichiers/, the name of the transmitted file is specified in config.txt).
    4• Change the error and loss rate in config.txt and redo steps 2 and 3.

  # PROJECT STRUCTURE:
    # v1: 
      • Flow control in Stop-and-Wait mode;
      • Error detection based on a checksum. This sum will be calculated by applying the “exclusive or” operator (XOR) on three bytes of the header (type, num_seq, lg_info) as well as on all the data bytes of
      the packet. If no errors are detected by the receiver then a positive acknowledgment will be returned (ACK type), otherwise a negative acknowledgment (NACK type).
      In this version we will only consider bit errors on data packets (DATA type), not on acknowledgment packets.
  
    # v2: 
      Loss management will be added to this protocol (in addition to bit errors), with “Stop-and-Wait” error recovery. Unlike the previous version, retransmission will take place following the expiration of a
      timer (no negative acknowledgment).
      Concerning bit errors, as previously, we will only consider errors on data packets (DATA type), not on acknowledgment packets.
    
    # v3: 
      In this protocol, you must implement a look-ahead window with a strategy “Go-Back-N” type retransmissions. The dialing capacity will be 16 and the transmission window will have a size of 7 by default. This
      size can be modified when calling the transmitter program by passing the size of the window as a parameter (which must in all cases be strictly lower than the counting capacity). Example with a transmission
      window of size 4:
      $ bin/emetteur 4
    
    # v4: 
      The previous protocols are in so-called “unconnected” mode, a single phase data transfer is programmed. In this protocol version, we will consider a connected mode which includes three successive phases:
      establishment of the connection, data transfer and connection release. To do this, you will use a variation of access primitives to the application layer: de_application_mode_c() and
      vers_application_mode_c(). These two functions include an additional parameter compared to the previous version: the transport service requested or provided to the application layer. This service can be: a
      request for connexion(T_CONNECT), a transfer of a data unit (T_DATA) or a disconnection request (T_DISCONNECT). To check the input and output file you can run the command ./check_files.

# CONTRIBUTIONS:
  Contributions are welcome! If you'd like to contribute to this project you can send me a message to my discord account ( aak9870 )  .
  
  
