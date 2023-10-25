>There is a new version of MicroDexed-touch in early development featuring a capacitive touch display. Progress about this update is located here:
https://codeberg.org/positionhigh/MicroDexed-touch/wiki/MicroDexed-Capacitive-Touch
    
<p> 
25/10/2023:
- [x] This build guide page will be unified for new and old version since the differences are not that big. Some components have changed the location on the PCB and an audio amplifier is added (optional). Further there are 2 optional buttons for soft power and and a program button.

<p> 
17/09/2023:

- [x] Working on an Hardware update with better display but at a similar price. This display offers capacitive touch and is MUCH more responsive as the current default display.
However, so far only one source of this display type is available and the order amount seems to be limited to one screen only per order.
https://www.aliexpress.com/item/1005005926026997.html?spm=a2g0o.cart.0.0.4c6e4ae4FvwMuX&mp=1
Note: It is possible to modifiy the current mdt pcb to fit this display. However this requires cutting 2 traces that are below the display connector on the mdt pcb. I do not suggest to make this modification if you have not somewhat advanced skill in desoldering and soldering components with multiple pins.

- [x] It will take some time to figure out software and hardware changes and the waiting for the arrival of a new pcb test version. I estimate this can take severals weeks or even months, so progress on this topic is estimated about end of 2023.
<br>
    <p> 
### Build instructions
    
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/front.png" >
<br>
PCB Front (Resistive touch)
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/back.png" >
<br>
PCB Back (Resistive touch)
<p>
<br>

<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/front-c.png" >
<br>
PCB Front (Capacitive touch)
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/back-c.png" >
<br>
PCB Back (Capacitive touch)
<p>
<br>

<p>

**If you want to donate to this project, please check out this link:**

<a href="https://liberapay.com/positionhigh/donate"><img alt="Donate using Liberapay" src="https://liberapay.com/assets/widgets/donate.svg"></a>
    
**PCB is available from PCBWAY**

If you register and pay as a new user at PCBWAY with this Invite link, you should get $5 "New User Free Credit" - so you can order 5 PCB pieces for "free", except shipping and customs cost etc.
    
