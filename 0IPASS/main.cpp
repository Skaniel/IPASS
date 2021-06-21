#include "hwlib.hpp"
#include "mpu6050.hpp"

void display_data(hwlib::i2c_bus &i2c_bus){
    //construct MPU6050 and Oled display
    auto mpu = mpulibrary::mpu_6050(i2c_bus);
    auto display = hwlib::glcd_oled(i2c_bus);

    //setup mpu6050 for use
    mpu.init();


    //split display in 4 parts
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
    auto font1 = hwlib::font_default_16x16();
    auto font2 = hwlib::font_default_8x8();

    auto display1 = hwlib::terminal_from(window1, font1); //Text window (terminal)
    auto display2 = hwlib::terminal_from(window3, font1); //Text window (terminal)
    auto display3 = hwlib::terminal_from(window4, font2); //Text window (terminal)

    //clear display
    display.clear();
    display.flush();

    //draw line/ rectangle
    for (int i = 0; i < window2.size.x; ++i) {
        for (int j = 0; j < display2.size.y; ++j) {
            window2.write(hwlib::xy(i, j));
        }
    }

//endless loop
    int x = 0;
    while (x < 100) {
        mpu.update();
        int16_t temp = mpu.get_Temp();
        int16_t x_accel = mpu.get_X_Accel();
        int16_t y_accel = mpu.get_Y_Accel();
        int16_t z_accel = mpu.get_Z_Accel();

        int16_t angle_x = mpu.get_angle();

        hwlib::cout << "Temp: " << temp / 100 << "." << temp - ((temp / 100) * 100) << hwlib::endl; //convert scaled int
        hwlib::cout << "X_Accel: " << x_accel << hwlib::endl;
        hwlib::cout << "Y_Accel: " << y_accel << hwlib::endl;
        hwlib::cout << "Z_Accel: " << z_accel << hwlib::endl << hwlib::endl;
        hwlib::cout << "angle_x: " << angle_x << hwlib::endl << hwlib::endl;

        //display text on oled
        //(~) replaced with Degree symbol in font_default_8x8 and font_default_16x16
        display1 << "\f" << "FORKTILT" << hwlib::flush;
        window3.clear();//remove trailing degree signs

//        if (x_accel == 0) {
//            display2 << "\f" << "0(level)" << hwlib::flush;
//        } else
        display2 << "\t0300" << hwlib::showpos << angle_x << "~" << hwlib::flush;

        display3 << "\f" << "IC Temp:" << temp / 100 << "." << temp - ((temp / 100) * 100) << "~C"
                 << hwlib::flush; //show scaled int on oled display
    }
}


int main() {
    // kill the watchdog
    WDT->WDT_MR = WDT_MR_WDDIS;
    hwlib::wait_ms(1000); //wait for the terminal to start
    hwlib::cout << "TERMINAL STARTED" << hwlib::endl;

    //Pins for i2c bus
    auto scl = hwlib::target::pin_oc(hwlib::target::pins::scl);
    auto sda = hwlib::target::pin_oc(hwlib::target::pins::sda);
    auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);

    display_data(i2c_bus);
}