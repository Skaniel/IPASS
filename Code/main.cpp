#include "hwlib.hpp"
#include "mpu6050.hpp"

//#define DEBUG_ALL     //if defined prints xyz accelerometer sensor data to terminal
//#define DEBUG_ANGLE     //if defined prints sensor angle to terminal

void display_data(hwlib::i2c_bus &i2c_bus) {
    //construct MPU6050 and Oled display with the i2c bus
    auto mpu = mpulibrary::mpu_6050(i2c_bus);
    auto display = hwlib::glcd_oled_i2c_128x64_fast_buffered(i2c_bus);

    //setup mpu6050 for fisrt use
    mpu.initialize();

    //split display into 4 parts
    auto window1 = hwlib::part(
            display,
            hwlib::xy(0, 0),
            hwlib::xy(128, 16));

    auto window2 = hwlib::part(
            display,
            hwlib::xy(0, 16),
            hwlib::xy(128, 4));

    auto window3 = hwlib::part(
            display,
            hwlib::xy(0, 25),
            hwlib::xy(128, 31));

    auto window4 = hwlib::part(
            display,
            hwlib::xy(0, 56),
            hwlib::xy(128, 8));

    //define fonts
    //(~) replaced with Degree (°) symbol in font_default_8x8 and font_default_16x16
    auto font1 = hwlib::font_default_16x16();
    auto font2 = hwlib::font_default_8x8();

    auto display1 = hwlib::terminal_from(window1, font1); //Text window (terminal)
    auto display2 = hwlib::terminal_from(window3, font1); //Text window (terminal)
    auto display3 = hwlib::terminal_from(window4, font2); //Text window (terminal)

    //clear display
    display.clear();
    display.flush();

    //Draw simple line/ rectangle
    for (int i = 0; i < window2.size.x; i++) {
        for (int j = 0; j < display2.size.y; j++) {
            window2.write(hwlib::xy(i, j));
        }
    }

//endless loop
    int x = 0;
    while (x < 100) {
        //update sensor values
        mpu.update();

        //store sensor values
        double angle_x = mpu.get_angle();
        int16_t temp = mpu.get_Temp();

        //display text on oled
        display1 << "\f" << "FORKTILT" << hwlib::flush;
        window3.clear();//remove trailing degree signs

        if (int(angle_x) != 0) {
            //(~) replaced with Degree (°) symbol in font_default_8x8 and font_default_16x16
            display2 << "\t0300" << hwlib::showpos << int(angle_x) << "~" << hwlib::flush;
        } else {
            //if angle is 0 add "level" to display
            display2 << "\f" << "0:level" << hwlib::flush;
        }
        display3 << "\f" << "IC Temp:" << temp / 100 << "." << temp - ((temp / 100) * 100) << "~C"
                 << hwlib::flush; //show scaled int on display

#ifdef DEBUG_ALL
        int16_t x_accel = mpu.get_X_Accel();
        int16_t y_accel = mpu.get_Y_Accel();
        int16_t z_accel = mpu.get_Z_Accel();
        hwlib::cout << "Temp: " << temp / 100 << "." << temp - ((temp / 100) * 100) << hwlib::endl; //convert scaled int
        hwlib::cout << "X_Accel: " << x_accel << hwlib::endl;
        hwlib::cout << "Y_Accel: " << y_accel << hwlib::endl;
        hwlib::cout << "Z_Accel: " << z_accel << hwlib::endl << hwlib::endl;
#endif

#ifdef DEBUG_ANGLE
        hwlib::cout << "angle_x: " << angle_x << hwlib::endl << hwlib::endl;
#endif

    }
}


int main() {
#if defined(DEBUG_ANGLE) or defined(DEBUG_ALL)
    hwlib::wait_ms(1000); //wait for the terminal to start
    hwlib::cout << "TERMINAL STARTED" << hwlib::endl;
#endif

    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;

    //Pins for i2c bus
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);

    display_data(i2c_bus); //function to display angle from the mpu6050 to the oled display
}