[https://www.pcbway.com/setinvite.aspx?inviteid=565384](https://www.pcbway.com/setinvite.aspx?inviteid=565384)

<a href="https://www.pcbway.com/project/shareproject/MicroDexed_Touch_current_version_with_PCM5102_d643a695.html"><img src="https://www.pcbway.com/project/img/images/frompcbway-1220.png" alt="PCB from PCBWay" /></a>

    
<br><p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/pcb_populated.jpg" > 
<br>
(Resistive touch)
<br>
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/resistors.png" >
<br>
 (Resistive touch)
<br>    
<p>
Let's begin with the resistors. You need 4, the values are printed on the board and are also listed in the BOM. 2x 47 Ohm, 1x 220 Ohm, 1x470 Ohm. For resistors, it does not matter, in which direction you put them in. At the botton, put in the 1N4148 diode. Make sure the cathode is pointing to the left side.
    
<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/6n138.png" >
<br>
 (Resistive touch)
<br>
<p>
This is the 6n138 optocoupler for MIDI. Make sure pin 1 is on the top left. Your chip will either have a notch at the top side or a circle mark at the first pin. You can use an old-school IC socket, solder it in directly or use a socket with (round) precision pins. All of these 3 methods should work fine.

<p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/resistors-c.png" >
<br>
(Capacitive touch)
<br>
<p>
The location of the resistors and the diode is slightly different on the newer capacitive touch pcb. Otherwise, the same instructions are valid as for the previous version.
<br>
<p>
<br>
### MAIN PIN HEADER / SOCKETS
    
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

### ENCODERS
<br><p>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/encoder.jpg" >
    <br>
    Use standard dupont connectors for the encoders. 1 pin is ground, then the 2 encoder pins, then the the pin for the button.
> If you are using encoders that come on a small pcb with resistors located at the bottom side and you experience unwanted "button push" events or other wrong encoder behaviour, try to remove any resistors from the pcbs. In fact it would be even better to remove the whole PCB and just use the encoders alone. This will prevent the collision with the SD Card in the Teensy, located very closely when you do not remove the small PCB. The resistors aren't neccessary as debouncing is done in software and if you keep them, they are known to make make more problems, than they help. When your encoder does not have a common ground (gnd) pin for encoder + button function but has seperate gnd pins for them, you can bridge both gns pins directly at the encoder.
>
<p>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/usbsocket.png" >
<br>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/usbsocket-c.png" >
<br>
The capactive touch pcb is identical, however there is an option to add buttons for soft-power and a program-button. The program-button really is only required when something during firmware flashing goes wrong. In this case it is more convinient that the button is reachable without opening the enclosure. The soft-power is mainly useful when using mdt from an optional battery.

The small 4 pin row between the 2 large Teensy pin rows is for USB MIDI HOST. Do not forget to put in the socked and also the counterpart pin headers on the Teensy.
<br>
 
Put in the sockets on the PCB and test fit with the Teensy if everything lines up before you solder the sockets and pin rows.
<br>

It is a suggestion to solder only the first and last pin, then every 4-5 pin of every row, test if it fits and then solder all remaining pins.
 
<br>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/teensywithheader.png" >
<br>
Teensy with pin headers
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/teensywithheaderandsockets.png" >
<br>
Teensy with pin headers and sockets loosely attached to test fit.
<br><p>

### AUDIO
<p>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/pcm1.png" >  
<p>
Do the same kind of connector for the PCM5102 Board. Snip of the short side of the header pins. Do no connection for the SCK pin to the main PCB. Instead, close the 2 small pads on the top side to force SCK to ground - This will tell the PCM5102 to run it's own, internal clock signal.
<b>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/pcm3.png" > 
<p>
    
If any of the solder bridge pads on the backside come closed by factory, remove all of them with desoldering wick or your prefered  desoldering tool/method. 
<b>
    
> If you get distorted/garbled audio, try to solder the bridges 1,2,4 to LOW , middle pad + right side and leave pin 3 without connection to LOW or HIGH. Pin 3 will be controlled by the Teensy for muting control.
> 
    
<b>  
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/pcm2.png" > 
<p>
    
<br>   
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
    
### FLASH chip
    
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
    
### DISPLAY
    
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
There are various versions/kinds of this display on the market.
If your display is working but your touch screen input is reversed/mirrored on one or both directions, you can change it in system - misc settings.
 
<p>
Take off the metal lid from the SD card reader on the TFT backside. These larger type of SD card are outdated and we already have a MICRO SD Card reader available on the Teensy. 
<br>You can solder it off more carefully if you really think you need it back on sometime in the future. If you are sure you do not need it, you can quickly snip it off at it's 4 holding/soldering pads with a small cutting tool.
<br>

> To avoid any unwanted electrical contact on the bottom side of the TFT to the teensy pin headers, you should put a non conductive layer between them. Electrical tape, 1-2 layers, should be sufficient.
>    


### FIRST TEST
<p>
At this stage, it should be possible to do a first test. 

> You do not need to put in a SD CARD with the preset files copied to it. MicroDexed should at least boot up and the menu/encoders can be tested in the menu - however it will take a few more seconds than usual to start up, if there is no SD CARD with valid files present.
>

<br>
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/finalwithencoders.png" >
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/heightview.png" >
<br>
<p>
    
### CASE/ENCLOSURE
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_0.jpg" >
<br>
    
There are 2 different versions provided for self printing you a simple case for MicroDexed (or you can use your favorite 3d printing service to do that).
<p>
Also you can order a printed SLA case from pcbway:<p>
  
<img src="https://pcbwayfile.s3.us-west-2.amazonaws.com/web/22/10/26/0407304330503.jpg" >
<br>
<img src="https://pcbwayfile.s3.us-west-2.amazonaws.com/web/22/10/26/0406237772058.png" >
<br>
    
[https://www.pcbway.com/project/shareproject/MicroDexed_Touch_MDT_SLA_printed_enclosure_LID_BOTTOM_CASE_317c88e1.html](https://www.pcbway.com/project/shareproject/MicroDexed_Touch_MDT_SLA_printed_enclosure_LID_BOTTOM_CASE_317c88e1.html)
    
<p>Print quality is much higher, then you can expect from a home made PLA or SLA DIY print.<br>
 Since SLA Prints can not be melted by a hot iron, this version has some special modification that will perfectly fit M3 threaded inserts to be superglued in, with 1-2 drops.
<p> The SLA version also has some extra details, that are not possible to print by a PLA printer, like the MDT Logo.   
<p>
    
### 3D PRINTING TIPS - Valid for both case versions
    
<p>
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/slicer.png" >
<br>
    If the case dimensions do not fit 100%, check if your slicer has an option as in the screen above.
<p>
     <img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/slicer2.png" >
<br>
    If you have problems with the case parts coming off the build plate during print, it is recommended to print a brim around each part. This helps a lot to not get any lift-ups during the print, even without any build plate heating.
    <p>
    All STL Files, of both versions, will print without any support structures.
    <p>
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
 
### CASE OPTIONS
    
The first case option described here is the improved version, featuring threaded inserts for M3 screws. This makes the overall stability and handling much better since you can open/close the device as many times as you like, without having to rely only on the printed snap-in clips to hold it together.
<p>
Since it is easier to build the previous version with the snap-in clips only, that is still available as an option further down this page.
It is however recommened to build the threaded version, even it needs some more parts and also more effort to put together but it is well worth it.
<p>
To make the instructions less confusing, photos on this page showing a black case are for the snap-case, the photos with the white case are for the improved, threaded inserts version.
    
    
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_1.jpg" >
<br>
    
### Threaded Inserts Case
    
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_2.jpg" >
<br>
This case has 4 mounting posts, 1 at each corner of the device. 
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_3.jpg" >
<br>
These are the inserts we are using, the M3 x 5.7 x 4.6         
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_3b.jpg" >
<br>
Maybe it is possible you can get a smaller ammount of them (since you only need 4 pieces) but it might easier to get a full set like this from Amazon, ebay etc. If you do any kind of 3d printing, this is a package you can use for a lot of other projects, too. I will add links to a set like this in the BOM.            
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_4.jpg" >
<br>
Close up mounting post                 
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_5.jpg" >
<br>
Each insert has 3 "zones". The zone with the smallest diameter will fit into the case like this.                    
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_6.jpg" >
<br>
To drive the insert into the case, you basically heat up your soldering iron to 175-180 degree celsius.
<br>
    
> If there is left residue of solder on your iron, heat it up to your usual soldering temerature first, and clean the iron tip as good as possible. It must be avoided that you put solder in side of the inserts, by accident.
    
When the soldering iron tip is clean of solder, switch down to a temperature of 175-180 degree celsius and give it enough time to "cool off".
Then push in the insert slowly with the tip of the iron.
<br>
This is a matter of maybe 4-5 seconds. Do not push it in to deeply - it should be flush with the top surface of the mounting post.
    
It should be possible to make some corrections by re-heating but if you haven't any experience with this procedure, it is recommended to practice with at least 1 other printed 3d part (maybe you have some failed prints or test prints from this or an other project).
<br>
The printing time of the bottom case for Microdexed is around 4 hours. So it is a good idea, to have some practice on an other print object, first.
    
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_7.jpg" >
<br>
The insert should look similar to this after melting it in.                            
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_8.jpg" >
<br>
This is the case with all 4 inserts fitted.                                
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_9.jpg" >
<br>
For the screw part you can use M3 x 4 or M3 x 5 screws. It is possible to even use much longer M3 screws - but that only makes sense if you have such already available in front of you - otherwise it is better to go with the M3 x 4.
As a side topic, for the screen attachment, we will use the M3 x 6.
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_10.jpg" >
<br>
Various M3 screws from x4 - x24 as a kit from Amazon.                                      
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_11.jpg" >
<br>
The head part of the M3 should be flush with the top of the device, after screwed in.                                           
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_12.jpg" >
<br>
All case screws in place                                             
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_13.jpg" >
<br>
This is the PCB inside the threaded inserts case.
<br>
Notice that the 2 encoders used in the picture had to be rotated a bit, to make space for the mounting posts.
<br>
Depending of the Encoders you are using, this might be necessary, or not.
    
If it seems not to fit, even by rotating: If they come with a small attached PCB, like in the picture, it might be an idea to saw off some empty space from it or remove the small PCB below the Encoder, altogether.
<br>
If you decide to saw off something, carefully check if there are traces on the top or bottom side, that are required. In the sample picture, i have manually connected a ground wire since i saw off the part of it's pcb trace.                                                  
<p><br>
    
Files last updated: 13/07/2023
<p><br>
STL - CASE BOTTOM (threaded insert version):
<br>
https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/CASE-BOTTOM-Threaded-Inserts.stl   
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_14.png" >
<br>
<p><br>
STL - CASE LID (threaded insert version): 
<br>   
https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/CASE-LID-Threaded-Inserts.stl   
<p><br> 
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_15.png" >
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_16.png" >
<p><br>
STL - KNOB (two of them are needed):
<br>
  https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/CASE-KNOB.stl
  
<br>    

-----
### CASE MODIFICATION (for easy SD CARD access)
<p> 
This section is about a modified LID part, there is no change required for the bottom part of the enclosure.
<p> 
This modification looks a bit hacked and it is, however it is tested about several month now and if you do not care so much about the design or looks, this hack is very useful. There is about no reason to open the enclosure ever again, if you use this lid.

<p>
On our youtube playlist you find a video covering this topic at:
<br>
https://www.youtube.com/watch?v=yFXT1o5kOMk
<p>
There is a about of 1-2mm tolerance about the bottom position of the card slot entry. This is by design since we do not know exactly about the total heigth your teensy is in the low profile socket. That really depends on your soldering style.
<p>
If you find the bottom position to be too high or low in general, you can edit the .stl file for example with blender to fit your unit. Or let us know in discord chat what you need and we will provide some additional version with maybe 1 or 2 mm height adjustments.

<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case-sdmod-lid.png" >
    <br>
    <br>
 https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/CASE-SDMOD-LID.stl
<br>


<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case-sdmod-lid-cover.png" >
    <br>
    <br>
 https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/CASE-SDMOD-LID-COVER.stl
<p><br>

-----


<p><br> 
        
### OLD CASE (without Threaded Inserts, deprecated)
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
  
   <br>
  
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/assembly_final3.jpg" >
<br>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/assembly_final6.jpg" >
    <br>
    Slide the PCB+Teensy assembly in the lower case. You might have to tilt a bit to the right side of the audio/midi connectors.
<p>
<br>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/pcb_populated.jpg" >
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


-----

          
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/final1.png" >
<br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/final2.png" >
<br>
    
### Final Assembly (depreciated case)
<p>
<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV2/case_1.jpg" >
<br>
    
### Final Assembly (Threaded Inserts Case)

<p>
