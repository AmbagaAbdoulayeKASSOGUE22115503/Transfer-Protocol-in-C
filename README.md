# Transfer-Protocol-in-C

-DESCRIPTION:
  This project aims to implement different data transfer methods, including the Stop-and-Wait with negative acknowledgment/unconnected mode method, the Stop-and-Wait ARQ/unconnected mode method, the Go-Back-N ARQ/unconnected mode method, and the Go-Back-N ARQ/connected mode method. Each method is designed to ensure reliable and efficient data transfer in various contexts.
  
-Installation:
  1• Clone this repository to your local machine.
  2• Ensure you have the required dependencies installed.
  3• Compile the code according to the specific instructions for each transfer method.

  -USAGE:
    1• make tdd(number of protocol); for example : make tdd1 for the protocol 1(Stop-and-Wait with negative acknowledgment/unconnected mode). This will generate the v1 protocol executables in the bin/ folder.
    2• In a terminal, launch the receiver bin/recepteur, then, in another terminal, start the transmitte bin/emetteur.
    3• Check if the transfer was successful (result in folder fichiers/, the name of the transmitted file is specified in config.txt).
    4• Change the error and loss rate in config.txt and redo steps 2 and 3.
  
  
