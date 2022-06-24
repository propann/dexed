>MicroDexed-touch is still in early development. However, these instructions should not change much in general in their steps. Some building/case parts and "look" might change rapidly.
    
<p> 

<br>
<p>
    
**Build instructions for current version**
    
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/front.png" >
<br>
PCB Front
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/back.png" >
<br>
PCB Back
<p>
<br>
<p>
You can get the PCB from:
<p>
    
https://www.pcbway.com/project/shareproject/MicroDexed_Touch_current_version_with_PCM5102_d643a695.html
<br><p>
<a href="https://www.pcbway.com/project/shareproject/MicroDexed_Touch_current_version_with_PCM5102_d643a695.html"><img src="https://pcbwayfile.s3.us-west-2.amazonaws.com/web/22/06/24/0636194052824.png" alt="PCB from PCBWay" /></a>

<br>
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/resistors.png" >
<br>
        
Let's begin with the resistors. You need 4, the values are printed on the board and are also listed in the BOM. 2x 47 Ohms, 1x 220 Ohms, 1x470 Ohms. For resistors, it does not matter, in which direction you put them in. At the botton, put in the 1N4148 diode. Make sure the cathode is pointing to the left side.
    
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/6n138.png" >
<br>
This is the 6n138 optocoupler for MIDI. Make sure pin 1 is on the top left. Your chip will either have a notch at the top side or a circle mark at the first pin. You can use an old-school IC socket, solder it in directly or use a socket with (round) precision pins. All of these 3 methods should work fine.


**MAIN PIN HEADER / SOCKETS**
    
<br><p>
For the Teensy, the PCM 5102 and the external Flash Chip, we will use pin socket rows instead of IC sockets. The sockets will be soldered to the main PCB.<p>
 <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/pinsandsocket.png" >
    
As the counterpart, we will use traditional rows of pin headers with the pins of the short side of the plastic spacer snipped off. 
<br>We keep only the side with the longer pins.
<br>
Since the top side of Teensy will be blocked by the plastic spacers, you solder the pins on to the bottom side of Teensy. This is the reason, why the pins are seemingly 2-3mm longer than they need to be for the sockets but the soldering will take up (some) of this extra space.<p>

 <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/cut-pins-teensy.png" >
    <br>
 For the Teensy we need 2 pieces of 24 pins.<br>
 
<p>
So for both rows, leave the plastic spacers exactly where they are at factory and snip off the short pin side, directly above the plastic spacer. <p>
This is tedious. Take your time and take breaks when necessary.
<br>
    
Please take care to not hurt yourself or others while you snip the pins. These pins are sharp when cut and will fly off unpredictably if you are not careful. 
<br>Place yourself so that the cut pins will not fly in your direct direction and use eye protection so nothing can happen in this process.

    
<p>
After the 2 long rows are cut like in the pictures above, we will do the same procedure to a much smaller row for the flash board, 2 for the PCM5102 and the USB Host Connector.
<br>Note that we are not connecting the SCK pin for the PCM5102 so no pin/header is required there.
<p>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/jumpers.png" >
<br>
    The Display uses a different kind of connector, do not solder in a low row socket there.
    <br>
    The remaining headers/jumpers are regular pin headers, you do not have to cut them.
    
 
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/encoderconnector.png" >

 <p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/board-with-sockets2.png" >
     
For the 2 encoder connections, it is advised to use vertical pin headers - Also, depending on your encoders, for example if they have a small PCB on their bottom to connect the pins instead directly to the decoder, it might be a good idea to solder the pin headers very lowly as seen in this image.<p>
This way they will use the free space "above" the encoder's PCBs and won't collide with them.  
<p>

**ENCODERS**
<br><p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/encoder.jpg" >
    <br>
    Use standard dupont connectors for the encoders. 1 pin is ground, then the 2 encoder pins, then the the pin for the button.
> If you are using encoders on small pcbs and you experience unwanted "button push" events, try to remove any resistors from the pcbs. They aren't neccessary as debouncing is done in software.
>
<p>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/allsockets.png" >

The small 4 pin row between the 2 large Teensy pin rows is for USB MIDI HOST. Do not forget to put in the socked and also the counterpart pin headers on the Teensy.
<br>
 
Put in the sockets on the PCB and test fit with the Teensy if everything lines up before you solder the sockets and pin rows.
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

 **AUDIO**
<p>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/pcm1.png" >  
<p>
Do the same kind of connector for the PCM5102 Board. Snip of the short side of the header pins. Do no connection for the SCK pin to the main PCB. Instead, close the 2 small pads on the top side to force SCK to ground - This will tell the PCM5102 to run it's own, internal clock signal.
<bp>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/pcm3.png" > 
<p>
      If any of the solder bridge pads on the backside come closed by factory, remove all of them with desoldering wick or your prefered  desoldering tool/method. 
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/pcm2.png" > 
<p>
Front viel with pin headers attached
<br>  
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/pcm4.png" >  
<p> Top view with pin headers attached

<br>  
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/board-with-flash.png" >  
<p> Flash board attached to main PCB
    
<br>  
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/board-with-flash2.png" >  
<p> Flash board attached to main PCB
<p>
<br>    
**FLASH chip**
<p>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/external-flash1.png" >
    
Instead of manually soldering the Flash Chip to the Teensy Audio Board as we did in the previous generation, we go for this plug-and-play, 2$ board that already has a Flash Chip soldered on by factory. So you only need to solder standard size pins/connectors.
Do the same procedure as for the Teensy for the connection to the main PCB. Use pin headers, leave the plastic spacers exactly where they are at factory and snip off the short pin side, directly above the plastic spacer. 
    
     
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/external-flash2.png" >
<p>
    Flash board with pin headers soldered, backside
<p>    
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/external-flash3.png" >
    
    <p>
    Flash board with pin headers soldered, frontside
        <p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/external-flash4.png" >
<p>
Flash board with pin headers soldered, frontside
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
  

   
   <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/bottomcaseandlidwithdisplay.png">
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
     Files last updated: 24/06/2022
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

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/board-assembled1.png" >
<br>
PCB with all on board components ready
<p>
        
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/board-assembled3.png" >
<br>
PCB with all on board components ready
<p>
        
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/board-assembled4.png" >
<br>
PCB with all on board components ready, view from rear connector side
    <p>
        
    
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/final1.png" >
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/final2.png" >
<br>
    Final Assembly
<p>


