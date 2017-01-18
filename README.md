# ArdusubFrameGenerator
A simple desktop app to add custom frames in the ArduSub project

##USAGE

1. Copy the ardusubFrameGenerator to the ardusub folder. (It's in prebuilds folder)
2. Give it permissions for execution sudo chmod u+x ardusubFrameGenerator
3. Run.
4. Fill the custom frame name that you want (Avoid to use bash reserved words, it may cause problems with Ardusub/deploy.sh file).
5. Set the number of motor that your are going to use in the new frame.
6. Configure each motor in the List below (Set the MotorChannel and geometry factors)
7. Push Generate button and that's all.
8. Finally compile ardusub and select your custom frame.

##RESULTS
ArdusubFrameGenerator modifys the following files thus: (The custom frame name is: xxx)

###FILE  ArduSub/Sub.h
	#define MOTOR_CLASS AP_MotorsSimpleROV
	#elif FRAME_CONFIG == VECTORED90_FRAME
 	#define MOTOR_CLASS AP_MotorsVectored90
	+#elif FRAME_CONFIG == XXX_FRAME
	+ #define MOTOR_CLASS AP_MotorsXxxROV 


###FILE  ArduSub/config.h
	#define FRAME_CONFIG_STRING "ROV_SIMPLEROV_FRAME"
	+#elif FRAME_CONFIG == XXX_FRAME
	+# define FRAME_CONFIG_STRING "ROV_XXX_FRAME"  

###FILE ArduSub/defines.h
	#define VECTORED6DOF_FRAME 12
	#define SIMPLEROV_FRAME 13
	#define VECTORED90_FRAME 14
	+#define XXX_FRAME 15    //Consecutively

###FILE Ardusub/deploy.sh	 
	BOARDS=(px4-v2)
	-TARGETS=(bluerov vectored vectored6dof simplerov)
	+TARGETS=(bluerov vectored vectored6dof simplerov xxx)

###FILE libraries/AP_Motors/AP_Motors.h
	+#include "AP_MotorsXxxROV.h"

###FILE mk/targets.mk
	+# cope with copter and hil targets NAUTILUS_MOD
	+FRAMES = quad tri hexa y6 octa octa-quad heli single coax bluerov vectored vectored6dof simplerov vectored90 obc nologging xxx  



###FILES created:
	/libraries/AP_MotorsXxxROV.h
	/libraries/AP_MotorsXxxROV.cpp

##TO COMPILE 
sudo make px4-v2-xxx
sudo make px4-v2-xxx-uplaod


