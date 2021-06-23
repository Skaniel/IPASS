
//          Copyright Daniel Hoegee 2020 - 2021.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "mpu6050.hpp"

void mpulibrary::mpu_6050::update() {
    read_regs();
    xyz_accel_avg.x = smoothData(xyz_accel_raw.x, smooth_accel_x);
    xyz_accel_avg.z = smoothData(xyz_accel_raw.y, smooth_accel_y);
    xyz_accel_avg.y = smoothData(xyz_accel_raw.z, smooth_accel_z);
    calculate_angle();
}

void mpulibrary::mpu_6050::initialize() {
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

int16_t mpulibrary::mpu_6050::smoothData(int16_t rawData, int16_t *sensorArray) {
    int16_t j, k, temp, top, bottom, total;
    static int16_t i;
    static int16_t sorted[samples];
    bool done;

    i = (i + 1) % samples;    //increment counter and roll over
    sensorArray[i] = rawData;   //input new data into the oldest slot

    for (j = 0; j < samples; j++) {     //Transfer data in new array for sorting and averaging
        sorted[j] = sensorArray[j];
    }
    done = false;
    while (done != 1) { //Swap sort, sorts numbers from lowest to highest
        done = true;
        for (j = 0; j < (samples - 1); j++) {
            if (sorted[j] > sorted[j + 1]) {     //If numbers are not sorted - swap them
                temp = sorted[j + 1];
                sorted[j + 1] = sorted[j];
                sorted[j] = temp;
                done = false;
            }
        }
    }
    //delete the top and bottom 15% of samples
    bottom = fmax(((samples * 15) / 100), 1);
    top = fmin((((samples * 85) / 100) + 1), (samples - 1));
    k = 0;
    total = 0;
    for (j = bottom; j < top; j++) {
        total += sorted[j];  // total remaining indices
        k++;
    }
    return total / k;    //Divide by total number of samples minus top and bottom

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

void mpulibrary::mpu_6050::calculate_angle() {
//calculate the angle from the x,y and z axis
    angle = atan2(xyz_accel_avg.y, sqrt(pow(xyz_accel_avg.x, 2) + pow(xyz_accel_avg.z, 2)));
    angle *= RAD_TO_DEG; //atan2 returns radians so convert radians to degrees
}
