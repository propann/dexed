>This page is about the next/upcoming version of MDT with a capacitive touch display instead of the previous resistive touch. The touch input quality is enhanced drastically by this change.
    
<p> 
<br>
  Until this transition is complete, the full, previous build instructions are (and will stay there) here:
<br>
https://codeberg.org/positionhigh/MicroDexed-touch/wiki/Build-Instructions

<p> 
<br>
For the moment, this page will only document the changes that are in progress.
<p> 
<br>

This update will feature a  better display and touch input at a similar price. This display offers capacitive touch and is MUCH more responsive as the current default display. 
<br>However, so far only one two source of this display type is available and the order amount seems to be limited to one screen only per order. (might have changed in the last weeks)
<br>
Display sources:
<br>
https://www.aliexpress.com/item/1005005926026997.html?spm=a2g0o.cart.0.0.4c6e4ae4FvwMuX&mp=1
<br>
https://a.aliexpress.com/_mtbH3VY
<br>
Note: It is possible to modifiy the previous mdt pcb to fit this display. However this requires cutting 2 traces that are below the display connector on the mdt pcb. I do not suggest to make this modification if you have not somewhat advanced skill in desoldering and soldering components with multiple pins.

It will take some time to figure out software and hardware changes and the waiting for the arrival of a new pcb test version. I estimate this can take severals weeks or even months, so progress on this topic is estimated about end of 2023. 

<br>From the software side, this display, including touch is already working, at least in a basic way. Some color inverting issues and the necessary changes for the new touch are already in the downloadable source. There will be further fixes and enhancements when the first prototype pcb will arrive. 

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV3_Capacitive_Touch/MD_Capacitive_Touch_pcb_overview.png" >
<br>
Updated PCB front and backside
<p>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV3_Capacitive_Touch/lid_nudge.png" >
<br>
Lid-Changes
<p>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV3_Capacitive_Touch/screw_offset.png" >
<br>
Lid-Changes
<p>


<p><br>
<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV3_Capacitive_Touch/LID-capacitive_touch.png" >
<br>

STL - CASE LID (capacitive touch): 
<br>   
https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV3_Capacitive_Touch/LID-capacitive_touch.stl   
<p><br> 

The bottom part of the enclosure is supposed to be the same as it was before. You can find it in the previous full build guide:
<br>
https://codeberg.org/positionhigh/MicroDexed-touch/wiki/Build-Instructions
<p><br> 

<p>

<img src="https://codeberg.org/positionhigh/MicroDexed-touch/raw/branch/main/doc/Build_InstructionsV3_Capacitive_Touch/lightbleed.jpg" >
<br>
I think the tightness of the top lid is very good. There is only a small strip of light coming in where the nudge for the ribbon connector is located. I am sure this can be further enhanced but for now i will not put a priority there.
<p>

