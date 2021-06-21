
//          Copyright Daniel Hoegee 2020 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "mpu6050.hpp"

void mpulibrary::mpu_6050::update() {
    average();
    get_angle();
}

void mpulibrary::mpu_6050::init() {
    // default at power-up:
    //    Gyro at 250 degrees second
    //    Acceleration at 2g
    //    Clock source at internal 8MHz
    //    The device is in sleep mode.

    //get mpu out of sleep mode and set PLL to gyro y
    i2c_write(PWR_MGMT_1, CLKSEL);

    //Set accelerometers sensitivity to +-8 g
    i2c_write(ACCEL_CONFIG, AFS_SEL);

    //Set gyroscope sensitivity to 500 deg/s
    i2c_write(GYRO_CONFIG, FS_SEL);
}

void mpulibrary::mpu_6050::average() {
    //reset accel and gyro
    xyz_accel = 0;
    xyz_gyro = 0;

    read_regs();
    for (int16_t i = 0; i < readings; i++) {
        read_regs();
        //Uses custom operator in xyz class
        xyz_accel += xyz_accel_raw;
        xyz_gyro += xyz_gyro_avg;
        hwlib::wait_ms(2);
    }

    //Uses custom operator in xyz class
    xyz_accel_avg = xyz_accel / readings;
    xyz_gyro_avg = xyz_gyro / readings;

}

void mpulibrary::mpu_6050::read_regs() {
    uint8_t result[NUMB_SENSOR_REGS]; //buffer for all the register data

    {
        hwlib::i2c_write_transaction wtrans = ((hwlib::i2c_bus *) (&bus))->write(
                MPU_ADDRESS); //open write transaction to MPU
        wtrans.write(ACCEL_XOUT);
        wtrans.~i2c_write_transaction(); //close write transaction

        hwlib::i2c_read_transaction rtrans = ((hwlib::i2c_bus *) (&bus))->read(
                MPU_ADDRESS); //open read transaction to MPU
        rtrans.read(result, NUMB_SENSOR_REGS);    //read value into result
        rtrans.~i2c_read_transaction(); //close read transaction
    }

    xyz_accel_raw.x = result[0] << 8 | result[1]; //ACCEL_XOUT_H and ACCEL_XOUT_L
    xyz_accel_raw.y = result[2] << 8 | result[3]; //ACCEL_YOUT_H and ACCEL_YOUT_L
    xyz_accel_raw.z = result[4] << 8 | result[5]; //ACCEL_ZOUT_H and ACCEL_ZOUT_L
    temp_raw = result[6] << 8 | result[7]; //TEMP_OUT_H and TEMP_OUT_L
    xyz_gyro_raw.x = result[8] << 8 | result[9]; //GYRO_XOUT_H and GYRO_XOUT_L
    xyz_gyro_raw.y = result[10] << 8 | result[11];//GYRO_YOUT_H and GYRO_YOUT_L
    xyz_gyro_raw.z = result[12] << 8 | result[13]; //GYRO_ZOUT_H and GYRO_ZOUT_L

    temperature = ((temp_raw * 100) / 340) + 3653; //times 100 to scale into int_16 to not use floats for printing
}

void mpulibrary::mpu_6050::i2c_write(uint8_t reg, uint8_t data) {
    hwlib::i2c_write_transaction wtrans = ((hwlib::i2c_bus *) (&bus))->write(MPU_ADDRESS);
    wtrans.write(reg);
    wtrans.write(data);
    wtrans.~i2c_write_transaction();
}

double mpulibrary::mpu_6050::get_angle() {
    angle = atan2(-xyz_accel.x, xyz_accel.z); //calculate the angle from the x and y axis
    return angle * RAD_TO_DEG; //atan2 returns radians so convert radians to degrees

}
