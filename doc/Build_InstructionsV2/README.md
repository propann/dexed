**UPDATE 27/05/2022:**

These are the build instructions for the newest version (V2), using low height pin socket rows instead of IC sockets.
The improvements for this version are mostly of cosmetic nature so there will be no functional difference.
<br>
**However be warned that it will not be trivial to convert to this version. It is possible but requires some time/effort and desoldering tools to swap sockets and headers later.**

Advantages :

1.  Device height reduced by 2mm: Faster 3d print times, a little less bulky looking device.
2.  no double / stacked special connector for the display, replaced by 1 standard connector.
3.  much stronger pin binding for teensy and audio board to the PCB.
4.  no more pin bending tricks required, pin locations are exactly where they are supposed to be.
<br>

<p>
    
> MicroDexed-touch is still in early development. However, these instructions should not change much in general in their steps. Some building/case parts and "look" might change rapidly.
    
<p> 

<br>
<p>
    
**Build instructions for newest version (V2)**
    
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/front.png" >
<br>
PCB Front
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/back.png" >
<br>
PCB Back
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/resistors.png" >
<br>
Begin with the resistors. You need 4, the values are printed on the board and are also listed in the BOM. 2x 47 Ohms, 1x 220 Ohms, 1x470 Ohms. It does not matter, in which (horizontal) direction you put them in. At the botton, put in the 1N4148 diode. Make sure the cathode is pointing to the left side.
    
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/6n138.png" >
<br>
This is the 6n138 optocoupler for MIDI. Make sure pin 1 is on the top left. Your chip will either have a notch at the top side or a circle mark at the first pin. You can use an old-school IC socket, solder it in directly or use a socket with (round) precision pins. All of these 3 methods should work fine.
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/jumpers.png" >
<br>
Nothing special about all of the 2-pin jumpers. Solder 2 pin horizontal pin headers for all of them. 
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/encoderconnector.png" >

For the 2 encoder connections, it is advised to use vertical pin headers - Also, depending on your encoders, for example if they have a small PCB on their bottom to connect the pins instead directly to the decoder, it might be a good idea to solder the pin headers very lowly as seen in this image.<p>
This way they will use the free space "above" the encoder's PCBs and won't collide with them.  
<p>

<br>

**Audio Board FLASH chip**
<p>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/flash.jpg" >
    <br>
    <p>
It is suggested to put on the flash chip first since this requires most attention and anything soldered before will get in the way.
        <br>
        It is possible to add it afterwards, however it might be a little more difficult to reach its small pins. 
       <br>
    If you already have experience how to solder SMD parts do what works best for you.
        <p>
    However if you didn't solder small pins like this before, the suggestion is to first tin all the pads, then place the chip carefully above the tinned pins. 
        <p>
        Then solder only one pin first and check how all the other pins line up to the pads.
            <br>
            Make corrections by resoldering the first pin until the other pins match their pads well enough. Then solder all remaining pins, one after another and double check after each steps if the allignment still fits.
            <br>
            In the picture above it does not line up 100%, however it is good enough to work.
            <br>Do not try to make this look better than necessary, overheating and killing the chip is worse than having it aligned "perfectly". 
   <br><p>
   <br>
<p>
    
**MAIN PIN HEADER / SOCKETS**
    
<br><p>
For the Teensy and for the Audio Board, in this build we will use pin socket rows instead of the old-school IC sockets. The sockets will be soldered to the main PCB.<p>
 <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/pinsandsocket.png" >
    
As the counterpart, we will use traditional rows of pin headers with the pins of the short side of the plastic spacer snipped off. 
<br>We keep only the side with the longer pins.
<br>
Since the top side of Teensy and Audio Board will be blocked by the plastic spacers, you solder the pins on to the bottom side of Teensy and Audio Board. This is the reason, why the pins are seemingly 2-3mm longer than they need to be for the sockets but the soldering will take up (some) of this extra space.<p>

 <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/cut-pins-teensy.png" >
    <br>
 For the Teensy we need 2 pieces of 24 pins.<br>
  
 <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/cut-pins-audio.png" >
     <br>
 For the Audio Board we need 2 pieces with 14 pins.
 
<p>
So for all 4 rows, 2 for the Teensy and 2 for the Audio Board, leave the plastic spacers exactly where they are at factory and snip off the short pin side, directly above the plastic spacer. <p>
This is tedious. Take your time and take breaks when necessary.
<br>
    
Please take care to not hurt yourself or others while you snip the pins. These pins are sharp when cut and will fly off unpredictably if you are not careful. 
<br>Place yourself so that the cut pins will not fly in your direct direction and use eye protection so nothing can happen in this process.

    
<p>
After all 4 long rows are cut like in the pictures above, we will do the same procedure to a much smaller 4 pin row and a 3 pin row.
<p>


