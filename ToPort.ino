
// http://www.bajdi.com 


2 // Bob the biped 


3 // Basic walking gait code 


4   


5 #include <Servo.h> 


6 // 4 servos 


7 Servo leftFootServo; 


8 Servo leftHipServo; 


9 Servo rightFootServo; 


10 Servo rightHipServo; 


11   


12 const int offset = 20; //how many degrees we want to move the servos 


13   


14 // Servo positions 


15 // Left foot servo 


16 const int leftFootC = 85;  // centered 


17 const int leftFootD = leftFootC-offset; // foot down 


18 const int leftFootU = leftFootC+offset; // foot up 


19   


20 // Left hip servo 


21 const int leftHipC = 85; // centered 


22 const int leftHipL = leftHipC+offset; // hip left 


23 const int leftHipR = leftHipC-offset; // hip right 


24   


25 // Right foot servo 


26 const int rightFootC = 90; 


27 const int rightFootD = rightFootC-offset; 


28 const int rightFootU = rightFootC+offset; 


29   


30 // Right hip servo 


31 const int rightHipC = 100; 


32 const int rightHipL = rightHipC-offset; 


33 const int rightHipR = rightHipC+offset; 


34   


35 // Servo positions we want the servo to move to (in small steps) 


36 float leftFootPos; 


37 float leftHipPos; 


38 float rightFootPos; 


39 float rightHipPos; 


40   


41 // Servo postions written to the servos 


42 float leftFootPosInt = leftFootC; 


43 float leftHipPosInt = leftHipC; 


44 float rightFootPosInt = rightFootC; 


45 float rightHipPosInt = rightHipC; 


46   


47 // Calculated values for moving servos in small steps (position we want to move to - current servo position / steps) 


48 float leftFootStep; 


49 float leftHipStep; 


50 float rightFootStep; 


51 float rightHipStep; 


52   


53 const int steps = 20;  // divide every servo move in 20 steps 


54 byte speed = 40;  // time between steps 


55 unsigned long SuperTurboTimer; 


56 byte SuperTurboStep = 1; 


57 unsigned long SuperTurboMillis; 


58 unsigned long timer; 


59 byte direction; 


60 byte legInStep = 1; 


61   


62 void setup() 


63 { 


64   leftFootServo.write(leftFootC); 


65   leftFootServo.attach(4); 


66   leftHipServo.write(leftHipC); 


67   leftHipServo.attach(5); 


68   rightFootServo.write(rightFootC); 


69   rightFootServo.attach(7); 


70   rightHipServo.write(rightHipC); 


71   rightHipServo.attach(6); 


72   delay(3000); 


73 } 


74   


75 void loop() 


76 { 


77   //direction = 0; //stopped 


78   direction = 1; // forward 


79   // direction = 2; // backward 


80   // direction = 3; // left 


81   // direction = 4; // right 


82   walk(); 


83 } 


84   


85 // This function makes Bob walk 


86 void walk() 


