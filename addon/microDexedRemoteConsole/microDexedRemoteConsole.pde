
import processing.serial.*;

Serial myPort;  // Create object from Serial class
int xh, xl; // x high byte, low byte
int yh, yl; // y high byte, low byte
int hh, hl; // height
int wh, wl; // width
int colh, coll; //color high byte, low byte
int val1, val2; //val1 is always 90 as a start command, val2 is the drawing function
int check; // minimal check value - should always return 88 when command was transmitted correctly
int col, r, g, b;
color c;

void setup()
{
  size(960, 640);
  String portName = Serial.list()[1];
  myPort = new Serial(this, portName);
  myPort.buffer(116);
}

void readxy()
{
  xh = myPort.read();
  xl = myPort.read();
  yh = myPort.read();
  yl = myPort.read();
}

void readcolor()
{
  colh= myPort.read();
  coll= myPort.read();
  col =  colh*256+coll;
  b = (((col) & 0x001F) << 3) & 0xFF;
  g = (((col) & 0x07E0) >>> 3) & 0xFF;
  r = (((col) & 0xF800) >>> 8) & 0xFF;
  color c = color(r, g, b);
  fill(c);
  noStroke();
}

void draw()
{
  while (myPort.available() > 0)
  {
    val1 = myPort.read();

    if (val1==99) // all commands start with 99
    {
      val2 = myPort.read();
      if (val2==90) // draw pixel (draw commands are from 90 - 94)
      {
        readxy();
        readcolor();
        check= myPort.read();
        if (check==88)
          rect((xh*256+xl)*2, (yh*256+yl)*2, 2, 2  );
      } else if (val2 == 91)  //draw vline
      {
        readxy();
        hh = myPort.read();
        hl = myPort.read();
        readcolor();
        check= myPort.read();
        if (check==88)
          rect((xh*256+xl)*2, (yh*256+yl)*2, 2, (hh*256+hl) *2 );
      } else if (val2==92) // draw hline
      {
        readxy();
        wh = myPort.read();
        wl = myPort.read();
        readcolor();
        check= myPort.read();
        if (check==88)
          rect((xh*256+xl)*2, (yh*256+yl)*2, (wh*256+wl) *2, 2  );
      } else if (val2==93)  // fill screen
      {
        readcolor();
        check= myPort.read();
        if (check==88)
          rect(0, 0, 960, 640);
      } else if (val2==94)  //fill rect
      {
        readxy();
        wh = myPort.read();
        wl = myPort.read();
        hh = myPort.read();
        hl = myPort.read();
        readcolor();
        check= myPort.read();
        if (check==88)
        {
          rect((xh*256+xl)*2, (yh*256+yl)*2, (wh*256+wl) *2, (hh*256+hl) *2  );
        }
      }
    }
  }
   if (keyPressed) 
  saveFrame("microdexed-######.png");
}