<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/allsockets.png" >

The 4 pin row is for USB MIDI and is located at the top between the 2 Teensy pin rows.
<br>
The 3 pin row is for Audio Line Out and is on the right side of the Audio Board.
 
Put in the sockets on the PCB and test fit with the Teensy and Audio Board if everything lines up before you solder the sockets and pin rows.
<br>

It is a suggestion to solder only the first and last pin of every row, test if it fits and then solder all remaining pins.
 
<br>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/teensywithheader.png" >
<br>
Teensy with pin headers
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/teensywithheaderandsockets.png" >
<br>
Teensy with pin headers and sockets loosely attached to test fit.
<br><p>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/audiowithheader.png" >
<br>
Audio Board with pin headers
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/audiowithheaderandsockets.png" >
<br>
Audio Board with pin headers and sockets loosely attached to test fit.
<br>
<p>
<p><br>
    
**DISPLAY**
    
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/displayconnector.png" >
<br>

The display connector now uses a standard part, not the stacked half height connector as in the last version.
<br>
    However we do use them now as sockets for the Teensy + Audio Board instead. (Without any stacking)<br>

<br>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/displayconnector2.png" >
<br>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/displayconnector3.png" >
<br>

The connector should match up with the pin header that the display comes with by default.<br> 
It does not matter if there is a small gap between the lid/display and the pin socket.
<br><p>
> There might be a small offset, depending on 3dprint, how much solder you have used on the audio board pin rows etc - this will mostly be influenced by how tight you screw on the display to the lid in a later step. 
> 
<br>
<br><p>
  
**ENCODERS**
<br><p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/encoder.jpg" >
    <br>
    Use standard dupont connectors for the encoders. 1 pin is ground, then the 2 encoder pins, then the the pin for the button.
<p>
   
   <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/bottomcaseandlidwithdisplay.png" >
<br>
    Take off the metal lid from the SD card reader on the TFT backside. These larger type of SD card are outdated and we already have 2 MICRO SD Card readers available. 
    <br>You can solder it off more carefully if you really think you need it back on sometime in the future. 
    <br>

> To avoid any unwanted electrical contact on the bottom side of the TFT to the teensy pin headers, you should put a non conductive layer between them. Electrical tape, 1-2 layers, should be sufficient.
>    


**FIRST TEST**
<p>
At this stage, it should be possible to do a first test. 

> You do not need to put in a SD CARD with the preset files copied to it. The MicroDexed should at least boot up and the menu/encoders can be tested in the menu.
>

<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/finalwithcomponents1.png" >
<br>
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/finalwithcomponents2.png" >
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/finalwithencoders.png" >
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/heightview.png" >
<br>

**CASE**
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/bottomcase.png" >
    <br>
    This is the bare bottom case. With the spacers of the case itself and the height matching the display's the PCB should not move in any direction when put together.
<p>
    

    
<br>
      <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case.png" >
<br>
    <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/lid.png" >
<br>
3D print (STL) files:
<br>
 https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/CASE-BOTTOM.stl
<br>
 https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/CASE-LID.stl
<br>
  https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/CASE-KNOB.stl
<br>   
     Files last updated: 04/06/2022
   <br>
   <p>
   <br>
     <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/slicer.png" >
<br>
    If the snap-in case does not fit 100%, check if your slicer has an option as in the screen above.
<p>
     <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/slicer2.png" >
<br>
    If you have problems with the case parts coming off the build plate during print, it is recommended to print a brim around each part. This helps a lot to not get any lift-ups during the print, even without any build plate heating.
    <p>
    The STL Files do not require to be printed with any support structures.
    <p>
        <br>
        <p>   
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/lidwithpcb.png" >
<br>
    <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/sdcard.jpg" >
<br>
    <p>
        If you have encoders that have a small PCB attached at the bottom, check out that there is no overlap or collision with the SD CARD of the Teensy.
      <br>
        If they do overlap, you might be able to cut/saw the PCBs a bit or just rotate them a few degrees away, like in the picture above.
        <br> 
        
> Observe that nothing touches the SD CARD when you put everything together.
        <br>Do not put any mechanical stress on your SD CARD or sooner or later it will get a fracture and then become unreliable or will stop working completely.
> 
 

    
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/assembly_final3.jpg" >
<br>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/assembly_final6.jpg" >
    <br>
    Slide the PCB+Teensy assembly in the lower case. You might have to tilt a bit to the right side of the audio/midi connectors.
<p>
<br>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/final1.png" >
<br>
    <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/final2.png" >
<br>
    Final Assembly
<p>