87 { 


88   SuperTurboMillis = millis(); 


89   


90   if (SuperTurboMillis >= timer){ 


91     timer = timer+speed; 


92   


93     legInStep = legInStep + 1; 


94     if (legInStep == steps + 1){ 


95       legInStep = 1; 


96     } 


97     if (legInStep == 1) 


98     { 


99       leftFootStep = (leftFootPos - leftFootPosInt) / steps; 


100       leftHipStep = (leftHipPos - leftHipPosInt) / steps; 


101       rightFootStep = (rightFootPos - rightFootPosInt) / steps; 


102       rightHipStep = (rightHipPos - rightHipPosInt) / steps; 


103     } 


104   


105     leftFootPosInt = leftFootPosInt + leftFootStep; 


106     leftHipPosInt = leftHipPosInt + leftHipStep; 


107     rightFootPosInt = rightFootPosInt + rightFootStep; 


108     rightHipPosInt = rightHipPosInt + rightHipStep; 


109   } 


110   


111   if (SuperTurboMillis >= SuperTurboTimer){ 


112     SuperTurboTimer = SuperTurboTimer+(steps*speed); 


113     SuperTurboStep = SuperTurboStep +1; 


114     if (SuperTurboStep == 7){ 


115       SuperTurboStep = 1; 


116     } 


117   } 


118   


119   if (direction == 0)  // stop, both feet on the ground 


120   { 


121     leftFootPos = leftFootC; 


122     leftHipPos = leftHipC; 


123     rightFootPos = rightFootC; 


124     rightHipPos = rightHipC; 


125   } 


126   


127   if (direction == 1)     // forward walking gait 


128   { 


129     if (SuperTurboStep == 1) 


130     { 


131       leftFootPos = leftFootU; 


132       rightFootPos = rightFootD; 


133     } 


134     if (SuperTurboStep == 2) 


135     { 


136       leftHipPos = leftHipL; 


137       rightHipPos = rightHipR; 


138     } 


139     if (SuperTurboStep == 3) 


140     { 


141       leftFootPos = leftFootC; 


142       rightFootPos = rightFootC; 


143     } 


144     if (SuperTurboStep == 4) 


145     { 


146       leftFootPos = leftFootD; 


147       rightFootPos = rightFootU; 


148     } 


149   


150     if (SuperTurboStep == 5) 


151     { 


152       leftHipPos = leftHipR; 


153       rightHipPos = rightHipL; 


154     } 


155     if (SuperTurboStep == 6) 


156     { 


157       leftFootPos = leftFootC; 


158       rightFootPos = rightFootC; 


159     } 


160   } 


161   


162   if (direction == 2)     // backward walking gait 


163   { 


164     if (SuperTurboStep == 1) 


165     { 


166       leftFootPos = leftFootU; 


167       rightFootPos = rightFootD; 


168     } 


169   


170     if (SuperTurboStep == 2) 


171     { 


172       leftHipPos = leftHipR; 


173       rightHipPos = rightHipL; 


174     } 


175   


176     if (SuperTurboStep == 3) 


177     { 


178       leftFootPos = leftFootC; 


179       rightFootPos = rightFootC; 


180     } 


181   


182     if (SuperTurboStep == 4) 


183     { 


184       leftFootPos = leftFootD; 


185       rightFootPos = rightFootU; 


186     } 


187   


188     if (SuperTurboStep == 5) 


189     { 


190       leftHipPos = leftHipL; 


191       rightHipPos = rightHipR; 


192     } 


193   


194     if (SuperTurboStep == 6) 


195     { 


196       leftFootPos = leftFootC; 


197       rightFootPos = rightFootC; 


198   


199     } 


200   } 


201   


202   if (direction == 3)     // left walking gait 


203   { 


204     if (SuperTurboStep == 1) 


205     { 


206       leftFootPos = leftFootU; 


207       rightFootPos = rightFootD; 


208     } 


209     if (SuperTurboStep == 2) 


210     { 


211       leftHipPos = leftHipL; 


212       rightHipPos = rightHipL; 


213     } 


214     if (SuperTurboStep == 3) 


215     { 


216       leftFootPos = leftFootC; 


217       rightFootPos = rightFootC; 


218     } 


219     if (SuperTurboStep == 4) 


220     { 


221       leftFootPos = leftFootD; 


222       rightFootPos = rightFootU; 


223     } 


224   


225     if (SuperTurboStep == 5) 


226     { 


227       leftHipPos = leftHipR; 


228       rightHipPos = rightHipR; 


229     } 


230     if (SuperTurboStep == 6) 


231     { 


232       leftFootPos = leftFootC; 


233       rightFootPos = rightFootC; 


234     } 


235   } 


236   


237   if (direction == 4)     // right walking gait 


238   { 


239     if (SuperTurboStep == 1) 


240     { 


241       leftFootPos = leftFootU; 


242       rightFootPos = rightFootD; 


243     } 


244     if (SuperTurboStep == 2) 


245     { 


246       leftHipPos = leftHipR; 


247       rightHipPos = rightHipR; 


248     } 


249     if (SuperTurboStep == 3) 


250     { 


251       leftFootPos = leftFootC; 


252       rightFootPos = rightFootC; 


253     } 


254     if (SuperTurboStep == 4) 


255     { 


256       leftFootPos = leftFootD; 


257       rightFootPos = rightFootU; 


258     } 


259   


260     if (SuperTurboStep == 5) 


261     { 


262       leftHipPos = leftHipL; 


263       rightHipPos = rightHipL; 


264     } 


265     if (SuperTurboStep == 6) 


266     { 


267       leftFootPos = leftFootC; 


268       rightFootPos = rightFootC; 


269     } 


270   


271   } 


272   leftFootServo.write(leftFootPosInt); 


273   leftHipServo.write(leftHipPosInt); 


274   rightFootServo.write(rightFootPosInt); 


275   rightHipServo.write(rightHipPosInt); 


276 } 
