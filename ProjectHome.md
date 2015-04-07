17.06.2014

TI announced a new generation of embedded WiFi Chips.
Since the CC3000 was broken from the start and is not recommended for new designs,
this code will not be updated in the future and will only have reference value.

I will not develop any code for the new CC3100 and CC3200 Chips or anything that follows.

The announcement can be read here:
http://e2e.ti.com/support/wireless_connectivity/f/851/t/348514.aspx
(the TI forum is also a good indicator for why I will not use it anymore and do not recommend the CC3000 for anything)



Original Text:

The CC3000 http://processors.wiki.ti.com/index.php/CC3000 is a Wi-Fi 802.11b/g Network Processor from Texas Instruments. The modified API allows quasi non-blocking behavior for work with RT OS and/or watchdog timers etc.

It is build with arm-none-eabi-gcc v 4.7.3

The CC3000 should have firmware 1.11 for server mode (sockets) there can be issues with socket listen, select, accept and bind otherwise...

Some info:
If an interrupt occurs, then the data is being received and handed over to the event handler. The event handler checks the affiliation of the data (e.g. if it is unsolicited) and it checks if the data is what we have been waiting for.

I have also introduced a flow\_handler. This completely handles outgoing and incoming data and also the different states of the CC3000. All executive actions are handled there.
In the main loop we only set the next action to execute and perhaps handle some of the payload data.(e.g. with recv and send)

There are two important variables: cc3000\_desired\_state and cc3000\_current\_state
Everytime a command is sent and we expect a return, then cc3000\_desired\_state is incremented. If we then received the desired code, cc3000\_current\_state is incremented.
If desired state equals current state, then cc3000\_is\_ready() will return 1 and the next state is set after we processed the data.

At the moment I check for the isr and also if the irq pin is low in the main loop. The goal is to only check for the isr flag, but due to some strange timing things in the CC3000 I have to check for both, because for some reason if I do not print out the debug strings, then the initialisation goes wrong.
(The code also works if there is no interrupt at all and the pin is just polled in the main loop)

At any time there will be no wait states. After a command has been executed the program will immediately go back to the main loop. There are more functions (we need a function to request data and one to handle the received payload) and more buffers (which can be reduced). But overall there is more control over what is going on.