# ArdusubFrameGenerator
A simple desktop app to add custom frames in the ArduSub project
<p align="center"><img src="https://media.giphy.com/media/l3q2EsPDe0WsMOQ36/source.gif"></p>

## USAGE

1. Copy the ardusubFrameGenerator (It's in prebuilds folder)  to the ardupilot folder. 
2. Give it permissions for execution sudo chmod u+x ardusubFrameGenerator
3. Run.
4. Set the number of motor that your are going to use in the new frame.
5. Configure each motor in the List below (Set the MotorChannel and geometry factors)
6. Push Generate button and that's all.
7. Compile ardusub with command "make px4-v2" or "make px4-v2-upload".
8. Choose your frame with the FRAME_CONFIG parameters.

## RESULTS
ArdusubFrameGenerator modifys the following files thus: (The custom number motor is X and factos values are [a-f])

### FILE  libraries/AP_Motors6DOF.cpp
  		case AS_MOTORS_CUSTOM_FRAME:
	    	+add_motor_raw_6dof(AP_MOTORS_MOT_X,		 a,		 b,		 c,		 d,		 e,		 f, 		 X);
	    	+break;// ArdusubFrameGenerator



