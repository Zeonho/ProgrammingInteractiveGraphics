/**
    CSci-4611 Assignment #1 Text Rain
**/


import processing.video.*;

// Global variables for handling video data and the input selection screen
String[] cameras;
Capture cam;
Movie mov;
PImage inputImage;
boolean inputMethodSelected = false;
PFont font;

boolean debug = false;
boolean debug_rain = false;


//Setup 
int threshold = 128;
int max_num_raindrops = 2048;
int num_raindrops = 0;
Rain[] rain_array = new Rain[max_num_raindrops];


void setup() {
  size(1280, 720);  
  inputImage = createImage(width, height, RGB);
  //Range C
  //Font
  font = createFont("Georgia", 15);
  textFont(font);
}


void draw() {
  // When the program first starts, draw a menu of different options for which camera to use for input
  // The input method is selected by pressing a key 0-9 on the keyboard
  if (!inputMethodSelected) {
    cameras = Capture.list();
    int y=40;
    text("O: Offline mode, test with TextRainInput.mov movie file instead of live camera feed.", 20, y);
    y += 40; 
    for (int i = 0; i < min(9,cameras.length); i++) {
      text(i+1 + ": " + cameras[i], 20, y);
      y += 40;
    }
    return;
  }


  // This part of the draw loop gets called after the input selection screen, during normal execution of the program.
  //Randomly add more rain
  if (num_raindrops < max_num_raindrops){
    if (random(1) > 0.5){
      rain_array[num_raindrops] = new Rain();
      rain_array[num_raindrops].init_time(millis());
      num_raindrops++;
    }
    //Range A
    //There is 1% of producing whole words at the same time
    if (random(1) < 0.01){
      int x = floor(random(width - 10*letters.length() ));
      for (int i = 0 ; i < letters.length(); i++){
        rain_array[num_raindrops] = new Rain();
        rain_array[num_raindrops].init_time(millis());
        rain_array[num_raindrops].setDropLetter(i);
        rain_array[num_raindrops].setXPosition(x);
        x+=10;
        num_raindrops++;
      }
    }
  }
  
  // STEP 1.  Load an image, either from a movie file or from a live camera feed. Store the result in the inputImage variable
  
  if ((cam != null) && (cam.available())) {
    cam.read();
    inputImage.copy(cam, 0,0,cam.width,cam.height, 0,0,inputImage.width,inputImage.height);
  }
  else if ((mov != null) && (mov.available())) {
    mov.read();
    inputImage.copy(mov, 0,0,mov.width,mov.height, 0,0,inputImage.width,inputImage.height);
  }


  // Fill in your code to implement the rest of TextRain here..
  for (int i = 0; i <= num_raindrops; i++){
      if (rain_array[i] != null){
        rain_array[i].update();
      }
  }

  
  // Tip: This code draws the current input image to the screen
  //  Range B
  //  Flip the video image
  //  Changing set() to image()
  //  After transformation, the cooridinate of (0,0) to (-width,0)
  pushMatrix(); 
  scale(-1,1); 
  image(inputImage, -width, 0);
  popMatrix(); 
  //set(0,0,inputImage);
  
  //  Range B 
  //  Debug Mode: THRESHOLD
  //  Default: GRAY
  if (debug){
    filter(THRESHOLD);
  }
  filter(GRAY);
  
  for (int i = 0; i <= num_raindrops; i++){
      if (rain_array[i] != null){
        //rain_array[i].update();
        rain_array[i].drop();
      }
  }
  
  
}//draw




void keyPressed() {
  if (!inputMethodSelected) {
    // If we haven't yet selected the input method, then check for 0 to 9 keypresses to select from the input menu
    if ((key >= '0') && (key <= '9')) { 
      int input = key - '0';
      if (input == 0) {
        println("Offline mode selected.");
        mov = new Movie(this, "TextRainInput.mov");
        mov.loop();
        inputMethodSelected = true;
      }
      else if ((input >= 1) && (input <= 9)) {
        println("Camera " + input + " selected.");           
        // The camera can be initialized directly using an element from the array returned by list():
        cam = new Capture(this, cameras[input-1]);
        cam.start();
        inputMethodSelected = true;
      }
    }
    return;
  }


  // This part of the keyPressed routine gets called after the input selection screen during normal execution of the program
  // Fill in your code to handle keypresses here..
  //  Range B 
  //  Threshold Adjustment
  if (key == CODED) {
    if (keyCode == UP) {
      // up arrow key pressed
      threshold++;
      if(debug)println("threshold: ",threshold);
    }
    else if (keyCode == DOWN) {
      // down arrow key pressed
      threshold--;
      if(debug)println("threshold: ",threshold);
    }
  }
  //  Range B 
  //  Debug Mode
  else if (key == ' ') {
    // space bar pressed
    if (debug){
      debug = false;
    }else{
      debug = true;
    }
  } 
  
}