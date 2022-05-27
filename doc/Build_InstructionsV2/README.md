**UPDATE 27/05/2022:**

these are the build instructions for the newest version, using low height pin socket rows, instead of IC sockets.

The matching BOM will be updated/compiled in the next days. 

Until then you can use the current instructions or wait a little more. The improvements for this version are mostly of cosmetic nature so there will be no functional difference. <br>
**However be warned that it will not be trivial to convert to the next version. It is possible but requires some time/effort and desoldering tools to swap sockets and headers later.**

Advantages :

1.  Device height reduced 2mm: Faster 3d print times, a little less bulky looking device.
2.  no double / stacked special connector for the display, replaced by 1 standard connector.
3.  much stronger pin binding for teensy and audio board to the PCB.
4.  no more pin bending tricks required, pin locations are exactly where they are supposed to be.

-----
<p>
MicroDexed-touch is still in early development. However, these instructions should not change much in general in their steps. Some building/case parts and "look" might change rapidly. 

<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/front.png" >
<br>
PCB Front
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/back.png" >
<br>
PCB Backside
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/resistors.png" >
<br>
Begin with the resistors. You need 4, the values are printed on the board and are also in the BOM. 2x 47ohm, 1x 220ohm, 1x470ohm. It does not matter, in which (horizontal) direction you put them in. At the botton, put in the 1N4148 diode. Make sure the cathode is pointing to the left side.
    
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/6n138.png" >
<br>
The 6n138 Optocoupler for MIDI. Make sure Pin 1 is on the top left. Your chip will either have a notch at the top side or a circle marking at the first pin. You can use an old-school socket, solder it in directly or use a socket with (round) precision pins. All of these 3 methods should work fine.
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/jumpers.png" >
<br>
Nothing special about the jumpers. Solder in for all of them 2 pin horizontal pin headers. For the 2 Encoder connections, it is advised to use vertical pin headers. 
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/special_headers1.jpg" >
<br>
    For the Teensy and the Audio Board you have to use the old-school IC sockets with pins from both sides. Do not get precision IC Sockets with round pins, the Standard Pins will not fit in there.
    Aside from that, this is the most tricky part to do. You will find that beside the Teensy IC Socket, there is not enough space to put in the 4-5 pin socket for USB HOST.
    
The pins must be bend to fit in. See the following 3 pictures how to get this done. It is not difficult but it will take some time to fit them in for the socket and for the connector pins, coming from the teensy.
    For the audio out pins socket, there is nothing special. There is more than enough space, to put them in without any issues. Make sure that you are using pin headers that are as the same height as the socket.
    <br>
    The part used is:<br>
    https://www.reichelt.de/buchsenleiste-2-54mm-1x5-gerade-bkl-10120967-p266692.html?&nbc=1
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/special_headers_board1.JPG" >
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/special_headers_board2.JPG" >
<p>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/bend.png" >
<p>
    Bend the pins of the Header on the PCB to the left and on the connector pins on the teensy to the right.
    They then should match up the the center, as there would have been no offset, caused by the socket.
    <p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/teensy_top.JPG" >
<p>
Make sure that you put the short side of the standard headers downwards to the sockets. Same for the Audio Board.
    <p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/teensy_back.JPG" >
<p>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/usbhost.jpg" >
<br> The additional 5 pin row is for connecting USB Host. Use the same type of pin headers as for the 2 main (longer) rows and the Audio Board.
<p><br>
    Display
    <p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/special_headers2.jpg" >
<br>
The Display connector currently is a stack of two connectors of this "half" height connector. It is listed in the BOM.
<br>
You can put the second connector on the pin header of the display or also on top of the first connector you solder on to the PCB. <br>
        From experience, when you take the display off, the 2nd connector keeps being attached to the display pin header. 
        <br>So there is nothing special to concider, when re-connecting or disconnecting the display, for example when changing the SD Card.
