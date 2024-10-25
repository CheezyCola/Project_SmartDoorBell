#include "Controls.h"

Controls::Controls()
{
}

Controls::~Controls()
{
    pinMode(TonePin, INPUT);
    pullUpDnControl(TonePin, PUD_DOWN);
    pinMode(ServoPin, INPUT);
    pullUpDnControl(ServoPin, PUD_DOWN);
}

int Controls::Initialize()
{
    if(wiringPiSetup() != 0){
        std::cout << "[Error] Controls: Setup wiringPi failed" << std::endl;
        return -1;
    }else{
        pullUpDnControl(TonePin, PUD_OFF);
        pinMode(TonePin, OUTPUT);

        pullUpDnControl(ServoPin, PUD_OFF);
        pinMode(ServoPin, OUTPUT);

        pinMode(SwitchPin, INPUT);
        pullUpDnControl(SwitchPin, PUD_DOWN);

        pinMode(LEDButtonPin, INPUT);
        pullUpDnControl(LEDButtonPin, PUD_DOWN);

        if (softPwmCreate(ServoPin, 0, 100) != 0){
            std::cout << "[Error] Controls: Setup soft pwm failed" << std::endl;
            return -1;
        }
        if (softToneCreate(TonePin) != 0){
            std::cout << "[Error] Controls: Setup soft tone failed" << std::endl;
            return -1;
        }

        return 0;
    }
}

int Controls::DoorBell()
{
    try{
        tone = {846, 0, 1044, 0, 1200, 0, 1310, 0, 1200, 0, 1044, 0, 846, 0};
        time = {200, 20, 200, 20, 200, 20, 200, 20, 200, 20, 200, 20, 200, 20};
        for(size_t i=0; i<tone.size(); i++){
            softToneWrite(TonePin,tone[i]);
            delay(time[i]);
        }
        return 0;
    }catch (const std::exception &exp){
        std::cout << "[Error] Controls: " << exp.what() << std::endl;
        return -1;
    }

}

int Controls::OnMusic()
{
    try{
        tone = {262, 0, 196, 0, 196, 0, 220, 0, 196, 0, 0, 0, 247, 0, 262, 0};
        time = {200, 15, 200, 15, 200, 15, 200, 15, 200, 15, 200, 15, 200, 15, 200, 15};
        for(size_t i=0; i<tone.size(); i++){
            softToneWrite(TonePin,tone[i]);
            delay(time[i]);
        }
        return 0;
    }catch (const std::exception &exp){
        std::cout << "[Error] Controls: " << exp.what() << std::endl;
        return -1;
    }
}

int Controls::BeepSound()
{
    try{
        softToneWrite(TonePin,350);
        delay(100);
        softToneWrite(TonePin,0);
        delay(400);
        return 0;
    }catch (const std::exception &exp){
        std::cout << "[Error] Controls: " << exp.what() << std::endl;
        return -1;
    }
}

int Controls::AlertSound()
{
    try{
        tone = {1044, 0, 846, 0};
        time = {200, 20, 200, 20};
        for(size_t i=0; i<tone.size(); i++){
            softToneWrite(TonePin,tone[i]);
            delay(time[i]);
        }
        return 0;
    }catch (const std::exception &exp){
        std::cout << "[Error] Controls: " << exp.what() << std::endl;
        return -1;
    }
}

int Controls::CheckIsDoorOpen(bool &status)
{
    // Switch is NO
    // output 0 when not pressed
    // output 1 when pressed
    try{
        if(digitalRead(SwitchPin)) status = false; // when pressed, output = 1, door is not open, open = false
        else status = true;
        return 0;
    }catch (const std::exception &exp){
        std::cout << "[Error] Controls: " << exp.what() << std::endl;
        return -1;
    }
}

int Controls::CheckIsDoorBellRing(bool &status)
{
    // Push button is NC
    // output 1 when not pushed
    // output 0 when pushed
    try{
        if(digitalRead(LEDButtonPin)) status = false; // when pushed, output is 0, bell is rang, ring = true
        else status = true;
        return 0;
    }catch (const std::exception &exp){
        std::cout << "[Error] Controls: " << exp.what() << std::endl;
        return -1;
    }
}

int Controls::openDoor()
{
    try{
    softPwmWrite(BCM13, 20);
    delay(100);
    softPwmWrite(BCM13, 0);
    return 0;
    }catch (std::exception &exp){
        std::cout << "[Error] Controls: " << exp.what() << std::endl;
        return -1;
    }
}

int Controls::closeDoor()
{
    try{
    softPwmWrite(BCM13, 3);
    delay(100);
    softPwmWrite(BCM13, 0);
    return 0;
    }catch (std::exception &exp){
        std::cout << "[Error] Controls: " << exp.what() << std::endl;
        return -1;
    }
}
