float velocity = 0; /*initial speed*/
float gravity = 20; /*accerleration*/
String letters = "TextRains";

//helper function for calculating time
float time(float time_before){
  float diff = 0;
  if (time_before == 59){
    diff = 1;
  }else{
    diff = second() - time_before;
  }
  return diff;
}


class Rain{
 float time;
 int x_position, y_position;
 float color_r, color_g, color_b;
 char drop_letter;
 
 //Varibles for update
 float delta_time = 0;
 float brightness, brightness_up, brightness_left, brightness_right;
 color color_at_raindrop, color_above_raindrop, color_left_raindrop, color_right_raindrop;
 boolean down,stop;
 Rain(){
   time = millis();
   x_position = floor(random(width));
   y_position = 0;
   // Randomly Generate Color
   color_r = random(255);
   color_g = random(255);
   color_b = random(255);
   // Picking a random letter from letters
   drop_letter = letters.charAt( floor(random(letters.length() - 1 )) );
 }
 void init_time(float t){
   time = t;
 }
 
 void setDropLetter(int indexes){
   drop_letter = letters.charAt(indexes);
 }
 void setXPosition(int x){
   x_position = x;
 }
 
 
 void update(){
   //calculating clock time
   delta_time =  (millis() - time) * 0.01;
   //get RGB value
   color_at_raindrop = get( x_position,  y_position);
   color_above_raindrop = get( x_position,  y_position) - 1 ;
   color_left_raindrop = get( x_position - 1 ,  y_position) + 1;
   color_right_raindrop = get( x_position + 1,  y_position) + 1;
   //convert color to brightness to compare with threshold
   brightness = brightness(color_at_raindrop);
   brightness_up = brightness(color_above_raindrop);
   brightness_left = brightness(color_left_raindrop);
   brightness_right = brightness(color_right_raindrop);
   
  
   //Downward
   if (brightness > threshold){
     //for debug color
     down = true;
     stop = false;
     // y_d = Vi*T + 1/2 * A * T^2
     // y_v = Vi + (A * T)
     if (delta_time > 0){
       //  Range A
       //  Speed Variation
       if (y_position > height / 3){
         gravity = 30;
       }else{ 
         gravity = 20;
       }
       //  Range A
       //  Clock rain fall
       y_position += floor (delta_time * velocity + (0.5 * gravity * delta_time * delta_time ) );
       //velocity = velocity + gravity * delta_time;
       if (debug_rain){
         println("brightness: ", brightness);
         println("millis(): ", millis());
         println("delta_time: ", delta_time);
         println("y_position: ",y_position);
         println("velocity: ",velocity);
       }  
     } 
     
    //Stop
   }else{
      //Range A
      //Upward
     while (y_position > 1 && brightness_up < threshold){
       y_position--;
       color_above_raindrop = get( x_position,  y_position) - 1 ;
       brightness_up = brightness(color_above_raindrop);
       //Calling up Recursively since y_position only-- and need to find correct position
       //update();
     }
     //flow
       //left 
    for (int i = 0 ; i < 10; i++){
      if (x_position > 1 &&  x_position < width - 1 && brightness_left > threshold){
        x_position--;
        y_position++;
        color_left_raindrop = get( x_position - 1,  y_position) + 1 ;
        brightness_left = brightness(color_left_raindrop);
        //update();
      }
     }
    
      //right
      for (int i = 0 ; i < 10; i++){
        if (x_position > 1 &&  x_position < width - 1 && brightness_right > threshold){
          x_position++;
          y_position++;
          color_right_raindrop = get( x_position + 1,  y_position) + 1 ;
          brightness_right = brightness(color_right_raindrop);
          //update();
        }
      }
    
    
     //for debug color
     down = false;
     stop = true;
   }
   
    
    //Rain goes back to top when it reach the end
    if (y_position >= height){
      y_position %= height;
      x_position = floor(random(width));
    }
    
    
   
   time = millis();
 }//Update()
 
 void drop(){
       if (debug){
         if (down){fill(0, 255, 0, 255);}else {fill(255, 0, 0, 255);}
       }else{
         fill(color_r, color_g, color_b, 255);
       }
       text(drop_letter, x_position, y_position);
     
  }
  
}//Rain Class