<br>
<p>
    <br>
    Audio Board
    <p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/flash.jpg" >
    <br>
    <p>
    It is suggested to first put on the flash chip since this required the most attention.
        <br>
        It is possible to add it afterwards, however it might be a little more difficult then to reach the small pins of it. 
        <br>
    If you already have experience how to solder SMD parts, do what works for you best.
        <p>
    However if you have not soldered small pins like this before, the suggestion is to first tin all the pads, then place the chip carefully above the tinned pins. 
        <p>
        Then solder only one pin first and check how all the other pins line up to the pads.
            <br>
            Make corrections by resoldering the first pin until it matches the other pads well enough. Then solder all remaining pins, one after another and double check after each steps if the allignment still fits.
            <br>
            In the picture above it does not line up 100%, however it is good enough to work.
            <br>Do not try to make this look better than necessary, overheating and killing the chip is worse than having it aligned "perfectly". 
   <br><p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/audio.JPG" >
    <p>
    Beside the 2 rows of Standard Pins, make sure to add the pins for the audio out like this.
    The shorter pin side goes down to the MicroDexed PCB, the longer pin side goes up to the top side of the Audio Board. The picture shows the bottom side of the audio Board to illustrate the pins - so there you see the inverse of the pin lenghts, with the short pin side upwards.
<p>

<br>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case.jpg" >
    <br>
    This is the bare bottom case. You will find that there is no need to screw in the PCB. With the limiters from the case itself and the height matching to the display, the PCB will fit in 100% and not move in any direction.
<p>
    
3D Print (STL) Files:
    
<br>
      <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case.png" >
<br>
    <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/lid.png" >
<br>
    <br>
    <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/front_update.png" >
<br>
 https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/CASE_BOTTOM.stl
<br>
 https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/CASE_LID.stl
<br>
     Files last updated: 22/05/2022
   <br>
     <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/slicer.png" >
<br>
    If the snap-in case does not fit 100%, check if your slicer has an option as in the screen above.
<p>
     <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/slicer2.png" >
<br>
    If you have problems with sticking the case parts to the build plate, it is recommended to print a brim around it. This helps a lot to not get any lift ups during the print, even without any build plate heating.
    <p>
    The STL Files do not require to be printed with any support structures.
    <p>
        <br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/encoder.jpg" >
    <br>
    Use standard connectors for the encoders. 1 pin is ground, then the 2 encoder pins, then the pin for the button.
<p>
   
   <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/tft.JPG" >
<br>
    Take off the metal lid from the SD Card Reader on the TFT backside. It would fit even keeping it, if you shorten the Teensy Header pins. But since these larger type of SD Card are outdated anyway, it might be easier to just remove it. You can solder it off more careful if you really think you need it back sometime in the future. 
    <br> To avoid any unwanted electrical contact from the bottom side of the TFT to the teensy pin headers, you should put a not conductive layer between them. Electrical tape, 1-2 layers, should be sufficient.
    
<p>
    
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/assembly_final1.jpg" >
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/assembly_final2.jpg" >
<br>
    <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/sdcard.jpg" >
<br>
    <p>
        If you have encoders that have a small PCB attached at the bottom, check out that there is no overlap or collision with the SD CARD from the Teensy.
      <br>
        If it does overlap, you might be able to cut/saw the PCB a bit or just rotate it a few degrees away, like in the picture above.
        <br> Observe that it will not touch the SD CARD when you put everything together.
        <br>Do not put any mechanical stress on your SD CARD or it sooner of later will get a fracture and then become unreliable or stop working at all.
        <br><p><br>
    
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/assembly_final3.jpg" >
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/assembly_final4.jpg" >
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/assembly_final5.jpg" >
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/assembly_final6.jpg" >
    <br>
    Slide the PCB+Teensy assembly in the lower case. You might have to tilt a bit to the right side of the audio/midi connectors.
<p>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/final1.jpg" >
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/final2.jpg" >
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/final3.jpg" >
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/final4.jpg" >
    <br>
    Final Assembly.
<p>


